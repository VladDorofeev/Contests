#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>


unsigned long long num;
enum { SEC_TO_ALARM = 5 };


void
sig_alrm (int sig) {
    printf("%llu\n", num);
    alarm(SEC_TO_ALARM);
}

void
sig_int (int sig) {
    unsigned int to_alarm = alarm(0);
    alarm(to_alarm);
    printf("%d\n", to_alarm);
}

int
main (void) {
    scanf("%llu", &num);

    signal(SIGALRM, sig_alrm);
    signal(SIGINT, sig_int);
    
    alarm(SEC_TO_ALARM);

    while (num != 0) {
        num -= 1;
    }

    return 0;
}