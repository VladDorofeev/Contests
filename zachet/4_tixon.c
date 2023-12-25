#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

int msgid;

enum { TO_MAIN = 1 };

/// text:
/// 0: work as usual
/// 1: end
/// num >= 2: number of son
enum { WORK = 0, END = 1 };

typedef struct Msgbuf {
    long type;
    unsigned text;
} Msgbuf;

void
delete_resources(int s)
{
    msgctl(msgid, IPC_RMID, 0);
}

void
son_process(int temp, char *argv, unsigned serial)
{
    unsigned num;
    int size = sizeof(num);
    key_t key = ftok(argv, 'k');
    msgget(key, 0666);
    Msgbuf msg;
    for(;;) {
        msgrcv(msgid, &msg, size, serial, 0);
        if (msg.text == END) {
            exit(0);
        }
        read(temp, &num, size);
        --num;
        write(temp, &num, size);
        lseek(temp, (-1) * size, SEEK_END);

        msg.type = TO_MAIN;
        if (num == 0) {
            msg.text = END;
            msgsnd(msgid, &msg, size, 0);
            exit(0);
        } else {
            msg.text = serial;
            msgsnd(msgid, &msg, size, 0);
        }
    }
}

int
main(int argc, char **argv)
{
    unsigned N, K;
    int size = sizeof(K);
    scanf("%u%u", &N, &K);
    if (N == 0) {
        return 0;
    }
    if (K == 0) {
        printf("0\n");
        return 0;
    }
    N += 2;
    char filename[6] = "XXXXXX";
    int temp = mkstemp(filename);
    unlink(filename);
    write(temp, &K, size);
    lseek(temp, 0, SEEK_SET);
    key_t key = ftok(*argv, 'k');
    msgid = msgget(key, IPC_CREAT | 0666);
    signal(SIGINT, delete_resources);
    Msgbuf msg;

    for (unsigned i = 2; i < N; ++i) {
        if (fork() == 0) {
            son_process(temp, *argv, i);
        }
    }
    msg.type = 2;
    msg.text = WORK;
    msgsnd(msgid, &msg, size, 0);

    for (;;) {
        msgrcv(msgid, &msg, size, TO_MAIN, 0);
        if (msg.text == END) {
            for (unsigned i = 2; i < N; ++i) {
                msg.type = i;
                msgsnd(msgid, &msg, size, 0);
            }
            break;
        } else {
            msg.type = msg.text + 1;
            if (msg.type == N) {
                msg.type = 2;
            }
            msg.text = WORK;
            msgsnd(msgid, &msg, size, 0);
        }
    }
    while(wait(NULL) != -1);

    lseek(temp, 0, SEEK_SET);
    unsigned num;
    while (read(temp, &num, size) > 0) {
        printf("%u\n", num);
    }
    close(temp);
    raise(SIGINT);
    return 0;
}