#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

void
child_proc(int f_sym, int sym) {
    if (f_sym == sym) {
        printf("%c%c", sym, sym);
    }
}


int
main (void) {
    setbuf(stdin, 0);
    int first_sym = fgetc(stdin);
    int sym;
    pid_t pid;
    while ((sym = fgetc(stdin)) != EOF) {
        if ((pid = fork()) == 0) {
            child_proc(first_sym, sym);
            return 0;
        } else if (pid < 0) {
            wait(NULL);
            if (fork() == 0) {
                child_proc(first_sym, sym);
                return 0;
            }
        }
    }
    for (; wait(NULL) != -1;);
    printf("\n");
    return 0;
}