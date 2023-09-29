#include <stdio.h>

enum {STR_SIZE = 82};

void
new_strstr(const char * f_str, const char * s_str)
{
    size_t f_str_size = 0, s_str_size = 0;
    const char * f_run_ptr = f_str, * s_run_ptr = s_str, * print_ptr = f_str;
    while(*f_run_ptr != '\0' ||  *s_run_ptr != '\0') {
        if (*f_run_ptr != '\0') {
            ++f_run_ptr;
            ++f_str_size;
        }
        if (*s_run_ptr != '\0') {
            ++s_run_ptr;
            ++s_str_size;
        }
    }
    f_run_ptr = f_str;
    for (int i = 0; i < (int)(f_str_size - s_str_size); ++i) {
        char flag = 0;
        const char * check_ptr = f_run_ptr;
        s_run_ptr = s_str;
        for (int j = 0; j < s_str_size; ++j) {
            if (*check_ptr != *s_run_ptr) {
                flag = 1;
                break;
            }
            ++check_ptr;
            ++s_run_ptr;
        }
        if (!flag) {
            print_ptr = check_ptr;
        }
        ++f_run_ptr;
    }
    printf("%s", print_ptr);
}

int
main(void)
{
    char inp_str[STR_SIZE];
    fgets(inp_str, sizeof inp_str, stdin);
    
    new_strstr(inp_str, "");

    return 0;
}