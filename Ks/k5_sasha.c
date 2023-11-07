#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
enum {BUF_SIZE = 500};


int
main(int argc, char *argv[]) {
    int file = open(argv[1], O_RDONLY | O_CREAT, 0666);
    char template[10] = "tmpXXXXXX";
    int tmp_file = mkstemp(template);
    unlink(template);
    char ch_arr[BUF_SIZE + 1];
    char *ptr;
    int cnt_symbols;
    int cnt = 0;
    while((cnt_symbols = read(file, &ch_arr, BUF_SIZE)) > 0) {
        ch_arr[cnt_symbols] = 0;
        ptr = ch_arr;
        if (!cnt) {
            ptr = strchr(ch_arr, '\n');
            if (ptr != NULL) {
                cnt = 1;
                write(tmp_file, ch_arr, (int)(ptr - ch_arr + 1));
            } else {
                write(tmp_file, ch_arr, strlen(ch_arr));
            }
            ptr++;
        }   
        if (cnt == 1) {
            while((ptr = (strchr(ptr, '\n'))) != NULL) {
                cnt = 2;
                ptr++;
                write(tmp_file, ptr, strlen(ptr));
                break;
            }
            if (cnt == 2) {
                continue;
            }
        } else if (cnt == 2) {
            write(tmp_file, ch_arr, strlen(ch_arr));
        }
    }
    close(file);
    file = open(argv[1], O_WRONLY | O_TRUNC, 0666);
    lseek(tmp_file, 0, SEEK_SET);
    while((cnt_symbols = read(tmp_file, &ch_arr, BUF_SIZE)) > 0) {
        write(file, ch_arr, cnt_symbols);
    }
    close(tmp_file);
    close(file);
    return 0;
}