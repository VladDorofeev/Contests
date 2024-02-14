#include <stdio.h>
#include <string.h>

enum { STRING_SIZE = 257 };

int
main(int argc, char **argv) {
    FILE *fp = fopen(argv[1], "r");
    FILE *temp_fp = tmpfile();
    if (argc < 3) {
        return 0;
    }
    char *substring = argv[2];

    char line[STRING_SIZE];
    char *ptr_line = &line[0];

    while ((ptr_line = fgets(ptr_line, STRING_SIZE, fp)) != NULL) {
        if (strstr(ptr_line, substring) != NULL) {
            fwrite(ptr_line, 1, strlen(ptr_line), temp_fp);
        }
    }

    fclose(fp);
    fp = fopen(argv[1], "w");
    fseek(temp_fp, 0, SEEK_SET);
    char chr;
    while (fread(&chr, 1, 1, temp_fp) == 1) {
        fprintf(fp, "%c", chr);
    }

    fclose(temp_fp);
    fclose(fp);
    return 0;
}