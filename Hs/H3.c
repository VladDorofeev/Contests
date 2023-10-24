#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum { MAX_SWITCH_SIZE = 256};

typedef void (*Str_Case) (char *);

typedef struct{
    char *str;
    Str_Case func;
} Pair;

typedef const Pair Str_Switch[MAX_SWITCH_SIZE];

void
run_str_switch(Str_Switch _switch, char *s) {
    int i;
    for (i = 0; strlen(_switch[i].str) != 0; ++i) {
        if (strcmp(_switch[i].str, s) == 0) {
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
    char *end;
    long num = strtol(s, &end, 10);
    if ((*end ==0) && ((int)num == num) && (strlen(s) != 0)) {
        printf("NUMBER\n");
    } else {
        printf("UNKNOWN\n");
    }
}



int
main (int argc, char **argv) {
    argc--;
    argv++;

    
    Str_Switch _switch = {
        {"add", case_add}, 
        {"sub", case_sub},
        {"", case_default},
    };


    for (int i = 0; i < argc; ++i) {
        run_str_switch(_switch, argv[i]);
    }
    
    return 0;
}
