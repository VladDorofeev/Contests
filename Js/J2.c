#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>



int
main (int argc, char **argv) {
    int status;
    argv++;
    if (fork() == 0) {
        //Child process
        execvp(argv[0], argv);
        return -1;
    }
    //Main process
    wait(&status);
    
    if (WIFEXITED(status)) {
        printf("%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("%d\n", WTERMSIG(status));
    }

    return 0;
}