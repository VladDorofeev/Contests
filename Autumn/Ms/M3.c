#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

enum { SEC_TO_ALARM = 5 };

unsigned long long num;
int need_print;
unsigned int to_alarm;

void
sig_alrm (int sig) {
    need_print = 1;
    alarm(SEC_TO_ALARM);
}

void
sig_int (int sig) {
    to_alarm = alarm(0);
    alarm(to_alarm);
}

int
main (void) {
    scanf("%llu", &num);

    signal(SIGALRM, sig_alrm);
    signal(SIGINT, sig_int);
    
    alarm(SEC_TO_ALARM);

    while (num != 0) {
        num -= 1;
        if (need_print) {
            printf("%llu\n", num);
            need_print = 0;
        }
        if (to_alarm) {
            printf("%u\n", to_alarm);
            to_alarm = 0;
        }
    }

    return 0;
}