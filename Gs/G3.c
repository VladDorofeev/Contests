#include <stdio.h>

int
main(int argc, char **argv) {

    //Open main file and create binary temporary file
    FILE *fp = fopen(argv[1], "r");
    FILE *temp_fp = fopen("__G3__.bin", "w+");
    
    //Write nums to temporary binary file from main file
    int num;
    int cnt = 0;
    while (fscanf(fp, "%d", &num) == 1) {
        fwrite(&num, sizeof num, 1, temp_fp);
        cnt++;
    }

    //Write to main file from end of temporary file (reversed)
    fclose(fp);
    fclose(temp_fp);

    fp = fopen(argv[1], "w");
    temp_fp = fopen("__G3__.bin", "r");

    for (int i = 0; i < cnt; ++i) {
        fseek(temp_fp, (-1) * sizeof(int) * (i + 1), SEEK_END);
        fread(&num, sizeof num, 1, temp_fp);
        fprintf(fp, "%d ", num);

    }

    //Remove temporary file and close main file
    remove("__G3__.bin");
    fclose(fp);
    return 0;
}