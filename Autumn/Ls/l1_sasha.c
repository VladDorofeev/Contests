#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[]) {
    char **ptr = argv;
    for (;;) {
        ptr++;
        if (strcmp(*ptr, "--") == 0) {
            *ptr = NULL;
            break;
        }
    }
    ptr++;
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execvp(argv[1], &argv[1]);
        exit(1);
    }
    if (fork() == 0) {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execvp(*ptr, ptr);
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    while(wait(NULL) != -1) {
    }
    return 0;
}