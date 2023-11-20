#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void
sig_hndlr(int sig) {
    static int cnt;
    printf("%d\n", ++cnt);
    if (cnt == 5) {
        exit(0);
    }
}

int
main(void) {
    pid_t pid;
    signal(SIGINT, sig_hndlr);
    if ((pid = fork()) == 0) {
        while(1) {
            usleep(500);
        }
    } else {
        while(1) {
            kill(pid, SIGINT);
            if (waitpid(-1, NULL, WNOHANG) != 0) {
                break;
            }
            usleep(50);
        }
    }
    return 0;
}