#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

enum { DELAY_TIME = 50, NUM_SIG = '5' };

int need_print = 0;
char to_print[2] = {'0', '\n'};

void 
sig_hndl(int sig) {
    need_print++;
}

int
main (void) {
    pid_t pid;
    signal(SIGINT, sig_hndl);
    if ((pid = fork()) == 0) {
        while(1) {
            usleep(1000);
            if (need_print--) {
                to_print[0]++;
                write(1, to_print, 2);
            }
            if (to_print[0] == NUM_SIG) {
                exit(0);
            }
        }
    }
    signal(SIGINT, SIG_DFL);
    for (int i = 0; i < 5; ++i) {
        kill(pid, SIGINT);
        usleep(DELAY_TIME);
    }
    wait(NULL);
    return 0;
}