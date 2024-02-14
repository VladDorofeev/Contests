#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int 
cmp_lexicographic (const void *arg1, const void *arg2) {

    const char *str1 = *(char **)arg1;
    const char *str2 = *(char **)arg2;
    return -strcmp(str1, str2);
}


int 
cmp_special (const void *arg1, const void *arg2) {
    const char *str1 = *(char **)arg1;
    const char *str2 = *(char **)arg2;
    int len_str1 = strlen(str1);
    int len_str2 = strlen(str2);

    if ((len_str1 % 2 == 0) && (len_str2 % 2 == 0)) {
        return strcmp(str1, str2);
    }

    if ((len_str1 % 2 != 0) && (len_str2 % 2 != 0)) {
        return -strcmp(str1, str2);
    }

    if ((len_str1 % 2 != 0) && (len_str2 % 2 == 0)) {
        return 1;
    }

    if ((len_str1 % 2 == 0) && (len_str2 % 2 != 0)) {
        return -1;
    }
    return 0;
}



int
main (int argc, char **argv) {
    argc--;
    argv++;

    qsort((void *)argv, (size_t)argc, sizeof(char *), cmp_lexicographic);

    for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }

    qsort((void *)argv, (size_t)argc, sizeof(char *), cmp_special);

    for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }

    return 0;
}