#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdio.h>


enum {SEMAPHORE_SIZE = 2, SHM_SIZE = 1024};

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};


void
do_son(char * file_name) {
    key_t key = ftok(file_name, 'S');
    semget(key, 0, 0666);
    msgget(key, 0666);
    shmget(key, SHM_SIZE, 0666);
    exit(0);
}

void
do_father(char * file_name) {
    union semun arg;
    arg.val = 0;
    key_t key = ftok(file_name, 'S');
    int semid = semget(key, SEMAPHORE_SIZE, 0666 | IPC_CREAT);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, arg);
    semctl(semid, 1, SETVAL, arg);
    if (fork() == 0) {
        do_son(file_name);
    }
    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
}

int
main(int argc, char *argv[]) {
    do_father(argv[0]);
    return 0;
}