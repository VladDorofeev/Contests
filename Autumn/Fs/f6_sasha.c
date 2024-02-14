#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
get_unknown_string(int *len, int *end_flag){
    *len = 0;
    int max_size_str = 1;
    char *my_string = (char*)malloc(sizeof(char));
    int sym = getchar();
    while ((char)sym != '\n') {
        if (sym == -1) {
            *end_flag = 1;
            if (len == 0) {
                free(my_string);
                return NULL;
            }
            break;
        }
        my_string[*len] = (char)sym;
        (*len)++;
        if (*len == max_size_str) {
            max_size_str *= 3;
            my_string = (char*)realloc(my_string, max_size_str * sizeof(char));
        }
        sym = getchar();
    }
    my_string[*len] = 0;
    return my_string;  
}

int
main(void){
    int max_len = -1;
    int cur_len;
    char *max_string = (char*)malloc(sizeof(char));
    char *cur_string;
    int end_f = 0;
    while (1) {
        cur_string = get_unknown_string(&cur_len, &end_f);
        if ((cur_len >= max_len) && (cur_string != NULL)) {
            if (cur_len != 0) {
                max_string = (char*)realloc(max_string, (cur_len + 1)*sizeof(char));
            }
            max_len = cur_len;
            strcpy(max_string, cur_string);
        } 
        free(cur_string);
        if ((end_f == 1) && (max_len != -1)) {
            printf("%s\n", max_string);
            break;
        }
    }
    free(max_string);
    return 0;
}