#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
    //Check blanc substring
    if ((argc < 3) || (*argv[2] == 0)) {
        return 0;
    }
    FILE *fp = fopen(argv[1], "r");
    FILE *temp_fp = tmpfile();

    
    char *substring = argv[2];

    int len_substr = strlen(substring);

    char *file_substring = malloc((len_substr + 1) * (sizeof *file_substring));
    int symbol;
    int temp_chr;

    long cur_pos;
    long new_str_pos = 0;

    while ((symbol = fgetc(fp)) != EOF) {

        if (symbol == '\n') {
            //Save position of new line 
            new_str_pos = ftell(fp);
            continue;
        }

        //If first characters are equal, then check another symbols
        if (symbol == substring[0]) {

            //Save position of current symbol
            cur_pos = ftell(fp);

            //Set position to expected substring and copy to memory from file
            fseek(fp, -1, SEEK_CUR);
            file_substring = fgets(file_substring, len_substr + 1, fp);
            

            //Check expected substring with substring
            if (strcmp(file_substring, substring) == 0) {
                //If they equals then copy to temporary file

                //Set file pointer after readed expected substring
                fseek(fp, new_str_pos, SEEK_SET);

                //Copy full line to temporary file
                while (((temp_chr = fgetc(fp)) != EOF) && (temp_chr != '\n')) {
                    fwrite(&temp_chr, 1, 1, temp_fp);
                }

                //Write \n and save pos of new line
                if (temp_chr == '\n') {
                    fwrite(&temp_chr, 1, 1, temp_fp);
                    new_str_pos = ftell(fp);
                }
            } else {
                //They not equal, go next symbol
                fseek(fp, cur_pos, SEEK_SET);
            }
        }
    }


    free(file_substring);
    fclose(fp);
    fp = fopen(argv[1], "w");
    fseek(temp_fp, 0, SEEK_SET);
    while (fread(&temp_chr, 1, 1, temp_fp) == 1) {
        fputc(temp_chr, fp);
    }

    fclose(temp_fp);
    fclose(fp);
    return 0;
}