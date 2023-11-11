#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

enum { BUF_SIZE = 1024 };

int
get_c(int fd, char *sym) {
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
    
    *sym = buf[pos++];
    

    return 1;
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
    static char buf[BUF_SIZE + 1];
    size_t buf_pos = 0;

    //Get first line
    while (get_c(fd, &sym) != 0) {
        buf[buf_pos++] = sym;
        if (buf_pos == BUF_SIZE) {
            write(temp_fd, &buf, buf_pos);
            buf_pos = 0;
        }
        if (sym == '\n') {
            break;
        }
    }
    if (buf_pos != 0) {
        write(temp_fd, &buf, buf_pos);
        buf_pos = 0;
    }
    //Skip second line
    while (get_c(fd, &sym) != 0) {
        if (sym == '\n') {
            break;
        }
    }

    //Write down file after second line
    while (get_c(fd, &sym) != 0) {
        buf[buf_pos++] = sym;
        if (buf_pos == BUF_SIZE) {
            write(temp_fd, &buf, buf_pos);
            buf_pos = 0;
        }
    }
    if (buf_pos != 0) {
        write(temp_fd, &buf, buf_pos);
        buf_pos = 0;
    }

    close(fd);
    fd = open(argv[1], O_WRONLY | O_TRUNC);
    lseek(temp_fd, 0, SEEK_SET);



    size_t cnt_readed_byte = 0;
    //Copy temp file to main file
    while ((cnt_readed_byte = read(temp_fd, buf, BUF_SIZE)) != 0) {
        write(fd, buf, cnt_readed_byte);
    }
    
    close(fd);
    close(temp_fd);
    return 0;
}