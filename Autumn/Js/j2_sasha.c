#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    argv++;
    int status;
    if (fork() == 0) {
        execvp(argv[0], argv);
        exit(0);
    } else {
        wait(&status);
        if (WIFEXITED(status)) {
            printf("%d\n", WEXITSTATUS(status));
        } else {
            printf("%d\n", WTERMSIG(status));
        }
    }
    return 0;
}