#include <stdio.h>
#include <string.h>
#include <ctype.h>

void
write_words(char *my_massive[]){
    char str_f[82] = {};
    int i;
    int max_len = 0;
    int cur_len = 0;
    char *ptr = *my_massive;
    char **ptr_second = my_massive;
    while (*my_massive != NULL) {
        cur_len = strlen(*my_massive);
        if (cur_len > max_len) {
            max_len = cur_len;
        }
        my_massive++;
    }
    my_massive = ptr_second;
    while (*my_massive != NULL){
        if (strlen(*my_massive) == max_len) {
            for (i = 0; i < max_len; ++i) {
                str_f[i] = *ptr;
                ptr++;
            }
            str_f[max_len] = 0;
            printf("%s\n", str_f);
        }
        my_massive++;
        ptr = *my_massive;
    }
}

int
main(void)
{
    char str[82];
    char *ptr;
    int i;
    int len;
    int cnt = 0;
    int start_str = 0;
    fgets(str, sizeof(str) , stdin);
    ptr = strrchr(str, '\n');
    if (ptr != NULL) {
        *ptr = 0;
    }
    char *ptr_s[41];
    char **massive = ptr_s;
    len = strlen(str);
    for (i = 0; i < len; ++i) {
        if (!isalpha(str[i])) {
            str[i] = 0;
            start_str = 0;
        } else {
            if (start_str == 0) {
                *massive = &str[i];
                massive++;
                cnt++;
            }
            start_str = 1;
        }
    }
    *massive = NULL;
    massive -= cnt;
    write_words(massive);
    return 0;
}