#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>


int
main (int argc, char **argv) {
    int status;
    char *pr_2 = argv[argc - 1];
    argv[argc - 1] = NULL;
    if (fork() == 0) {
        execvp(argv[1], &argv[1]);
        return 1;
    }
    wait(&status);

    if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)) {
        if (fork() == 0) {
            execlp(pr_2, pr_2, NULL);
            return 1;
        }
        wait(&status);
    }

    return 0;
}