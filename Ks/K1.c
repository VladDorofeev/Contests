#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int 
main (int argc, char **argv) {
    argc--;
    argv++;
    if (argc == 0) {
        return 0;
    }
    int fd = open(*argv, O_RDWR);

    off_t file_length = lseek(fd, 0, SEEK_END);
    char first_byte;
    if (file_length > 1) {
        lseek(fd, 0, SEEK_SET);
        read(fd, &first_byte, 1);
        write(fd, &first_byte, 1);
    } 
    close(fd);
    return 0;
}