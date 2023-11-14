#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

int 
main (int argc, char **argv) {
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    if (fork() == 0) {
        dup2(fd1[1], STDOUT_FILENO);

        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);

        execlp("grep", "grep", "^[a-zA-Z0-9_]*(.*)", (char *)NULL);
    }
    if (fork() == 0) {
        dup2(fd1[0], STDIN_FILENO);
        dup2(fd2[1], STDOUT_FILENO);

        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);

        execlp("cut", "cut", "-d(", "-f1", (char *)NULL);
    }

    close(fd1[0]);
    close(fd1[1]);

    if (fork() == 0) {
        dup2(fd2[0], STDIN_FILENO);

        close(fd2[0]);
        close(fd2[1]);

        execlp("sort", "sort", "-u", (char *)NULL);
    }

    close(fd2[0]);
    close(fd2[1]);

    while (wait(NULL) != -1);
    return 0;
}
/*
grep \ \(.*\)\ \{ test1.c | cut -d\( -f1

grep \ \(.*\)\ \{ test1.c | cut -d\( -f1 | sort
*/