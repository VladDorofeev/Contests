#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *
get_string(void) {
    char *str= (char *)calloc(1, sizeof str[0]);
    *str = 0;
    int new_size;
    static char buf[1024];
    char *buf_ptr = &buf[0];
    while((buf_ptr = fgets(buf_ptr, sizeof buf, stdin)) != NULL) {
        new_size = strlen(buf_ptr) + strlen(str);
        str = realloc(str, (new_size + 1) * sizeof(char));
        str = strcat(str, buf_ptr);
        if (strchr(buf_ptr, '\n')) {
            break;
        }
    }
    if (strlen(str) == 0) {
        free(str);
        return NULL;
    }
    return str;
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