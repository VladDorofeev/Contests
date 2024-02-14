#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pid_t pid_arr[1000];
static int cnt;
static int i;
int tmp_file;

void
sig_hndlr(int sig) {
    switch (sig) {
    case SIGCONT:
        int number;
        //printf("pid in fork = %d\n", getpid());
        lseek(tmp_file, 0, SEEK_SET);
        //printf("n = %d\n", n);
        read(tmp_file, &number, sizeof(int));
        lseek(tmp_file, 0, SEEK_SET);
        number++;
        //printf("n = %d\n", n);
        write(tmp_file, &number, sizeof(int));
        close(tmp_file);
        exit(0);
        break;
    case SIGUSR2:
        for (;cnt < i; ++cnt) {
            //printf("pid in signal = %d\n", pid_arr[cnt]);
            while(kill(pid_arr[cnt], SIGCONT) == -1);
            waitpid(pid_arr[cnt], 0, 0);
        }
        break;
    default:
        break;
    }
}

int
main(void) {
    signal(SIGUSR2, sig_hndlr);
    signal(SIGCONT, sig_hndlr);
    pid_t pid;
    pid_t father;
    int n = 0;
    char template[10] = "tmpXXXXXX";
    tmp_file = mkstemp(template);
    unlink(template);
    write(tmp_file, &n, sizeof(int));
    scanf("%d", &n);
    father = getpid();
    for (; i < n; i++) {
        if ((pid = fork()) == -1) {
            //printf("overflow\n");
            i--;
            kill(father, SIGUSR2);
            i++;
            pid = fork();
        }
        pid_arr[i] = pid;
        if (pid == 0) {
            while(kill(getpid(), SIGTSTP) == -1);
        }
    }
    kill(father, SIGUSR2);
    lseek(tmp_file, 0, SEEK_SET);
    read(tmp_file, &n, sizeof(int));
    printf("%d\n", n);
    close(tmp_file);
    return 0;
}