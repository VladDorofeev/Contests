#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <signal.h>

enum {SEMAPHORE_SIZE = 1, ODD_SON = 1, NOT_ODD_SON = 0};

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int semid;

void
sig_hndlr(int sig) {
    //printf("kill sem\n");
    semctl(semid, 0, IPC_RMID);
    exit(0);
}

void
do_son(char * file_name, int type_son, int num) {
    struct sembuf lock_proc_odd = {0, -1, 0};
    struct sembuf lock_proc_not_odd = {0, 0, 0};
    key_t key = ftok(file_name, 'S');
    int semid_sec = semget(key, SEMAPHORE_SIZE, 0666);
    for (int i = type_son; i < num; i+=2) {
        if (type_son == ODD_SON) {
            semop(semid_sec, &lock_proc_odd, 1);//if val<= 0 it block but if val > 0 it unlock
            printf("%d\n", i);
            semctl(semid_sec, 0, SETVAL, (int) 0);//unblock not_odd_son
        } else if (type_son == NOT_ODD_SON) {
            semop(semid_sec, &lock_proc_not_odd, 1);//if val != 0 it block but if val == 0 it unlock
            printf("%d\n", i);
            semctl(semid_sec, 0, SETVAL, (int) 2);//unblock odd_son
        }
    }
    exit(0);
}

int
main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
    key_t key = ftok(argv[0], 'S');
    semid = semget(key, SEMAPHORE_SIZE, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, (int) 0);//start value = 0 because first not_odd_son
    int n;
    scanf("%d", &n);
    signal(SIGINT, sig_hndlr);//for debug
    //launch processes
    for (int i = 0; i < 2; ++i) {
        if (fork() == 0) {
            do_son(argv[0], i, n);
        }
    }
    while(wait(NULL) != -1) {
    }
    semctl(semid, 0, IPC_RMID);//kill semaphore
    return 0;
}