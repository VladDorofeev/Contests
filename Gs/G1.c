#include <stdio.h>

int
main(int argc, char **argv) {
    FILE *fp = fopen(argv[1], "r");

    int chr;
    int cnt_str = 1;
    while ((chr = getc(fp)) != EOF) {
        if (cnt_str % 2 == 0) {
            printf("%c", chr);
        }
        if (chr == '\n') {
            cnt_str++;
        }
    }
    fclose(fp);
    return 0;
}