#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static int n;
pid_t father;
pid_t pid;
pid_t pid_arr[1000];
static int cnt;
static int i;
int tmp_file;

void
sig_hndlr(int sig) {
    switch (sig) {
    case SIGCONT:
        signal(SIGTSTP, SIG_IGN);
        //printf("pid in fork = %d\n", getpid());
        lseek(tmp_file, 0, SEEK_SET);
        //printf("n = %d\n", n);
        read(tmp_file, &n, sizeof(int));
        lseek(tmp_file, 0, SEEK_SET);
        n++;
        //printf("n = %d\n", n);
        write(tmp_file, &n, sizeof(int));
        close(tmp_file);
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
    char template[10] = "tmpXXXXXX";
    tmp_file = mkstemp(template);
    unlink(template);
    write(tmp_file, &n, sizeof(int));
    scanf("%d", &n);
    father = getpid();
    for (; i < n; ++i) {
        while((pid = fork()) == -1) {
            kill(father, SIGUSR2);
        }
        pid_arr[i] = pid;
        if (pid == 0) {
            while(kill(getpid(), SIGTSTP) == -1);
            exit(0);
        }
    }
    kill(father, SIGUSR2);
    lseek(tmp_file, 0, SEEK_SET);
    read(tmp_file, &n, sizeof(int));
    printf("%d\n", n);
    close(tmp_file);
    return 0;
}