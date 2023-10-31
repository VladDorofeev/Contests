#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(int argc, char *argv[]) {
    char* ptr = argv[argc-1];
    argv[argc- 1] = NULL;
    int status;
    if (fork() == 0) {
        execvp(argv[1], &argv[1]);
        exit(0);
    } else {
        wait(&status);
        argv[argc-1] = ptr;
        if ((WIFEXITED(status) != 0) && (WEXITSTATUS(status) == 0)) {
            if (fork() == 0) {
                execvp(argv[argc-1], &argv[argc-1]);
                exit(0);
            } else {
                wait(&status);
            }
        }
    }
    return 0;
}