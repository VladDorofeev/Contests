#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

enum { SIZE_OF_MEM = 1024, NUM_OF_SEMS = 2 };

//Global variables for signal handler in father
int msgid;
int shmid;
int semid;

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

void
delete_res (int sig) {
    msgctl(msgid, IPC_RMID, NULL);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
}

void 
son_process (char *filename) {
    key_t key = ftok(filename, 'a');
    msgget(key, 0666);
    shmget(key, SIZE_OF_MEM, 0666);
    semget(key, NUM_OF_SEMS, 0666);
}

int
main (int argc, char **argv) {
    signal(SIGINT, delete_res);
    
    key_t key = ftok(argv[0], 'a');
    msgid = msgget(key, 0666 | IPC_CREAT);
    shmid = shmget(key, SIZE_OF_MEM, 0666 | IPC_CREAT);
    semid = semget(key, NUM_OF_SEMS, 0666 | IPC_CREAT);

    union semun arg;
    arg.val = 0;
    semctl(semid, 0, SETVAL, arg);
    semctl(semid, 1, SETVAL, arg);

    pid_t pid;
    if ((pid = fork()) == 0) {
        son_process(argv[0]);
        return 0;
    }
    
    wait(NULL);


    msgctl(msgid, IPC_RMID, NULL);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    return 0;
}