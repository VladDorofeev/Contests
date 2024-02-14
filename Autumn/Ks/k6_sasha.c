#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#define EOF -1

void
do_process_writer(int rd_file, int wr_file, char symbol) {
    off_t write_pos = lseek(wr_file, 0, SEEK_CUR);
    if (write_pos == 32) {
        write_pos  = lseek(wr_file, 0, SEEK_SET);
    }
    while(((write_pos - lseek(rd_file, 0, SEEK_CUR)) == 1) ||
            ((lseek(rd_file, 0, SEEK_CUR) - write_pos) == 31)) {
        usleep(500);
    }
    write(wr_file, &symbol, 1);
}

int
do_process_reader(int rd_file, int wr_file) {
    char sym;
    off_t read_pos = lseek(rd_file, 0, SEEK_CUR);
    if (read_pos == 32) {
        read_pos = lseek(rd_file, 0, SEEK_SET);
    }
    while(((read_pos - lseek(wr_file, 0, SEEK_CUR)) == 0) ||
            ((lseek(wr_file, 0, SEEK_CUR) - read_pos) == 32)) {
        usleep(500);
        if (waitpid(-1, NULL, WNOHANG) != 0) {
            return EOF;
        }
    }
    read(rd_file, &sym, 1);
    write(1, &sym, 1);
    return 0;
}

int
main(void) {
    char template[10] = "tmpXXXXXX";
    int tmp_file = mkstemp(template);
    int file_write = open(template, O_WRONLY);
    int file_read = open(template, O_RDONLY);
    unlink(template);
    close(tmp_file);
    if (fork() == 0) {
        char byte;
        while(read(0, &byte, 1) == 1) {
            do_process_writer(file_read, file_write, byte);
        }
        close(file_read);
        close(file_write);
        exit(0);
    } else {
        while(do_process_reader(file_read, file_write) != EOF) {
        }
        close(file_read);
        close(file_write);
    }
    return 0;
}