#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

int 
main (int argc, char **argv) {
    int status;
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        if (fork() == 0) {
            //Run prog1
            execlp(argv[1], argv[1], (char*) NULL);
            return 0;
        }
        wait(&status);

        if (!((WIFEXITED(status)) && (WEXITSTATUS(status) == 0))) {
            if (fork() == 0) {
                //Run prog2
                execlp(argv[2], argv[2], (char*) NULL);
                return 0;
            }
            wait(&status);
        } 
        return 0;
    }

    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(argv[3], &argv[3]);
        return 0;
    }
    close(fd[0]);
    close(fd[1]);

    while (wait(NULL) != -1);
    
    return 0;
}