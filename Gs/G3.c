#include <stdio.h>

int
main(int argc, char **argv) {
    //Open main file and create temporary file
    FILE *fp = fopen(argv[1], "r");
    FILE *temp_fp = tmpfile();
    
    //Write nums to temporary file from main file
    int num;
    while (fscanf(fp, "%d", &num) == 1) {
        fwrite(&num, sizeof num, 1, temp_fp);
    }


    //Reopen main file
    fclose(fp);
    fp = fopen(argv[1], "w");

    //Calculate how many numbers there were
    long amount_of_nums = ftell(temp_fp) / sizeof num;
    
    //Set pos to begin temporary file
    fseek(temp_fp, 0, SEEK_SET);
    //Write to main file from end of temporary file (reversed)
    for (int i = 0; i < amount_of_nums; ++i) {
        fseek(temp_fp, (-1) * sizeof num * (i + 1), SEEK_END);
        fread(&num, sizeof num, 1, temp_fp);
        fprintf(fp, "%d ", num);
    }

    //Remove temporary file and close main file
    fclose(temp_fp);
    fclose(fp);
    return 0;
}