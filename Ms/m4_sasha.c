#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

clock_t start;
clock_t end;
char * number;

void
sig_hndlr(int sig) {
    end = clock() / CLOCKS_PER_SEC;
    printf("%ld\n", 5 - (end - start));
}

void
sig_hndlr_sec(int sig) {
    write(1, number, strlen(number) * sizeof(char));
    printf("\n");
    alarm(5);
    start = clock() / CLOCKS_PER_SEC;
}

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
            ptr_start == ptr_cur;
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
    signal(SIGALRM, sig_hndlr_sec);
    number = argv[1];
    start = clock() / CLOCKS_PER_SEC;
    alarm(5);
    while(*number != '0') {
        number = dec(number);
    }
    return 0;
}