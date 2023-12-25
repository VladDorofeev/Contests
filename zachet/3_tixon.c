#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

int count = 0, have_to_exit = 0;

void
sig_handler(int s)
{
    ++count;
}

void
term_handler(int s)
{
    have_to_exit = 1;
}

void
son_process(char **p1, char **p2)
{
    int status = -1;
    for(;;) {
        usleep(100);
        while (count >= 3) {
            int fd[2];
            pipe(fd);
            if (fork() == 0) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
                close(fd[1]);
                execvp(*p1, p1);
                exit(1);
            }
            if (fork() == 0) {
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                close(fd[1]);
                execvp(*p2, p2);
                exit(1);
            }
            close(fd[0]);
            close(fd[1]);
            do {
                wait(&status);
            } while (status == -1);
            status = -1;
            do {
                wait(&status);
            } while (status == -1);
            status = -1;

            count -= 3;
        }
        if (have_to_exit) {
            exit(0);
        }
    }
}

int
main(int argc, char **argv)
{
    signal(SIGUSR1, sig_handler);
    signal(SIGTERM, term_handler);
    pid_t son_pid;
    char **p1 = argv + 1;
    char **p2 = argv;
    while (strcmp(*p2, "--") != 0) {
        ++p2;
    }
    *p2 = NULL;
    ++p2;

    if ((son_pid = fork()) == 0) {
        son_process(p1, p2);
    }
    unsigned num;
    while (scanf("%u", &num) == 1) {
        if (num == 0) {
            kill(son_pid, SIGUSR1);
        } else {
            usleep(num);
        }
    }
    kill(son_pid, SIGTERM);
    wait(NULL);
    return 0;
}