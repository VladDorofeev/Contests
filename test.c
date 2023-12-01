#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

pid_t pid = 0;


void
sig_hndl(int sig) {
    printf("zxc\n");
}


void
sig_hndl2(int sig) {
    if (pid != 0) {
        kill(pid, SIGINT);
    }
}

int
main (void) {
    if ((pid = fork()) == 0) {
        sleep(10);
        return 0;
    }
    sleep(1);
    pid_t stat = waitpid(pid, 0, WNOHANG);
    if (stat == 0) {
        printf("return 0\n");
    } else {
        printf("%d\n", stat);
    }
    return 0;
}