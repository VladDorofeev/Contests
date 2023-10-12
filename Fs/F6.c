#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { START_LENGTH = 4 };

char *
get_string(void) {
    int cur_length = START_LENGTH;
    char *str = (char *)calloc(START_LENGTH + 1, sizeof str[0]);
    char *start = str;

    while ((fgets(str, cur_length / 2 + 1, stdin)) != NULL) {
        if (strchr(str, '\n') != NULL) {
            break;
        }
        if (strlen(start) == cur_length) {
            cur_length *= 2;
            start = (char *)realloc(start, cur_length + 1);
            str = start + strlen(start);
        } else {
            str += strlen(str);
        }
    }
    
    if (strlen(start) == 0) {
        free(start); 
        return NULL;
    }
    return start;
}


int
main (void) {

    int max_length = 0;
    int cur_length;
    char *ans = NULL;

    char *buf = NULL; 
    buf = get_string();
    while (buf != NULL) {
        cur_length = strlen(buf);
        if (cur_length >= max_length) {
            if (ans != NULL) {
                free(ans);
            }
            ans = (char *)calloc(cur_length + 1, sizeof buf[0]); 
            ans[cur_length - 1] = 0;
            ans = strcpy(ans, buf);
            max_length = cur_length;
        }
        free(buf);
        buf = get_string();
    }
    if (ans != NULL) {
        printf("%s", ans);
        free(ans);
    }
    return 0;
}