#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

void 
sig_hndl(int sig) {
    static int cnt_sig = 1;
    printf("%d\n", cnt_sig);
    if (cnt_sig++ == 5) {
        exit(0);
    }
}

int
main (void) {
    pid_t pid;
    signal(SIGINT, sig_hndl);
    if ((pid = fork()) == 0) {
        while(1) {
            usleep(500);
        }
    }
    signal(SIGINT, SIG_DFL);
    for (int i = 0; i < 5; ++i) {
        if (waitpid(-1, NULL, WNOHANG) == 0) {
            kill(pid, SIGINT);
            usleep(50);
        } else {
            break;
        }
    }
    wait(NULL);
    return 0;
}