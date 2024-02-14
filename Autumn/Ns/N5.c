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

//Type of message to create server
enum { CREATE_SERVER = 1 }; 

//Command to server
enum { INC_NUM = 1, RET_NUM = 2, SHUT_DOWN = 3 };

void 
client (char *filename, int N) {
    pid_t client_pid = getpid();

    key_t key = ftok(filename, 'a');
    int msgid = msgget(key, 0666);

    //Send request to create server
    struct msgbuf msg = {CREATE_SERVER, client_pid};
    msgsnd(msgid, &msg, sizeof(int), 0);

    //Recieve server pid
    msgrcv(msgid, &msg, sizeof(int), client_pid, 0);
    pid_t server_pid = msg.msg;

    //Send to server N command to increase num
    msg.type = client_pid;
    msg.msg = INC_NUM;
    for (int i = 0; i < N; i++) {
        msgsnd(msgid, &msg, sizeof(int), 0);
    }   

    //Get num from server
    msg.msg = RET_NUM;
    msgsnd(msgid, &msg, sizeof(int), 0);
    msgrcv(msgid, &msg, sizeof(int), server_pid, 0);

    printf("%d\n", msg.msg);

    //Close server
    msg.type = getpid(); 
    msg.msg = SHUT_DOWN;
    msgsnd(msgid, &msg, sizeof(int), 0);

    //Waiting answer that server closed
    msgrcv(msgid, &msg, sizeof(int), server_pid, 0);
    
    exit(0);
}

void 
server (char *filename, pid_t client_pid) {
    int num = 0;

    key_t key = ftok(filename, 'a');
    int msgid = msgget(key, 0666);

    //Two messages for send num and receive command
    struct msgbuf msg;

    while (1) {
        //Receive command from client
        msgrcv(msgid, &msg, sizeof(int), client_pid, 0);
        switch (msg.msg)
        {
        case INC_NUM:
            //Increase num
            num++;
            break;
        case RET_NUM:
            //Send current num to client
            msg.type = getpid();
            msg.msg = num;
            msgsnd(msgid, &msg, sizeof(int), 0);
            break;
        case SHUT_DOWN:
            //Shut down the work
            msg.type = getpid();
            msg.msg = SHUT_DOWN;
            msgsnd(msgid, &msg, sizeof(int), 0);
            exit(0);
        default:
            break;
        }
    }
}

void
registrator(char *filename) {
    key_t key = ftok(filename, 'a');
    int msgid = msgget(key, 0666);

    struct msgbuf msg;
    pid_t client_pid;
    pid_t server_pid;
    while (1) {
        //Get request to create server from client
        msgrcv(msgid, &msg, sizeof(int), CREATE_SERVER, 0);
        
        //If main send message to stop registrator
        if (msg.msg == 0) {
            while (wait(NULL) != -1);
            exit(0);
        }

        client_pid = msg.msg;

        //Create server with client pid
        while ((server_pid = fork()) == -1) {
            wait(NULL);
        }
        if (server_pid == 0) { 
            server(filename, client_pid);
        }

        //Send to client his server pid
        msg.type = client_pid;
        msg.msg = server_pid;
        msgsnd(msgid, &msg, sizeof(int), 0);
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

    int N; 
    int cnt_clients;
    pid_t client_pid;
    for (cnt_clients = 0; scanf("%d", &N) != -1; cnt_clients++) {
        while ((client_pid = fork()) == -1) {
            wait(NULL);
            cnt_clients--;
        }
        if (client_pid == 0) {
            client(argv[0], N);
            return 0;
        }
    }

    //Waiting all clients
    for (int i = 0; i < cnt_clients; i++) {
        wait(NULL);
    } 

    //Stop registrator
    struct msgbuf msg = {CREATE_SERVER, 0};
    msgsnd(msgid, &msg, sizeof(int), 0);
    wait(NULL);

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}