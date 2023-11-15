#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int
main(void) {
    int fd[2];
    pipe(fd);//pipe for greb output and cut input
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp("grep", "grep", "-E", "^[0-9a-z_]{1,}\\(", NULL);
        exit(1);
    }
    int fd_sec[2];
    pipe(fd_sec);//pipe for cut output and sort input
    if (fork() == 0) {
        dup2(fd[0], 0);
        dup2(fd_sec[1], 1);
        close(fd[1]);
        close(fd[0]);
        close(fd_sec[1]);
        close(fd_sec[0]);
        execlp("cut", "cut", "-d", "(", "-f", "1", NULL);
        exit(1);
    }
    close(fd[1]);
    close(fd[0]);
    if (fork() == 0) {
        dup2(fd_sec[0], 0);
        close(fd_sec[0]);
        close(fd_sec[1]);
        execlp("sort", "sort", NULL);
        exit(1);
    }
    close(fd_sec[0]);
    close(fd_sec[1]);
    while(wait(NULL) != -1){
    }
    return 0;
}