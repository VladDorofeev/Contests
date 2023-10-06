#include <stdio.h>
#include <string.h>
#include <ctype.h>


void
split_string_by_not_latin (char *str) {
    int was_latin = 0;
    char *writer_from_begin = str;
    while (*str) {
        if (isalpha(*str)) {
            was_latin = 1;
            *writer_from_begin++ = *str;
        } else if (was_latin) {
            *writer_from_begin++ = 0;
            was_latin = 0;
        }
        str++;
    }
    *writer_from_begin++ = 0;
}

char *
get_first_three_sym (const char *str) {
    static char buf[4] = {0, 0, 0, 0};
    char *end_buf = &buf[3];
    char *ptr_buf = &buf[0];
    while ((*str) && (ptr_buf != end_buf)) {
        *ptr_buf++ = *str++;
    }
    return &buf[0];
}


void
print_first_three_symbols(char *arr){
    while (1) {
        if (*arr == 0) {
            return;
        }

        char *substring = arr;

        char *first_three_sym = get_first_three_sym(substring);
        if (strlen(first_three_sym) == 3) {
            printf("%s\n", first_three_sym);
        }
        arr += strlen(arr) + 1;
    }
}

int
main (void) {
    static char arr[82];

    char *ptr = fgets(arr, sizeof(arr), stdin);
    split_string_by_not_latin(ptr);
    print_first_three_symbols(&arr[0]);
    return 0;
}