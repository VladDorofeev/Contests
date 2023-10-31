#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

void
do_process(const char sym, const char key) {
    if (key == sym) {
        printf("%c%c", sym, sym);
    }
}

int
main(void) {
    setbuf(stdin, 0);
    int cnt = 0;
    char first_symbol = getchar();
    int symbol;
    pid_t pid;
    while((symbol = getchar()) != EOF) {
        cnt++;
        if ((pid = fork()) == 0) {
            do_process((char)symbol, first_symbol);
            exit(0);
        } else if (pid < 0) {
            wait(NULL);
            if (fork() == 0) {
                do_process((char)symbol, first_symbol);
                exit(0);
            }
        }
    }
    for (int i = 0; i < cnt; ++i) {
        wait(NULL);
    }
    printf("\n");
    return 0;
}