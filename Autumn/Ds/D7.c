#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *
numfirst_symend (const char *s1) {
    char str[82];
    char *ptr_s = &str[0] + strlen(s1) - 1;
    char *ptr_n = &str[0];
    char *ptr = strchr(s1, s1[0]);
    while ((*ptr != 0) && (*ptr != '\n')) {
        if (isdigit(*ptr)) {
            *ptr_n++ = *ptr;
        } else {
            *ptr_s-- = *ptr;
        }
        ptr++;
    }
    ptr_n = &str[0];
    ptr_s++;
    ptr = strchr(s1, s1[0]);
    while (*ptr_n != 0) {
        *ptr++ = *ptr_n++;
    }
    while (*ptr_s != 0) {
        *ptr++ = *ptr_s++;
    }
    *ptr = 0;
    ptr = strchr(s1, s1[0]);
    return ptr;
}

int
main (void) {
    char str1[82];
    char *s1; 
    s1 = fgets(str1, sizeof(str1), stdin);
    char *new_s = numfirst_symend(s1);
    printf("%s\n", new_s);
    return 0;
}