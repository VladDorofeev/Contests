#include <stdio.h>
#include <stdlib.h>
int
main(int argc, char **argv) {
    //Binary file
    FILE *fp = fopen(argv[1], "r+");

    char *readed_byte = malloc(2);
    int cnt_readed_byte = fread(readed_byte, 1, 2, fp);
    if (cnt_readed_byte == 2) {
        //Copy first byte to second byte
        fseek(fp, 1, SEEK_SET);
        fwrite(readed_byte, 1, 1, fp);
    }
    free(readed_byte);
    fclose(fp);
    return 0;
}