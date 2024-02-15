#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


int
main(int argc, char **argv) {
    char **filename = argv + 1;
    int fd = open(*filename, O_RDWR);
    off_t len = lseek(fd, 0 , SEEK_END);
    if (len != 0) {
        char num;
        lseek(fd, 0, SEEK_SET);
        read(fd, &num, 1);
        num = (num ^ 0x0A);
        lseek(fd, 0, SEEK_SET);
        write(fd, &num, 1);
    }
    close(fd);
    return 0;
}