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
int can_work = 0; //zero means file available 

void
process_inc (void) {
    int num;

    //Wait another process
    while (1) {
        if (can_work == 0) {
            //Block all process
            kill(0, SIGUSR1);
            can_work = 1;
            break;
        }
    }

    //Critical section start
    read(fd, &num, sizeof(int));
    num++;
    lseek(fd, 0, SEEK_SET);
    write(fd, &num, sizeof(int));
    lseek(fd, 0, SEEK_SET);
    //Critical section end

    //Unblock all process
    kill(0, SIGUSR2);
}

void
block_process (int sig) {
    can_work += 1;
}

void
unblock_process (int sig) {
    can_work -= 1;
}


int
main (int argc, char **argv) {
    char temp_name[] = "tempXXXXXX";
    fd = mkstemp(temp_name);
    unlink(temp_name);

    int num = 0;
    write(fd, &num, sizeof(int));
    lseek(fd, 0, SEEK_SET);

    int n;
    scanf("%d", &n);

    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, SIG_IGN);

    pid_t pid;
    for (int i = 0; i < n; ++i) {
        if (fork() == 0) {
            signal(SIGUSR1, block_process);
            signal(SIGUSR2, unblock_process);
            process_inc();
            signal(SIGUSR1, SIG_IGN);
            signal(SIGUSR2, SIG_IGN);
            return 0;
        }
    }


    while (wait(NULL) != -1);

    read(fd, &num, sizeof(int));
    close(fd);
    printf("%d\n", num);

    return 0;
}


/*
    //Critical section start
    read(fd, &num, sizeof(int));
    num++;
    lseek(fd, 0, SEEK_SET);
    write(fd, &num, sizeof(int));
    //Critical section end
*/