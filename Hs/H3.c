#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef void (*str_case) (char *);
//typedef str_case (*str_switch) (char *, str_case);
typedef std::vector<std::pair(char *, str_case)> str_switch;
void
run_str_switch(str_switch _switch, char *s) {
    _switch(s)(s);
}

void
case_add (char *s) {
    printf("ADD\n");
}

void
case_sub (char *s) {
    printf("SUB\n");
}

void
case_default (char *s) {
    long num;

    if ((num = strtol(s, NULL, 10)) != 0) {
        printf("NUMBER\n");
    } else {
        printf("UNKNOWN\n");
    }
}


int
main (int argc, char **argv) {
    argc--;
    argv++;

    str_switch _switch = {
        {"ADD", case_add}, 
        {"SUB", case_sub},
    };


    for (int i = 0; i < argc; ++i) {
        run_str_switch(_switch, argv[i]);
    }
    return 0;
}
