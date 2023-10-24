#include <stdio.h>
#include <limits.h>

int
main (int argc, char **argv) {
    argc--;
    argv++;

    FILE *cur_file;
    FILE *my_file = tmpfile();
    int num;  
    int cnt = 0;
    for (int i = 0; i < argc; ++i) {
        cur_file = fopen(argv[i], "r");
        while (fscanf(cur_file, "%d", &num) == 1) {
            fwrite(&num, 1, sizeof(int), my_file);
            cnt++;
        }
        fclose(cur_file);
    }

    int min;
    long min_pos;

    for (int j = 0; j < cnt; ++j) {
        min = INT_MAX;
        min_pos = 0;
        fseek(my_file, 0, SEEK_SET);

        for (int i = 0; i < cnt; i++) {
            fread(&num, 1, sizeof(int), my_file);
            if (num < min) {
                min = num;
                min_pos = ftell(my_file) - sizeof(int);
            }
        }
        printf("%d ", min);
        fseek(my_file, min_pos, SEEK_SET);
        num = INT_MAX;
        fwrite(&num, 1, sizeof(int), my_file);
    }   
    printf("\n");
    fclose(my_file);
    return 0;
}