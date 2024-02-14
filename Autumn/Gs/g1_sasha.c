#include <stdio.h>

int
main(int argc, char *argv[]){
    FILE* my_file = fopen(argv[1], "r");
    if (my_file == NULL) {
        return 0;
    }
    int str_cnt = 1;
    char symbol = fgetc(my_file);
    while(!feof(my_file)) {
        if (str_cnt % 2 == 0) {
            putchar(symbol);
        }
        if (symbol == '\n') {
            str_cnt++;
        }
        symbol = fgetc(my_file);
    }
    fclose(my_file);
    return 0;
}