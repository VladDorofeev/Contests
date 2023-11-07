#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int fd_write;
int fd_read;

void 
handler (int sig) {
    char output;
    for (;;) {
        off_t pos_rd = lseek(fd_read, 0, SEEK_CUR);

        while (lseek(fd_write, 0, SEEK_CUR) == pos_rd) {
            close(fd_read);
            exit(0);
        }

        if (pos_rd == 32) {
            lseek(fd_read, 0, SEEK_SET);
        } 

        read(fd_read, &output, 1);
        printf("%c", output);
    }
    
}

int 
main (void) {
    signal(SIGUSR1, handler);

    char temp_name[7] = "XXXXXX";
    mkstemp(temp_name);
    fd_write = open(temp_name, O_WRONLY);
    fd_read = open(temp_name, O_RDONLY);
    unlink(temp_name);

    if (fork() == 0) {
        //Writer
        char input;
        while (read(0, &input, 1) != 0) {
            off_t pos_wr = lseek(fd_write, 0, SEEK_CUR);
            
            //Stop one char before fd_read
            while ((pos_wr + 1) % 32 == lseek(fd_read, 0, SEEK_CUR)) {
                usleep(1000);
            }

            if (pos_wr == 32) {
                lseek(fd_write, 0, SEEK_SET);
            } 
            write(fd_write, &input, 1);
        }
        //Send special symbol
        input = '@';
        write(fd_write, &input, 1);
        close(fd_write);
        return 0;
    } else {
        //Reader
        char output;
        for (;;) {
            off_t pos_rd = lseek(fd_read, 0, SEEK_CUR);

            while (lseek(fd_write, 0, SEEK_CUR) == pos_rd ) {
                usleep(500);
            }

            if (pos_rd == 32) {
                lseek(fd_read, 0, SEEK_SET);
            } 

            read(fd_read, &output, 1);
            if (output == '@') {
                exit(0);
            }
            printf("%c", output);
        }
    }
}