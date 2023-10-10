#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum { START_BUF_SIZE = 2 };

typedef struct {
    char *str;
    int length;
} Huge_String;


Huge_String
get_string(void) {
    int cnt = 0;
    int cur_len = START_BUF_SIZE;
    char *buf = (char *)calloc(cur_len, sizeof(char));
    int chr = 0;
    while ((chr != '\n') && (!feof(stdin))) {
        chr = getc(stdin);
        buf[cnt] = (char)chr;

        //Get more memory
        if (cnt == START_BUF_SIZE - 1) {
            cur_len *= 2;            
            buf = (char *)realloc(buf, cur_len * sizeof(char));
        }
        cnt++;
    }
    printf("OUT!!\n");
    buf[cnt] = 0;
    Huge_String read_str = {NULL, 0};
    if (cnt == 0) {
        free(buf);
        return read_str;
    }
    read_str.str = (char *)calloc(cnt, sizeof(char));
    read_str.str = strcpy(read_str.str, buf);
    read_str.length = cnt;
    free(buf);
    return read_str;
}


int
main (void) {
    int max_length = 0;
    
    Huge_String buf;
    char *ans;
    while ((buf = get_string()).str != NULL) {
        printf("READED\n");
        if (buf.length > max_length) {
            ans = (char *)calloc(buf.length, sizeof buf.str[0]);
            ans = strcpy(ans, buf.str);
            max_length = buf.length;
        }
        if (buf.length > 0) {
            free(buf.str);
        }
    }
    if (max_length > 0) {
        printf("%s\n", ans);    
        free(ans);
    }
    return 0;
}