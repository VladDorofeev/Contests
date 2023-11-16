#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

void
process_a(int d_pipe1, int d_pipe2){
    int num_arr[2];
    int rez[2];
    while(read(d_pipe1, &num_arr[0], 8) == 8) {
        rez[0] = num_arr[0] + num_arr[1];
        rez[1] = num_arr[0] - num_arr[1];
        write(d_pipe2, &rez[0], 8);
    }
}

void
do_son_process(int d_pipe1, int d_pipe2, int d_pipe3, int nums[2]) {
    int rez[2];
    write(d_pipe1, nums, 8);
    while(read(d_pipe2, &rez[0], 8) == 8) {
        if ((nums[1] + nums[0] == rez[0]) && (nums[0] - nums[1] == rez[1])) {
            break;
        }
        else {
            write(d_pipe3, &rez[0], 8);
        }
    }
    printf("%d %d %d %d\n", nums[0], nums[1], rez[0], rez[1]);
}

int
main(void) { 
    setbuf(stdin, 0);
    int fd[2];
    int fd_sec[2];
    pipe(fd);
    pipe(fd_sec);
    if (fork() == 0) {
        close(fd_sec[0]);
        close(fd[1]);
        process_a(fd[0], fd_sec[1]);
        close(fd[0]);
        close(fd_sec[1]);
        exit(0);
    }
    int two_num[2];
    pid_t pid;
    while(scanf("%d %d", &two_num[0], &two_num[1]) == 2) {
        if ((pid =fork()) == 0) {
            close(fd[0]);
            do_son_process(fd[1], fd_sec[0], fd_sec[1], two_num);
            close(fd_sec[1]);
            close(fd[1]);
            close(fd_sec[0]);
            exit(0);
        } else if (pid == -1) {
            while(pid == -1) {
                wait(NULL);
                pid = fork();
            }
            if (pid == 0) {
                close(fd[0]);
                do_son_process(fd[1], fd_sec[0], fd_sec[1], two_num);
                close(fd_sec[1]);
                close(fd[1]);
                close(fd_sec[0]);
                exit(0);
            }
        }
    }
    close(fd[0]);
    close(fd_sec[1]);
    close(fd[1]);
    close(fd_sec[0]);
    while(wait(NULL) != -1) {
    }
    return 0;
}