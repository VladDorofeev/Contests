#include <stdio.h>
#include <string.h>

enum {STR_LEN = 82, BUFSIZE = 4096};

int
main(int argc, char **argv) {
    char str[STR_LEN] = {};
    char *ptr = NULL;
    fgets(str, sizeof(str), stdin);
    if ((ptr = strchr(str, '\n')) != NULL) {
        *ptr = '\0';
    }

    char str_file[STR_LEN] = {};
    FILE* text_file = fopen(argv[1], "r");
    FILE* tmp_file = tmpfile();
    while(fgets(str_file, sizeof(str_file), text_file) != NULL) {
        int cur_len = strlen(str_file);
        if (strstr(str_file, str) != NULL) {
            fwrite(&str_file[0], sizeof(str_file[0]), cur_len, tmp_file);
        }
    }
    fclose(text_file);

    text_file = fopen(argv[1], "w");
    fseek(tmp_file, 0, SEEK_SET);
    char buf[BUFSIZE] = {};
    int byte_count = 0;
    while((byte_count = fread(&buf[0], sizeof(buf[0]), BUFSIZE, tmp_file)) != 0) {
        fwrite(&buf[0], sizeof(buf[0]), byte_count, text_file);
    }

    fclose(text_file);
    fclose(tmp_file);
    return 0;
}
