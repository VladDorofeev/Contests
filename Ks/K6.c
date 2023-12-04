#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

void 
special_write (int fd_read, int fd_write, char *sym) {
    off_t writer_pos = lseek(fd_write, 0, SEEK_CUR);
    if (writer_pos == 32) {
        writer_pos = lseek(fd_write, 0, SEEK_SET);
    }
    while ((writer_pos == lseek(fd_read, 0, SEEK_CUR) - 1) || 
            (writer_pos - lseek(fd_read, 0, SEEK_CUR) == 31)) {
        usleep(500);
    }
    write(fd_write, sym, 1);
}


int
special_read(int fd_read, int fd_write, char *sym) {
    off_t reader_pos = lseek(fd_read, 0, SEEK_CUR);
    if (reader_pos == 32) {
        reader_pos = lseek(fd_read, 0, SEEK_SET);
    }
    while (reader_pos == lseek(fd_write, 0, SEEK_CUR)) {
        if (waitpid(-1, 0, WNOHANG) != 0) {
            return -1;
        }
        usleep(500);
    }
    read(fd_read, sym, 1);
    return 0;
}

int 
main (void) {
    char temp_name[] = "tempXXXXXX";
    int temp_fd = mkstemp(temp_name);
    close(temp_fd);
    int fd_write = open(temp_name, O_WRONLY);
    int fd_read = open(temp_name, O_RDONLY);
    //unlink(temp_name);


    if (fork() == 0) {
        //Writer
        char input;
        while (read(STDIN_FILENO, &input, 1) != 0) {
            special_write(fd_read, fd_write, &input);
        }
        close(fd_read);
        close(fd_write);
        return 0;
    }
    //Reader
    char output;

    while (1) {
        if (special_read(fd_read, fd_write, &output) == -1) {
            close(fd_read);
            close(fd_write);
            return 0;
        }
        write(STDOUT_FILENO, &output, 1);
        
    }
}