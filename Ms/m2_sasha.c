#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int cnt;
int happens = 0;
char buf_print[2] = "0\n";

void
sig_hndlr(int sig) {
    buf_print[0]++;
}

int
main(void) {
    pid_t pid;
    signal(SIGINT, sig_hndlr);
    if ((pid = fork()) == 0) {
        while(1) {
            if (buf_print[0] < '6') {
                write(1, buf_print, 2 * sizeof(char));
                if (buf_print[0] == '5') {
                    exit(0);
                }
            }
            usleep(1000);
        }
    } else {
        for (int i = 0; i < 5; ++i) {
            kill(pid, SIGINT);
            usleep(50);
        }
        wait(NULL);
    }
    return 0;
}