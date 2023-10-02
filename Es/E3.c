#include <stdio.h>
#include <string.h>
#include <ctype.h>


void
SpecialPrint(char *arr){
    char buf[4] = {0, 0, 0, 0};
    char *end_buf = &buf[3];
    char *ptr_buf = &buf[0];
    int flag = 1;
    while (flag) {
        if (!*arr) {
            flag = 0;
        } else {
            char *cycle = arr;
            while ((*cycle) && (ptr_buf != end_buf)) {
                *ptr_buf++ = *cycle++;
            }
            if (ptr_buf == end_buf) {
                printf("%s\n", buf);
            }
            ptr_buf = &buf[0];
            arr += strlen(arr) + 1;
        }
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
    SpecialPrint(&arr[0]);
    return 0;
}