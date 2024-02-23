#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char *** commands = NULL;
static int length = 0;

int add_to_pipeline(const char *array[], int size) {
    size++;
    length++;
    commands = realloc(commands, length * sizeof *commands);
    if (commands == NULL) {
        return 1;
    }
    commands[length - 1] = NULL;
    commands[length - 1] = realloc(commands[length - 1], (size + 1) * sizeof *commands[length - 1]);
    
    if (commands[length - 1] == NULL) {
        return 1;
    }

    for (int i = 0; i < size; i++) {
        commands[length - 1][i] = NULL;
        commands[length - 1][i] = realloc(commands[length - 1][i], strlen(array[i]) + 1);
        strcpy(commands[length - 1][i], array[i]);
    }
    commands[length - 1][size] = NULL;

    return 0;
}
 
int run_pipeline(void) {
    int fd_next[2] = {0, 1}, fd_prev[2];

    for (int i = 0; i < length; ++i) {
        fd_prev[0] = fd_next[0];
        fd_prev[1] = fd_next[1];
        if (i != length - 1)
        {
            pipe(fd_next);
        }
        if (fork() == 0)
        {
            if (i > 0)
            { 
                dup2(fd_prev[0], 0);
                close(fd_prev[0]);
                close(fd_prev[1]);
            }
            if (i != length - 1)
            {
                dup2(fd_next[1], 1);
                close(fd_next[0]);
                close(fd_next[1]);
            }
            execvp(commands[i][0], commands[i]);
            return 0;
        }
        if ((i > 0) || (i == length - 1))
        {
            close(fd_prev[0]);
            close(fd_prev[1]);
        }
    }

    while(wait(NULL) != -1);

    return 0; 
}

void free_pipeline(void) {
    for (int i = 0; i < length; i++) {
        char **argv = commands[i];
        while(*argv != NULL) {
            free(*argv);
            argv++;
        }
        free(commands[i]);
    }
    free(commands);
}