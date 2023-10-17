#include <stdio.h>

int
main(int argc, char *argv[]){
    FILE* my_file = fopen(argv[1], "r+");
    if (my_file == NULL) {
        return 0;
    }
    char byte_file = fgetc(my_file);
    fseek(my_file, 0, SEEK_END);

    int pos_in_file = ftell(my_file);
    if (pos_in_file >= 2) {
        fseek(my_file, 1, SEEK_SET);
        fputc(byte_file, my_file);
    }
    fclose(my_file);
    return 0;
}