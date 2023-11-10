#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

enum { BUF_SIZE = 1024 };

char get_c(int fd) {
    static char buf[BUF_SIZE + 1];
    static int pos = 0;
    static int cnt_readed_byte = 0;
    if (pos == cnt_readed_byte) {
        pos = 0;
    }
    if (pos == 0) {
        cnt_readed_byte = read(fd, buf, BUF_SIZE);
        if (cnt_readed_byte == 0) {
            return 0;
        }
    }
    
    return buf[pos++];
}

int 
main (int argc, char **argv) {
    if (argc < 2) {
        return 0;
    }
    char *file_name = argv[1];
    int fd = open(file_name, O_RDONLY);

    if (fd == -1) {
        return 0;
    }

    char temp_name[7] = "XXXXXX";
    int temp_fd = mkstemp(temp_name);
    unlink(temp_name);

    char sym;
    while ((sym = get_c(fd)) != 0) {
        write(temp_fd, &sym, 1);
        if (sym == '\n') {
            break;
        }
    }
    while ((sym = get_c(fd)) != 0) {
        if (sym == '\n') {
            break;
        }
    }
    while ((sym = get_c(fd)) != 0) {
        write(temp_fd, &sym, 1);
    }

    close(fd);
    fd = open(argv[1], O_WRONLY | O_TRUNC);
    lseek(temp_fd, 0, SEEK_SET);
    static char buf[BUF_SIZE + 1];
    size_t cnt_readed_byte = 0;

    while ((cnt_readed_byte = read(temp_fd, buf, BUF_SIZE)) != 0) {
        write(fd, buf, cnt_readed_byte);
    }
    
    close(fd);
    close(temp_fd);
    return 0;
}