#include <stdio.h>
#include <string.h>

enum {BUFSIZE = 257};

int
main(int argc, char *argv[]){
    FILE* my_file = fopen(argv[1], "r");
    FILE* my_file_s = tmpfile();
    char str[BUFSIZE];
    while (fgets(str, BUFSIZE, my_file) != NULL) {
        if (strstr(str, argv[2]) != NULL) {
            fprintf(my_file_s, "%s", str);
        }
    }
    fclose(my_file);
    fseek(my_file_s, 0, SEEK_SET);
    my_file = fopen(argv[1], "w");
    while (1) {
        fread(str, 1, sizeof(char), my_file_s);
        if (feof(my_file_s)) {
            break;
        }
        fprintf(my_file, "%c", str[0]);
    }
    fclose(my_file);
    fclose(my_file_s);
    return 0;
}