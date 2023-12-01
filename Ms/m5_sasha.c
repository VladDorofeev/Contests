/*
1)активного ожидание нет в процессах,
т.к. иначе они бы просто тратили впустую
ресурсы процессора на свои циклы, ожидая сигнал.
2)Корректный доступ к файлу достигается 
использованием сигналов. Отец параллельно
создаёт процессы и ,передавая сигнал,
может дать доступ одному из процессов доступ
к критической секции, то есть к файлу.
Если один из потомков выполняется, то отец
не может дать доступ к файлу другому потомку.
Закончив свою работу, сын передаёт сигнал отцу,
и отец может запускать другой процесс.
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int tmp_file;
int can_get_access = 1;
int can_work = 0;
pid_t arr_pid[1001];
int cnt = 0;
pid_t father;

void
sig_hndlr(int sig) {
    if (getpid() != father) {
        can_work = 1;
    } else {
        can_get_access = 1;
    }
}

void
get_access(void) {
    if (can_get_access) {//if can get access then one of the processes can start
        can_get_access = 0;
        kill(arr_pid[cnt], SIGUSR1);
        cnt++;
    }
}

int
main(void) {
    signal(SIGUSR1, sig_hndlr);
    char template[10] = "tmpXXXXXX";
    tmp_file = mkstemp(template);
    unlink(template);
    int n = 0;
    write(tmp_file, &n, sizeof(int));
    scanf("%d", &n);
    pid_t pid;
    father = getpid();
    for(int i = 0;;) {
        if (i < n) {
            while((pid = fork()) == -1) {
                //printf("overflow\n");
                wait(NULL);
                get_access();
            }
            arr_pid[i] = pid;
            i++;
            if (pid == 0) {
                while(1) {//waiting access
                    if (can_work) {
                        break;
                    }
                    usleep(1000);
                }
                lseek(tmp_file, 0, SEEK_SET);
                read(tmp_file, &n, sizeof(int));
                n++;
                lseek(tmp_file, 0, SEEK_SET);
                write(tmp_file, &n, sizeof(int));
                kill(father, SIGUSR1);
                close(tmp_file);
                exit(0);
            }
        }
        if (cnt == n) {
            break;
        }
        get_access();
    }
    while (wait(NULL) != -1) {
    }
    lseek(tmp_file, 0, SEEK_SET);
    read(tmp_file, &n, sizeof(int));
    printf("%d\n", n);
    close(tmp_file);
    return 0;
}