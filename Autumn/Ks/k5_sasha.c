#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define EOF -1
enum {BUF_SIZE = 4096};


int
my_fgetc(int my_file) {
    static char buffer[BUF_SIZE];
    static int pos_buf;
    static int cnt_sym;
    if (pos_buf == cnt_sym) {
        pos_buf = 0;
        if ((cnt_sym = read(my_file, buffer, BUF_SIZE)) == 0) {
            return EOF;
        }
    }
    pos_buf++;
    return buffer[pos_buf - 1];
}


int
main(int argc, char *argv[]) {
    int file = open(argv[1], O_RDONLY);
    char template[10] = "tmpXXXXXX";
    int tmp_file = mkstemp(template);
    unlink(template);
    char ch_arr[BUF_SIZE];
    int cur_sym;
    int cnt_symbols;
    int pos_arr = 0;
    //write first string
    while((cur_sym = my_fgetc(file)) != EOF) {
        if (pos_arr == BUF_SIZE) {
            write(tmp_file, ch_arr, pos_arr);
            pos_arr = 0;
        }
        ch_arr[pos_arr] = cur_sym;
        pos_arr++;
        if (cur_sym == '\n') {
            break;
        }
    }
    if (pos_arr != 0) {
        write(tmp_file, ch_arr, pos_arr);
    }
    //skip second string
    while((cur_sym = my_fgetc(file)) != EOF) {
        if (cur_sym == '\n') {
            break;
        }
    }
    //write other strings
    pos_arr = 0;
    while((cur_sym = my_fgetc(file)) != EOF) {
        if (pos_arr == BUF_SIZE) {
            write(tmp_file, ch_arr, pos_arr);
            pos_arr = 0;
        }
        ch_arr[pos_arr] = cur_sym;
        pos_arr++;
    }
    if (pos_arr != 0) {
        write(tmp_file, ch_arr, pos_arr);
    }
    close(file);
    //write strings from tmp_file
    file = open(argv[1], O_WRONLY | O_TRUNC);
    lseek(tmp_file, 0, SEEK_SET);
    while((cnt_symbols = read(tmp_file, &ch_arr, BUF_SIZE)) > 0) {
        write(file, ch_arr, cnt_symbols);
    }
    close(tmp_file);
    close(file);
    return 0;
}