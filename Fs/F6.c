#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *
get_string(void) {
    char *buf;
    int length = 1;
    buf = (char *)calloc(1, sizeof buf[0]);
    
    int pos = 0;
    char chr = getc(stdin);
    while (!(feof(stdin)) && (chr != '\n')) {
        buf[pos] = chr;
        if (pos == length - 1) {
            length *= 2;
            buf = realloc(buf, length * sizeof buf[0]);
        }
        pos++;
        chr = getc(stdin);
        if ((ferror(stdin)) || (feof(stdin))) {
            break;
        } 
    }
    if (pos == 0) {
        free(buf);
        return NULL;
    }
    buf[pos] = 0;
    return buf;
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
        }
        free(buf);
        buf = get_string();
    }
    if (ans != NULL) {
        printf("%s\n", ans);
        free(ans);
    }
    return 0;
}