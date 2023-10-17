#include <stdio.h>

int
main(int argc, char* argv[]){
    FILE* my_file = fopen(argv[1], "r");
    FILE* my_file_s = tmpfile();
    int cur_number;
    while (fscanf(my_file, "%d ", &cur_number) == 1) {
        fwrite(&cur_number, sizeof(cur_number), 1, my_file_s);
    }
    fclose(my_file);
    my_file = fopen(argv[1], "w");
    long cnt = 0;
    long max_pos;
    fseek(my_file_s, 0, SEEK_END);
    max_pos = ftell(my_file_s);
    if (max_pos >= 0) {
        for (cnt = 4; cnt <= max_pos; cnt += 4 ) {
            fseek(my_file_s, -cnt, SEEK_END);
            fread(&cur_number, sizeof(cur_number), 1, my_file_s);
            fprintf(my_file, "%d ", cur_number);
        }
    }
    fclose(my_file);
    fclose(my_file_s);
    return 0;
}