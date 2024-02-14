#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

char
special_read(int fd, char *byte) {
    static char buf[1025];
    static size_t buf_pos = 0;
    static size_t readed = 0;

    if (buf_pos == readed) {
        buf_pos = 0;
        readed = read(fd, &buf[0], 1024);
        if (readed == 0) {
            return 0;
        }
    }

    *byte = buf[buf_pos++];

    return 1;
}

int 
main (void) {
    char buf;
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        //son to connect
        close(fd[0]);
        int cnt = 1;
        while (special_read(STDIN_FILENO, &buf) == 1) {
            if ((cnt++) % 2 == 0) {
                write(fd[1], &buf, 1);
            }
        }
        close(fd[1]);

        return 0;
    }
    if (fork() == 0) {
        if (fork() == 0) {
            //grandson to connect
            close(fd[1]);
            int cnt = 1;
            while (special_read(fd[0], &buf) == 1) {
                if ((cnt++) % 2 == 0) {
                    write(STDOUT_FILENO, &buf, 1);
                }
            }
            close(fd[0]);
            return 0;
        }
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        return 0;
    }
    
    close(fd[0]);
    close(fd[1]);

    while(wait(NULL) != -1);

    return 0;
}