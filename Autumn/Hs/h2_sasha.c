#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
comp (const char **first_str, const char **second_str) {
    return strcmp(*first_str, *second_str);
}

void*
my_bsearch (void *key, void *base,
        size_t num, size_t size,
        int (*compare)(const void*, const void*)) 
{
    int start = 0;
    int end = num - 1;
    int middle;
    void* cur_str;
    int cmp = 0;
    while (start <= end) {
        middle = (start + end) / 2;
        cur_str = (void*)((char*)base + size*middle);
        cmp = compare(key, cur_str);
        if (cmp < 0) {
            end = middle - 1;
        } else if (cmp > 0) {
            start = middle + 1;
        } else {
            return cur_str;
        }
    }
    return NULL;
}

int
main (int argc, char *argv[]) {
    argv++;
    argc--;
    qsort(argv, argc, sizeof(char*), (int(*) (const void*, const void*)) comp);
    char my_str[257];
    char *ptr;
    fgets(my_str, sizeof(my_str)*sizeof(*my_str), stdin);
    ptr = strchr(my_str, '\n');
    if (ptr != NULL) {
        *ptr = 0;
    }
    char **answer;
    ptr = &my_str[0];
    if ((answer = my_bsearch(&(ptr), argv, argc,
        sizeof(char*), (int(*) (const void*, const void*)) comp)) != NULL) {
        printf("%ld\n", ((char*)answer - (char*)argv)/sizeof(char*) + 1);
    } else {
        printf("%d\n", 0);
    }
    return 0;
}