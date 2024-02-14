/*
Сложность задачи заключается в том,
что мы используем сигналы, и сигнал
может прийти во время вычисления.
Но число нужно печатать сразу после сигнала,
поэтому я сохраняю предыдущее число и сделал 
флаги "вычислено ли число?", если нет,
то печатается предыдущее, иначе настоящее.
*/

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {TIME_ALARM = 5};

char * number;
int is_calculated = 1;
char *prev_num;
unsigned seconds_to_write;

void
sig_hndlr(int sig) {
    switch (sig) {
    case SIGALRM:
        if (is_calculated) {
            write(1, number, strlen(number));
        } else {
            write(1, prev_num, strlen(prev_num));
        }
        printf("\n");
        alarm(TIME_ALARM);
        break;
    case SIGINT:
        seconds_to_write = alarm(0);
        printf("%u\n", seconds_to_write);
        alarm(seconds_to_write);
        break;
    default:
        break;
    }
}

//my dec
/*char *
dec(char * num_str) {
    char *ptr_end = NULL;
    char *ptr_start = NULL;
    char *ptr_cur = NULL;
    ptr_start = strpbrk(num_str, "123456789");
    ptr_end = strchr(num_str, '\0');
    ptr_end--;
    ptr_cur = ptr_end;
    while(*ptr_cur == '0') {
        *ptr_cur = '9';
        ptr_cur--;
    }
    *ptr_cur = *ptr_cur - 1;
    if ((*ptr_cur == '0') && (ptr_cur == ptr_start)) {
        if (ptr_cur == ptr_end) {
            ptr_start = ptr_cur;
        } else {
            ptr_cur++;
            ptr_start = ptr_cur;
        }
    }
    return ptr_start;
}*/

int
main(int argc, char * argv[]) {
    signal(SIGINT, sig_hndlr);
    signal(SIGALRM, sig_hndlr);
    prev_num = malloc(strlen(argv[1]) + 1);
    number = argv[1];
    alarm(TIME_ALARM);
    while(*number != '0') {
        strcpy(prev_num, number);
        is_calculated = 0;
        number = dec(number);
        is_calculated = 1;
    }
    free(prev_num);
    return 0;
}