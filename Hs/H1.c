#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int 
cmp_lexicographic (const void *arg1, const void *arg2) {

    const char *str1 = *(char **)arg1;
    const char *str2 = *(char **)arg2;
    return strcmp(str1, str2);
}

int 
cmp_special (const void *arg1, const void *arg2) {
    const char *str1 = *(char **)arg1;
    const char *str2 = *(char **)arg2;
    int len_str1 = strlen(str1);
    int len_str2 = strlen(str2);

    //If both length not odd
    if ((len_str1 % 2 == 0) && (len_str2 % 2 == 0)) {
        return strcmp(str1, str2);
    }

    //If both length odd
    if ((len_str1 % 2 != 0) && (len_str2 % 2 != 0)) {
        return strcmp(str1, str2);
    }

    //Check len1 and len2 
    //Anyone must be not odd, and what len not odd, that str is more
    if (len_str1 % 2 == 0) {
        return INT_MAX;
    }

    if (len_str2 % 2 == 0) {
        return INT_MIN;
    }
}



int
main (int argc, char **argv) {
    argc--;
    argv++;

    //qsort((void *)argv, (size_t)argc, sizeof(char *), cmp_lexicographic);

    //for (int i = 0; i < argc; ++i) {
    //    printf("%s\n", argv[i]);
    //}
    //printf("\n");

    qsort((void *)argv, (size_t)argc, sizeof(char *), cmp_special);

    for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }

    return 0;
}