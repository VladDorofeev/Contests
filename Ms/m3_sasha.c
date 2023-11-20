#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

clock_t start;
clock_t end;
unsigned long long number;

void
sig_hndlr(int sig) {
    end = clock() / CLOCKS_PER_SEC;
    printf("%ld\n", end - start);
}

void
sig_hndlr_sec(int sig) {
    printf("%llu\n", number);
    alarm(5);
    start = clock() / CLOCKS_PER_SEC;
}

int
main(void) {
    signal(SIGINT, sig_hndlr);
    signal(SIGALRM, sig_hndlr_sec);
    scanf("%llu", &number);
    start = clock() / CLOCKS_PER_SEC;
    alarm(5);
    while(number != 0) {
        number--;
    }
    return 0;
}