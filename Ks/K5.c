#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

enum { BUF_SIZE = 1024 };

int 
main (int argc, char **argv) {
    argc--;
    argv++;
    if (argc < 1) {
        return 0;
    }
    char *file_name = *argv;
    int fd = open(file_name, O_RDONLY);

    if (fd == -1) {
        return 0;
    }

    char temp_name[7] = "XXXXXX";
    int temp_fd = mkstemp(temp_name);
    unlink(temp_name);

    static char buf[BUF_SIZE + 1];

    int cnt_readed_byte = 0;
    char *pos_end_first_str;
    //Read first str and write it to temp file
    do {
        write(temp_fd, buf, cnt_readed_byte);
        cnt_readed_byte = read(fd, buf, BUF_SIZE);
        *(buf + cnt_readed_byte) = 0;
    } while (!(((pos_end_first_str = strchr(buf, '\n')) != NULL) || (cnt_readed_byte == 0)));

    //All file is one string without \n
    if (cnt_readed_byte == 0) {
        return 0;
    }
    
    //Write end of first str
    write(temp_fd, buf, pos_end_first_str - buf + 1);

    //Find end of second str
    char *pos_end_second_str = strchr(pos_end_first_str + 1, '\n');

    if (pos_end_second_str != NULL) {
        //Write file after second str
        write(temp_fd, pos_end_second_str + 1, (buf + strlen(buf)) - pos_end_second_str - 1);
    } else {
        //Skipping second str
        do {
            cnt_readed_byte = read(fd, buf, BUF_SIZE);
            *(buf + cnt_readed_byte) = 0;
        } while (!(((pos_end_second_str = strchr(buf, '\n')) != NULL) || (cnt_readed_byte == 0)));

        //Write file after second str
        write(temp_fd, pos_end_second_str + 1, (buf + strlen(buf)) - pos_end_second_str -1);
    }

    //Copy file after second str
    cnt_readed_byte = 0;
    do {
        write(temp_fd, buf, cnt_readed_byte);
        cnt_readed_byte = read(fd, buf, BUF_SIZE);
        *(buf + cnt_readed_byte) = 0;
    } while (cnt_readed_byte != 0);

    //Rewrite file without second str (from temp file)
    lseek(temp_fd, 0, SEEK_SET);
    close(fd);
    fd = open(file_name, O_WRONLY | O_TRUNC);
    cnt_readed_byte = 0;
    do {
        write(fd, buf, cnt_readed_byte);
        cnt_readed_byte = read(temp_fd, buf, BUF_SIZE);
        *(buf + cnt_readed_byte) = 0;
    } while (cnt_readed_byte != 0);


    close(fd);
    close(temp_fd);
    return 0;
}