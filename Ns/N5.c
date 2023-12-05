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
#include <errno.h>

struct msgbuf 
{
    long type;
    int msg;
};

enum { CREATE_SERVER = 0 }; 
enum { MAX_SEQ_LEN = 100 };

void 
client (char *filename, int N) {
    pid_t client_pid;
    printf("client pid = %d, his n = %d\n", client_pid, N);

    key_t key = ftok(filename, 'a');
    int msgid = msgget(key, 0666);


    struct msgbuf msg;
    msg.type = CREATE_SERVER;
    msg.msg = client_pid;
    msgsnd(msgid, &msg, sizeof(int), 0);
    printf("client: req to create server sended\n");

    msgrcv(msgid, &msg, sizeof(int), client_pid, 0);
    printf("client: server pid = %d\n", msg.msg);


    printf("client %d going stop\n", client_pid);
    exit(0);
}

void 
server (char *filename, pid_t client_pid) {
    sleep(5);
    exit(0);
}

void
registrator(char *filename) {
    printf("reg: start\n");
    key_t key = ftok(filename, 'a');
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        printf("reg: msgid broken\n");
    }
    struct msgbuf msg;
    pid_t client_pid;
    pid_t server_pid;
    while (1) {
        printf("reg: in a cycle\n");
        //Get request to create server from client
        msgrcv(msgid, &msg, sizeof(int), CREATE_SERVER, 0);
        client_pid = msg.msg;
        printf("reg: msg rcv from %d\n", client_pid);

        //Create server with client pid
        if ((server_pid = fork()) == 0) { ///////////////////////////////////////////
            server(filename, client_pid);
        }

        //Send to client his server pid
        msg.type = client_pid;
        msg.msg = server_pid;
        msgsnd(msgid, &msg, sizeof(int), 0);
        printf("reg: msg send to %d with server %d\n", client_pid, server_pid);
    }
}

//Global variables for signal handler in father
int msgid;

void
delete_res (int sig) {
    msgctl(msgid, IPC_RMID, NULL);
}

int
main (int argc, char **argv) {
    setbuf(stdout, 0);
    signal(SIGINT, delete_res);
    
    key_t key = ftok(argv[0], 'a');
    msgid = msgget(key, 0666 | IPC_CREAT);

    
    //Create Registrator
    pid_t reg_pid;
    if ((reg_pid = fork()) == 0) {
        registrator(argv[0]);
        return 0;
    }

    static int sons_n[MAX_SEQ_LEN];
    int cnt_n;
    for (cnt_n = 0; scanf("%d", &sons_n[cnt_n]) != -1; cnt_n++);

    for (int i = 0; i < cnt_n; i++) {
        if (fork() == 0) {////////////////////////////////////////////////////////////
            client(argv[0], sons_n[i]);
            return 0;
        }
    }    

    //Waiting all clients
    for (int i = 0; i < cnt_n; i++) {
        wait(NULL);
    } 

    //Stop registrator
    kill(reg_pid, SIGKILL);
    wait(NULL);

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}