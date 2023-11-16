#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
enum {BUF_SIZE = 4096};

int
my_fgetc(int my_file_desc) {
    static char buffer[BUF_SIZE];
    static int pos_buf;
    static int cnt_sym;
    if (pos_buf == cnt_sym) {
        pos_buf = 0;
        if ((cnt_sym = read(my_file_desc, buffer, BUF_SIZE)) == 0) {
            return EOF;
        }
    }
    pos_buf++;
    return buffer[pos_buf - 1];
}

void
do_processes(void) {
    int parity = 0;
    int cur_sym;
    int pos_arr = 0;
    char byte_arr[BUF_SIZE];
    while((cur_sym = my_fgetc(0)) != EOF) {
        if (pos_arr == BUF_SIZE) {
            write(1, byte_arr, pos_arr);
            pos_arr = 0;
        }
        if (parity == 1) {
            byte_arr[pos_arr] = cur_sym;
            pos_arr++;
            parity = 0;
        } else {
            parity = 1;
        }
    }
    if (pos_arr != 0) {
        write(1, byte_arr, pos_arr);
    }
}

int
main(void) {
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        do_processes();
        exit(0);
    }
    close(fd[1]);
    if (fork() == 0) {
        if (fork() == 0) {
            dup2(fd[0], 0);
            close(fd[0]);
            do_processes();
            exit(0);
        }
        close(fd[0]);
        wait(NULL);
        exit(0);
    }
    close(fd[0]);
    while(wait(NULL) != -1) {
    }
    return 0;
}