#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void 
case_add(char* str) {
    printf("ADD\n");
    return;
}

void 
case_sub(char* str) {
    printf("SUB\n");
    return;
}

void 
default_func(char* str) {
    char *end;
    long number = strtol(str, &end, 10);
    if ((*end == 0) && (strlen(str) != 0) && 
        ((int)number == number)) {
        printf("NUMBER\n");
    } else {
        printf("UNKNOWN\n");
    }
}

typedef struct my_switch
{
    char *str_case;
    void (*case_func)(char *);
}my_switch;

void do_switch(char* str) {
    my_switch arr_switch[] = {{"add", case_add}, {"sub", case_sub},
        {"random_string", default_func},{NULL, NULL}};
    for (int i = 0; arr_switch[i].str_case != NULL; ++i) {
        if ((strcmp(str, arr_switch[i].str_case) == 0) || (arr_switch[i].case_func == default_func)) {
            arr_switch[i].case_func(str);
            break;
        }
    }
    return;
}

int
main (int argc, char* argv[]) {
    argv++;
    argc--;
    for (int i = 0; i < argc; ++i) {
        do_switch(argv[i]);
    }
    return 0;
}