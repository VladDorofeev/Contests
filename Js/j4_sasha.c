#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void
do_process(int num) {
    if (num == 1) {
        usleep(1234);
    }
    printf("%d\n", num);
}

int
main(void) {
    int status;
    if (fork() == 0) {
        do_process(1);
        exit(0);
    } else {
        if (fork() == 0) {
            do_process(2);
            exit(0);
        }
        wait(&status);
        wait(&status);
    }
    return 0;
}