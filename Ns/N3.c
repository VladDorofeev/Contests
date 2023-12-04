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

enum { NUM_OF_SEMS = 2, MAX_NUM_TO_PRINT = 50001 };

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

void 
son_process (char *filename, int N, int type) {
    key_t key = ftok(filename, 'a');
    int s_shmid = shmget(key, (N + 2) * sizeof(int), 0666);
    int s_semid = semget(key, NUM_OF_SEMS, 0666);


    int *buf = shmat(s_shmid, 0, 0); 
    int *pos_1 = buf;
    int *pos_2 = buf + 1;

    int *iter = buf + 2; 


    //struct sembuf s0_u = {0, 1, 0};//Up semaphor[0]
    struct sembuf s1_u = {1, 1, 0};//Up semaphor[1]
    //struct sembuf s0_d = {0, -1, 0};//Down semaphor[0]
    struct sembuf s1_d = {1, -1, 0};//Down semaphor[1]

    if (type == 1) {
        for (int i = 0;; i++) {
            semop(s_semid, &s1_d, 1);
            if (*pos_2 == i) {
                semop(s_semid, &s1_u, 1);
                exit(0);
            }
            (*pos_1)++;
            (*iter++)++;
            semop(s_semid, &s1_u, 1);
        }
        exit(0);
    } else if (type == 2) {
        //Second son will be increase "middle" num
        iter = iter + N - 1;
        for (int i = N-1;; i--) {
            semop(s_semid, &s1_d, 1);
            if (*pos_1 == i) {
                semop(s_semid, &s1_u, 1);
                (*iter--)++;
                exit(0);
            }

            (*pos_2)--;
            (*iter--)++;
            semop(s_semid, &s1_u, 1);

        }
        exit(0);
    }
}

//Global variables for signal handler in father
int shmid;
int semid;

void
delete_res (int sig) {
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
}


int
main (int argc, char **argv) {
    //setbuf(stdout, 0);
    signal(SIGINT, delete_res);
    
    key_t key = ftok(argv[0], 'a');

    semid = semget(key, NUM_OF_SEMS, 0666 | IPC_CREAT);
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    semctl(semid, 1, SETVAL, arg);

    int N;
    scanf("%d", &N);
    
    shmid = shmget(key, (N + 2) * sizeof(int), 0666 | IPC_CREAT);
    int *buf = shmat(shmid, 0, 0);


    int *iter = buf;

    //In first int write start pos of son_1 (0)
    //In second int write start pos of son_2 (N)
    *iter++ = 0;
    *iter++ = N - 1;

    //printf("0\n%d\n", N-1);
    //Write start values (0 - N)
    for (int i = 0; i < N; i++) {
        *iter = i;
        //printf("%d\n", i);
        iter++;
    }
    
    if (fork() == 0) {
        son_process(argv[0], N, 1);
        return 0;
    }
    if (fork() == 0) {
        son_process(argv[0], N, 2);
        return 0;
    }
    
    wait(NULL);
    wait(NULL);

    //printf("================\n");
    if (N < MAX_NUM_TO_PRINT) {
        iter = buf + 2;
        for (int i = 0; i < N; i++) {
            printf("%d\n", *iter);
            iter++;
        }
    }
    
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    return 0;
}