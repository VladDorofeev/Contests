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

//Type of messages
enum { MSG_COMMAND = 1, MSG_NUM = 2 };

//Command from client to server
enum { INC_NUM = 1, RET_NUM = 2, SHUT_DOWN = 3 };

struct msgbuf 
{
    long type;
    int msg;
};


void 
client (char *filename, int N) {
    key_t key = ftok(filename, 'a');
    int msgid_send = msgget(key, 0666);
    key = ftok(filename, 'b');
    int msgid_recieve = msgget(key, 0666);


    //Send N command to increase num in a server
    struct msgbuf cmd = {MSG_COMMAND, INC_NUM};
    for (int i = 0; i < N; i++) {
        msgsnd(msgid_send, &cmd, sizeof(int), 0);
    }


    //Send command to client for return num
    cmd.msg = RET_NUM;
    msgsnd(msgid_send, &cmd, sizeof(int), 0);


    //Get num from server
    struct msgbuf num;
    msgrcv(msgid_recieve, &num, sizeof(int), MSG_NUM, 0);
    printf("%d\n", num.msg);


    //Shut down server
    cmd.msg = SHUT_DOWN;
    msgsnd(msgid_send, &cmd, sizeof(int), 0);
    
    _exit(0);
}
void 
server (char *filename, int N) {
    int num = 0;

    key_t key = ftok(filename, 'a');
    int msgid_recieve = msgget(key, 0666);
    key = ftok(filename, 'b');
    int msgid_send = msgget(key, 0666);

    //Two messages for send num and receive command
    struct msgbuf cmd;
    struct msgbuf msg_num = {MSG_NUM, num};

    while (1) {
        //Receive command from client
        msgrcv(msgid_recieve, &cmd, sizeof(int), MSG_COMMAND, 0);
        switch (cmd.msg)
        {
        case INC_NUM:
            //Increase num
            num++;
            break;
        case RET_NUM:
            //Send current num to client
            msg_num.msg = num;
            msgsnd(msgid_send, &msg_num, sizeof(int), 0);
            break;
        case SHUT_DOWN:
            //Shut down the work
            _exit(0);
        default:
            break;
        }
    }
}

//Global variables for signal handler in father
int msgid1;
int msgid2;

void
delete_res (int sig) {
    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
}

int
main (int argc, char **argv) {
    signal(SIGINT, delete_res);
    
    key_t key = ftok(argv[0], 'a');
    msgid1 = msgget(key, 0666 | IPC_CREAT);
    key = ftok(argv[0], 'b');
    msgid2 = msgget(key, 0666 | IPC_CREAT);

    int N;
    scanf("%d", &N);

    if (fork() == 0) {
        server(argv[0], N);
        return 0;
    }
    if (fork() == 0) {
        client(argv[0], N);
        return 0;
    }

    wait(NULL);
    wait(NULL);

    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
    return 0;
}