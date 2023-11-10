#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int 
main (void) {
    char temp_name[7] = "XXXXXX";
    int temp_fd = mkstemp(temp_name);
    close(temp_fd);
    int fd_write = open(temp_name, O_WRONLY);
    int fd_read = open(temp_name, O_RDONLY);
    unlink(temp_name);

    if (fork() == 0) {
        //Writer
        char input;
        while (read(0, &input, 1) != 0) {
            off_t pos_wr = lseek(fd_write, 0, SEEK_CUR);
            
            //Stop one char before fd_read
            while (lseek(fd_read, 0, SEEK_CUR) - pos_wr == 1) {
                usleep(1000);
            }
            while (pos_wr - lseek(fd_read, 0, SEEK_CUR) == 32) {
                usleep(1000);
            }

            if (pos_wr == 32) {
                lseek(fd_write, 0, SEEK_SET);
            }

            write(fd_write, &input, 1);
        }
        close(fd_write);
        return 0;
    } else {
        //Reader
        char output;
        for (;;) {
            off_t pos_rd = lseek(fd_read, 0, SEEK_CUR);

            while (lseek(fd_write, 0, SEEK_CUR) == pos_rd) {
                if (waitpid(-1, 0, WNOHANG) != 0) {
                    close(fd_read);
                    return 0;
                }
                usleep(500);
            }

            if (pos_rd == 32) {
                lseek(fd_read, 0, SEEK_SET);
                pos_rd = 0;
            }

            read(fd_read, &output, 1);
            write(1, &output, 1);
        }
    }
}