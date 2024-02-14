#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int 
main (int argc, char **argv) {

    int fd[2];
    pipe(fd);
    int index_separ;

    for (index_separ = 1; index_separ < argc; ++index_separ) {
        
        if (strstr(argv[index_separ], "--") != NULL) {
            break;
        }
    }

    argv[index_separ] = (char *)NULL;

    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        execvp(argv[1], &argv[1]);
        return 0;
    } 

    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);

        execvp(argv[index_separ + 1], &argv[index_separ + 1]);
        return 0;
    }

    close(fd[0]);
    close(fd[1]);

    while (wait(NULL) != -1);
    
    return 0;
}