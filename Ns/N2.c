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

enum { NUM_OF_SEMS = 2 };

//Global variable for signal handler in father
int semid;

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

void
delete_res (int sig) {
    semctl(semid, 0, IPC_RMID);
}

void 
son_process (char *filename, int N, int type) {
    key_t s_key = ftok(filename, 'a');
    int s_semid = semget(s_key, NUM_OF_SEMS, 0666);


    //Semaphor[0] is associated with son type 1
    //Semaphor[1] is associated with son type 2
    
    //Sons print num if their semaphor > 0:

    //How it works?
    //Son(1) down his semaphor, print and up son(2)`s semaphor
    //Son(2) down his semaphor, print and up son(1)`s semaphor  
    
    //Semaphor[0] == 1 in start
    //Semaphor[1] == 0 in start


    struct sembuf s0_u = {0, 1, 0};//Up semaphor[0]
    struct sembuf s1_u = {1, 1, 0};//Up semaphor[1]
    struct sembuf s0_d = {0, -1, 0};//Down semaphor[0]
    struct sembuf s1_d = {1, -1, 0};//Down semaphor[1]

    if (type == 1) {
        for (int i = 0; i < N; i = i + 2) {
            //Down semaphor[0]
            semop(s_semid, &s0_d, 1);

            printf("%d\n", i);

            //Up semaphor[1]
            semop(s_semid, &s1_u, 1);
        }
    } else if (type == 2) {
        for (int i = 1; i < N; i = i + 2) {
            //Down semaphor[1]
            semop(s_semid, &s1_d, 1);

            printf("%d\n", i);
            
            //Up semaphor[0]
            semop(s_semid, &s0_u, 1);
        }
    }
}

int
main (int argc, char **argv) {
    signal(SIGINT, delete_res);
    
    key_t key = ftok(argv[0], 'a');
    semid = semget(key, NUM_OF_SEMS, 0666 | IPC_CREAT);

    //Initilization semaphors:
    //Semaphor[0] = 1
    //Semaphor[1] = 0
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    arg.val = 0;
    semctl(semid, 1, SETVAL, arg);


    int N;
    scanf("%d", &N);

    if (fork() == 0) {
        son_process(argv[0], N, 1);
        return 0;
    }
    sleep(2);
    if (fork() == 0) {
        son_process(argv[0], N, 2);
        return 0;
    }

    wait(NULL);
    wait(NULL);

    semctl(semid, 0, IPC_RMID);
    return 0;
}