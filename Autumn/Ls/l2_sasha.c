#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        int status;
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        if (fork() == 0) {
            execlp(argv[1], argv[1], NULL);
            exit(1);
        } else {
            wait(&status);
            if ((WIFEXITED(status) == 0) || (WEXITSTATUS(status) != 0)) {
                execlp(argv[2], argv[2], NULL);
                exit(1);
            }
            exit(0);
        }
    }
    if (fork() == 0) {
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execvp(argv[3], &argv[3]);
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    while(wait(NULL) != -1) {
    }
    return 0;
}