#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fd;
pid_t pids[10000];
int cnt_sons = 0;
pid_t p_pid;

void
critical_section (int sig) {
    int num;

    read(fd, &num, sizeof(int));
    num++;
    lseek(fd, 0, SEEK_SET);
    write(fd, &num, sizeof(int));
    lseek(fd, 0, SEEK_SET);

    kill(p_pid, SIGUSR2);
    _exit(0);
}

void
new_process (int sig) {
    static int pos = 0;
    if (pos < cnt_sons) {
        kill(pids[pos], SIGUSR1);
        kill(pids[pos], SIGCONT);
        pos++;
    }
}

int
main (int argc, char **argv) {
    p_pid = getpid();
    char temp_name[] = "tempXXXXXX";
    fd = mkstemp(temp_name);
    unlink(temp_name);

    int num = 0;
    write(fd, &num, sizeof(int));
    lseek(fd, 0, SEEK_SET);

    signal(SIGUSR1, critical_section);
    signal(SIGUSR2, new_process);

    int n;
    scanf("%d", &n);
    
    pid_t pid;
    for (int i = 0; i < n; ++i) {
        while ((pid = fork()) == -1) {
            wait(NULL);
        }
        if (pid == 0) {
            raise(SIGSTOP);
            return 0;
        }
        pids[cnt_sons++] = pid;
    }
    raise(SIGUSR2);

    while (wait(NULL) != -1);

    read(fd, &num, sizeof(int));
    close(fd);
    printf("%d\n", num);

    return 0;
}
