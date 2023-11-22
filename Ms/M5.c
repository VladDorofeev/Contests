#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int fd;
pid_t pids[10000000];
int cnt_sons = 0;
int pos = 0;
pid_t p_pid;

void
critical_section (int sig) {
    int num;

    read(fd, &num, sizeof(int));
    num++;
    lseek(fd, 0, SEEK_SET);
    write(fd, &num, sizeof(int));
    lseek(fd, 0, SEEK_SET);
    close(fd);
    
    kill(p_pid, SIGUSR2);
    _exit(0);
}

void
new_process (int sig) {
    if (pos < cnt_sons) {
        kill(pids[pos++], SIGCONT);
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

    signal(SIGCONT, critical_section);
    signal(SIGUSR2, new_process);

    int n;
    scanf("%d", &n);
    
    pid_t pid;
    for (int i = 0; i < n; ++i) {
        while ((pid = fork()) == -1) {
            usleep(100);

            //check working sons
            pid_t pid_wait = waitpid(-1, 0, WNOHANG);
            if (pid_wait == 0) {
                //Haven`t working child
                raise(SIGUSR2);
            } 
        }
        if (pid == 0) {
            raise(SIGTSTP);
            return 0;
        }
        pids[cnt_sons++] = pid;

        if (i == 0) {
            raise(SIGUSR2);
        }
    }

    //waiting all sons
    int sleep_result;
    while (pos != cnt_sons) {
        sleep_result = usleep(100);
        if (sleep_result == 0) {
            //Not interrupted => no working child
            raise(SIGUSR2);
        }
    }

    while (wait(NULL) != -1);

    read(fd, &num, sizeof(int));
    close(fd);
    printf("%d\n", num);
    return 0;
}