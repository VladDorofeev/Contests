#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {BUFSIZE = 4};

char*
get_unknown_string(void){
    int max_size_str = 1;
    char buf[BUFSIZE];
    char *ptr;
    int cur_len = 0;
    char *my_string = (char*)calloc(1, sizeof(char));
    char *end_flag;
    *my_string = 0;
    while ((end_flag = fgets(buf, sizeof(buf), stdin)) != NULL) {
        cur_len += strlen(buf);
        if (cur_len >= max_size_str) {
            max_size_str *= 5;
            my_string = (char*)realloc(my_string, (max_size_str + 1) * sizeof(char));
        }
        strcat(my_string, buf);
        ptr = strchr(my_string, '\n');
        if (ptr != NULL) {
            return my_string;
        }
    }
    free(my_string);
    return NULL;
}

int
main(void) {
    int max_len = -1;
    int cur_len;
    char *max_string = (char*)malloc(sizeof(char));
    char *cur_string;
    while (1) {
        cur_string = get_unknown_string();
        if (cur_string == NULL) {
            printf("%s", max_string);
            free(max_string);
            break;
        }
        cur_len = strlen(cur_string);
        if (cur_len >= max_len) {
            max_len = cur_len;
            max_string = (char*)realloc(max_string, (max_len + 1)*sizeof(char));
            strcpy(max_string, cur_string);
        } 
        free(cur_string);
    }
    return 0;
}