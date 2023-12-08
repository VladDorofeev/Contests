#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <signal.h>
#include <sys/sem.h>

enum {MESSAGE_TO_REGIST = 1, INC = 1,
    SHOW_NUM =2, DIE = 3, KILL_REGIST = 0};

pid_t father;

struct msgbuf{
    long mtype;
    int msgtext; 
};

void
do_server(char * file_name) {
    key_t key_son = ftok(file_name, 'S');
    int num = 0;
    struct msgbuf message_son;
    int msgid_son = msgget(key_son, 0);
    pid_t pid_son = getpid();
    pid_t pid_client;
    msgrcv(msgid_son, &message_son, sizeof(int), pid_son, 0);//get client pid
    pid_client = message_son.msgtext;
    while(1) {
        msgrcv(msgid_son, &message_son, sizeof(int), pid_son, 0);//get comands
        switch (message_son.msgtext) {
        case INC: num++;
            break;
        case SHOW_NUM: message_son.mtype = pid_client;
            message_son.msgtext = num;
            msgsnd(msgid_son, &message_son, sizeof(int), 0);
            break;
        case DIE: exit(0);
            break;
        default:
            break; 
        }
    }
}

void
do_client(char * file_name, int num) {
    key_t key_son = ftok(file_name, 'S');
    struct msgbuf message_son;
    int msgid_son = msgget(key_son, 0);
    pid_t pid_son = getpid();
    pid_t pid_server;
    message_son.mtype = MESSAGE_TO_REGIST;
    message_son.msgtext = pid_son;
    //message to registrator that client is
    //ready work with server, then registartor creates server
    msgsnd(msgid_son, &message_son, sizeof(int), 0);
    //get pid server
    msgrcv(msgid_son, &message_son, sizeof(int), pid_son, 0);
    pid_server = message_son.msgtext;
    message_son.mtype = pid_server;
    message_son.msgtext = pid_son;
    //server get client pid
    msgsnd(msgid_son, &message_son, sizeof(int), 0);
    message_son.msgtext = INC;
    for (int i = 0; i < num; ++i) {
        //num++
        msgsnd(msgid_son, &message_son, sizeof(int), 0);
    }
    message_son.msgtext = SHOW_NUM;
    msgsnd(msgid_son, &message_son, sizeof(int), 0);//get num
    msgrcv(msgid_son, &message_son, sizeof(int), pid_son, 0);//get num
    printf("%d\n", message_son.msgtext);
    message_son.mtype = pid_server;
    message_son.msgtext = DIE;
    msgsnd(msgid_son, &message_son, sizeof(int), 0);//exit from server
    exit(0);
}

//server zombies must to die after this
void
sig_hndlr_sec(int sig) {
    wait(NULL);
}

void
do_registrator(char * file_name) {
    signal(SIGCHLD, sig_hndlr_sec);
    key_t key_son = ftok(file_name, 'S');
    struct msgbuf message_son;
    int msgid_son = msgget(key_son, 0);
    pid_t pid_son;
    while(1) {
        //get comand to create server or exit after die all clients
        while(msgrcv(msgid_son, &message_son, sizeof(int), MESSAGE_TO_REGIST, 0) == -1){ 
        }
        if (message_son.msgtext != KILL_REGIST) {
            while((pid_son = fork()) == -1) {
                wait(NULL);
            }
            if (pid_son == 0) {
                do_server(file_name);
            } else {
                //message to client about his server
                message_son.mtype = message_son.msgtext;
                message_son.msgtext = pid_son;
                while(msgsnd(msgid_son, &message_son, sizeof(int), 0) == -1) {
                }
            }
        } else {
            while(wait(NULL) != -1) {
            }
            exit(0);
        }
    }
}

int msgid;

void
sig_hndlr(int sig) {
    if (getppid() == father) {
        while(wait(NULL) != -1) {
        }
        msgctl(msgid, IPC_RMID, NULL);
        exit(0);
    } else {
        exit(0);
    }
}

int
main(int argc, char * argv[]){
    signal(SIGINT, sig_hndlr);
    father = getpid();
    //count process cleint that is not died
    int count = 0;
    struct msgbuf message;
    message.mtype = MESSAGE_TO_REGIST;//kill type mes
    message.msgtext = KILL_REGIST;
    key_t key = ftok(argv[0], 'S');
    msgid = msgget(key, 0666 | IPC_CREAT);
    pid_t pid;
    int number;
    //launch registartor
    if (fork() == 0) {
        do_registrator(argv[0]);
    }
    while(scanf("%d", &number) == 1) {//read nums
        //printf("create new proc\n");
        while((pid = fork()) == -1) {//check if cant create process
            //printf("overflow\n");
            count--;
            wait(NULL);
        }
        //launch client
        if (pid == 0) {
            do_client(argv[0], number);
        } else {
            count++;
        }
    }
    for(int i = 0; i < count; ++i) {//kill process
        wait(NULL);
    }
    msgsnd(msgid, &message, sizeof(int), 0);
    wait(NULL);
    //kill res
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}