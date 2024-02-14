#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

void
MaxPrint(char *arr){
    char buf[82];
    buf[81] = 0;
    char *ptr_buf = &buf[0];
    int max_size = strlen(arr);
    int flag = 1;
    while (flag) {
        if (!*arr) {
            flag = 0;
        } else {
            char *cycle = arr;
            int cur_len = strlen(cycle);
            if (cur_len > max_size) {
                ptr_buf = &buf[0];
                while (*cycle) {
                    *ptr_buf++ = *cycle++;
                }
                max_size = cur_len;
                *ptr_buf = 0;
                ptr_buf++;
            } else if (cur_len == max_size){
                while (*cycle) {
                    *ptr_buf++ = *cycle++;
                }
                *ptr_buf = 0;
                ptr_buf++;
            }
            arr += strlen(arr) + 1;
        }
    }
    ptr_buf = &buf[0];
    while (strlen(ptr_buf)) {
        printf("%s\n", ptr_buf);
        ptr_buf += strlen(ptr_buf) + 1;
    }
}

int
main (void) {
    static char arr[82];

    char *ptr = fgets(arr, sizeof(arr), stdin);
    int flag = 0;
    char *writer = &arr[0];
    while (*ptr) {
        if (isalpha(*ptr)) {
            flag = 1;
            *writer++ = *ptr;
        } else if (flag) {
            *writer++ = 0;
            flag = 0;
        }
        ptr++;
    }
    *writer++ = 0;
    MaxPrint(&arr[0]);
    return 0;
}