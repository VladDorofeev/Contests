#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { MAX_SWITCH_SIZE = 255 };

typedef void (*str_case) (char *);

typedef struct{
    char *str;
    str_case func;
} pair;

typedef pair str_switch[MAX_SWITCH_SIZE];

void
run_str_switch(str_switch _switch, char *s) {
    int i;
    for (i = 0; strlen(_switch[i].str) != 0; ++i) {
        if (!strcmp(_switch[i].str, s)) {
            //Found s in our cases
            if (_switch[i].func != NULL) {
                _switch[i].func(s);
            }
            return;
        }
    }
    //Default
    _switch[i].func(s);
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
        {"add", case_add}, 
        {"sub", case_sub},
        {"", case_default},
    };


    for (int i = 0; i < argc; ++i) {
        run_str_switch(_switch, argv[i]);
    }
    
    return 0;
}
