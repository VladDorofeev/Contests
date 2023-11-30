#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>

enum {SEMAPHORE_SIZE = 2, SHM_SIZE = 1024};

int semid;
int msgid;
int shmid;

void
sig_hndlr(int sig) {
    //kill all resources by signal
    msgctl(msgid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

void
do_son(char * file_name) {
    key_t key = ftok(file_name, 'S');//get the same key
    //launch res
    semget(key, SEMAPHORE_SIZE, 0666);
    msgget(key, 0666);
    shmget(key, SHM_SIZE, 0666);
    exit(0);
}

int
main(int argc, char *argv[]) {
    signal(SIGINT, sig_hndlr);
    //create resources
    key_t key = ftok(argv[0], 'S');//create key for resources
    semid = semget(key, SEMAPHORE_SIZE, 0666 | IPC_CREAT);//create array of semaphores this can be used
    //to synchronize processes
    msgid = msgget(key, 0666 | IPC_CREAT);//create queue of messages this can be used to a shared
    //resource
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);//create share memory this can be used to
    // a synchronize processes and shared resource
    semctl(semid, 0, SETVAL, (int) 0);//init sem 0 by 0
    semctl(semid, 1, SETVAL, (int) 0);//init sem 1 by 0
    if (fork() == 0) {
        do_son(argv[0]);
    }
    wait(NULL);
    //kill all resources
    msgctl(msgid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}