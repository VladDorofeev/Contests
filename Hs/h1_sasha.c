#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
compare (const char **first_str, const char **second_str) {
    int answer = strcmp(*first_str, *second_str);
    if (answer > 0) {
        return -1;
    } else if (answer < 0) {
        return 1;
    } else {
        return 0; 
    }
}

int
second_compare (const char **first_str, const char **second_str) {
    int first_len = strlen(*first_str);
    int second_len = strlen(*second_str);
    if ((first_len % 2 == 0) && (second_len % 2 == 0)) {
        return -compare(first_str, second_str);
    } else if ((first_len % 2 == 1) && (second_len % 2 == 1)) {
        return compare(first_str, second_str);
    } else if ((first_len % 2 == 1) && (second_len % 2 == 0)) {
        return 1;
    } else if ((first_len % 2 == 0) && (second_len % 2 == 1)) {
        return -1;
    } else {
        return 0;
    }
}

int
main (int argc, char *argv[]) {
    argv++;
    argc--;
    qsort(argv, argc, sizeof(char*), (int(*) (const void*, const void*)) compare);
    int i;
    for (i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }
    qsort(argv, argc, sizeof(char*), (int(*) (const void*, const void*)) second_compare);
    for (i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }
    return 0;
}