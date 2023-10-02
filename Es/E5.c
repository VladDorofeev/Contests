#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

void
BeginPrint(char *arr){
    int flag = 1;
    while (flag) {
        if (!*arr) {
            flag = 0;
        } else {
            char *cycle = arr;
            if (strstr(cycle, "begin") != NULL) {
                printf("%s\n", cycle);
            }
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
    BeginPrint(&arr[0]);
    return 0;
}