#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

enum {TIME_ALARM = 5};

int happens_int;
int happens_alarm;

void
sig_hndlr(int sig) {
    switch (sig) {
    case SIGALRM:
        happens_alarm = 1;
        break;
    case SIGINT:
        happens_int = 1;
        break;
    default:
        break;
    }
}

int
main(void) {
    signal(SIGINT, sig_hndlr);
    signal(SIGALRM, sig_hndlr);
    unsigned long long number;
    unsigned seconds_to_write;
    scanf("%llu", &number);
    alarm(TIME_ALARM);
    while(number != 0) {
        if (happens_alarm) {
            printf("%llu\n", number);
            alarm(TIME_ALARM);
            happens_alarm = 0;
        }
        if (happens_int) {
            seconds_to_write = alarm(0);
            printf("%u\n", seconds_to_write);
            alarm(seconds_to_write);
            happens_int = 0;
        }
        number--;
    }
    return 0;
}