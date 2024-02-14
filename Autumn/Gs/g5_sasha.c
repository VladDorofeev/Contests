#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char* argv[]){
    FILE* my_file = fopen(argv[1], "r");
    FILE* my_file_s = tmpfile();
    int symbol;
    long cur_len_str = 0;
    char *sub_str = (char*)malloc(strlen(argv[2]) + 1);
    while ((symbol = fgetc(my_file)) != EOF) {
        if ((symbol == *(argv[2])) || (*(argv[2]) == 0)) {
            fseek(my_file, -1, SEEK_CUR);
            fgets(sub_str, strlen(argv[2]) + 1, my_file);
            if ((strcmp(sub_str, argv[2]) == 0) || (*(argv[2]) == 0)) {
                fseek(my_file, -strlen(argv[2]) - cur_len_str, SEEK_CUR);
                while ((symbol = fgetc(my_file)) != EOF) {
                    fwrite(&symbol, sizeof(char), 1, my_file_s);
                    if (symbol == '\n') {
                        break;
                    }
                }
                if (symbol == EOF) {
                    break;
                }
            } else {
                fseek(my_file, -strlen(sub_str) + 1, SEEK_CUR);
            }
        }
        cur_len_str++;
        if (symbol == '\n') {
            cur_len_str = 0;
        }
    }
    free(sub_str);
    fclose(my_file);
    fseek(my_file_s, 0, SEEK_SET);
    my_file = fopen(argv[1], "w");
    while (fread(&symbol, sizeof(char), 1, my_file_s) == 1) {
        fputc(symbol, my_file);
    }
    fclose(my_file);
    fclose(my_file_s);
    return 0;
}