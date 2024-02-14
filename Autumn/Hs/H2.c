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

void *
my_bsearch(void *key, void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    size_t start = 0;
    size_t end = nmemb;
    size_t middle;
    int res_compar;
    void *cur_item;

    while (start < end) {
        middle = (start + end) / 2;

        cur_item = (void *)(((char *)base) + (middle * size));

        res_compar = compar(key, cur_item);

        if (res_compar == 0) {
            return cur_item;
        }
        if (res_compar < 0) {
            end = middle;
            continue;
        }
        if (res_compar > 0) {
            start = middle + 1;
            continue;
        }
    }
    return NULL;
}


enum { SIZE_OF_LINE = 257 };

int
main (int argc, char **argv) {
    argc--;
    argv++;

    qsort((void *)argv, (size_t)argc, sizeof(char *), cmp_lexicographic);

    char key[SIZE_OF_LINE];
    char *p_key = &key[0];
    char *p_slash_n = NULL;

    p_key = fgets(p_key, SIZE_OF_LINE, stdin);

    //Delete '\n'
    p_slash_n = strchr(p_key, '\n');
    
    if (p_slash_n != NULL) {
        *p_slash_n = 0;
    }

    char *find_str = (char *)my_bsearch((void *)(&p_key), (void *)argv, 
                        (size_t)argc, (size_t)(sizeof(char *)), cmp_lexicographic);

    if (find_str != NULL) {
        int pos = abs(((char *)argv - find_str) / sizeof(char *)) + 1;
        printf("%d\n", pos);
    } else {
        printf("0\n");
    }

    return 0;
}