#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

enum {COUNT_SIGNALS = 5, LAST_SIGNAL = '5', WATING_TIME = 50};

int cnt;
char buf_print[2] = "0\n";
int hapens_count = 0;

void
sig_hndlr(int sig) {
    hapens_count++;
}

int
main(void) {
    pid_t pid;
    signal(SIGINT, sig_hndlr);
    if ((pid = fork()) == 0) {
        while(1) {
            usleep(100);
            if (hapens_count--) {//if several signals come nearly simultaneously
                buf_print[0]++;  //to this process, it will do all of them
                write(1, buf_print, 2 * sizeof(char));
                if (buf_print[0] == LAST_SIGNAL) {
                    exit(0);
                }
            }
        }
    } else {
        for (int i = 0; i < COUNT_SIGNALS; ++i) {
            kill(pid, SIGINT);
            usleep(WATING_TIME);
        }
        wait(NULL);
    }
    return 0;
}