#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <signal.h>

struct msgbuf{
    long mtype;
    int msgtext; 
};

void
do_server(char * file_name) {
    key_t key_son = ftok(file_name, 'S');
    int num = 0;
    struct msgbuf message_son;
    int msgid_son = msgget(key_son, 0666);
    key_son = ftok(file_name, 'D');
    int msgid_son_sec = msgget(key_son, 0666);
    while(1) {
        msgrcv(msgid_son, (struct msgbuf*) (&message_son), sizeof(int), 0, 0);//get mes
        switch (message_son.mtype)
        {
        case 1: num++;//type 1 to inc num;
            break;
        case 2: message_son.msgtext = num;//type 2 to send num
            msgsnd(msgid_son_sec, (struct msgbuf*) (&message_son), sizeof(int), 0);
            break;
        case 3: exit(0);//type 3 for exit
        default:
            break;
        }
    }
}

void
do_client(char * file_name, int num) {
    key_t key_son = ftok(file_name, 'S');
    struct msgbuf message_son;
    int msgid_son = msgget(key_son, 0666);
    key_son = ftok(file_name, 'D');
    int msgid_son_sec = msgget(key_son, 0666);
    message_son.mtype = 1;
    for (int i = 0; i < num; ++i) {
        msgsnd(msgid_son, (struct msgbuf *) (&message_son), sizeof(int), 0);
    }
    message_son.mtype = 2;
    msgsnd(msgid_son, (struct msgbuf *) (&message_son), sizeof(int), 0);
    msgrcv(msgid_son_sec, (struct msgbuf*) (&message_son), sizeof(int), 0, 0);
    printf("%d\n", message_son.msgtext);
    message_son.mtype = 3;
    msgsnd(msgid_son, (struct msgbuf *) (&message_son), sizeof(int), 0);
    exit(0);
}

int msgid;
int msgid_sec;

void
sig_hndlr(int sig) {
    //kill all resources by signal
    msgctl(msgid, IPC_RMID, NULL);
    msgctl(msgid_sec, IPC_RMID, NULL);
    exit(0);
}

int
main(int argc, char * argv[]) {
    key_t key = ftok(argv[0], 'S');
    int number;
    scanf("%d", &number);
    msgid = msgget(key, 0666 | IPC_CREAT);
    key = ftok(argv[0], 'D');
    msgid_sec = msgget(key, 0666 | IPC_CREAT);
    if (fork() == 0) {
        do_server(argv[0]);
    }
    if (fork() == 0) {
        do_client(argv[0], number);
    }
    signal(SIGINT, sig_hndlr);
    while(wait(NULL) != -1) {
    }
    msgctl(msgid, IPC_RMID, NULL);
    msgctl(msgid_sec, IPC_RMID, NULL);
    return 0;
}