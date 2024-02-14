#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { BUF_SIZE = 1024 };

char *
get_string(void) {
    char buf[BUF_SIZE];
    int str_length = 0;
    int str_capacity = 0;
    char *str = NULL;

    while ((fgets(buf, BUF_SIZE, stdin)) != NULL) {
        if (str_length + strlen(buf) >= str_capacity) {
            if (str_capacity == 0) {
                str_capacity = BUF_SIZE * 2;
            }
            str_capacity *= 2;
            str = realloc(str, str_capacity);
            *(str + str_length) = 0;
        }
        strcat(str, buf);
        
        if (strchr(buf, '\n') != NULL) {
            break;
        }
        str_length = strlen(str);
    }
    
    return str;
}


int
main (void) {

    int max_length = 0;
    int cur_length;
    char *ans = NULL;

    char *line = NULL; 
    while ((line = get_string()) != NULL) {
        cur_length = strlen(line);
        if (cur_length >= max_length) {
            free(ans);
            ans = (char *)calloc(cur_length + 1, sizeof line[0]); 
            ans[cur_length - 1] = 0;
            ans = strcpy(ans, line);
            max_length = cur_length;
        }
        free(line);
    }
    if (ans != NULL) {
        printf("%s", ans);
        free(ans);
    }
    return 0;
}