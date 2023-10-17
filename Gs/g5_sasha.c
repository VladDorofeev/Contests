#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char* argv[]){
    FILE* my_file = fopen(argv[1], "r");
    FILE* my_file_s = tmpfile();
    char symbol = fgetc(my_file);
    long cur_len_str = 0;
    char *sub_str = (char*)malloc(strlen(argv[2]) + 1);
    while (!feof(my_file)) {
        if ((symbol == *(argv[2])) || (*(argv[2]) == 0)) {
            fseek(my_file, -1, SEEK_CUR);
            fgets(sub_str, strlen(argv[2]) + 1, my_file);
            if ((strcmp(sub_str, argv[2]) == 0) || (*(argv[2]) == 0)) {
                fseek(my_file, -strlen(argv[2]) - cur_len_str, SEEK_CUR);
                while (1) {
                    symbol = fgetc(my_file);
                    if (feof(my_file)) {
                        break;
                    }
                    fwrite(&symbol, sizeof(symbol), 1, my_file_s);
                    if (symbol == '\n') {
                        break;
                    }
                }
                if (feof(my_file)) {
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
        symbol = fgetc(my_file);
    }
    free(sub_str);
    fclose(my_file);
    fseek(my_file_s, 0, SEEK_SET);
    my_file = fopen(argv[1], "w");
    while (1) {
        fread(&symbol, sizeof(char), 1, my_file_s);
        if (feof(my_file_s)) {
            break;
        }
        fputc(symbol, my_file);
    }
    fclose(my_file);
    fclose(my_file_s);
    return 0;
}