#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

enum { MAX_SWITCH_SIZE = 256, MAX_STRING_SIZE = 82 };

typedef void (*StrCase) (const char *);

typedef struct
{
    char const str[MAX_STRING_SIZE];
    const StrCase func;
} Pair;

typedef const Pair StrSwitch[MAX_SWITCH_SIZE];

void
run_str_switch(const StrSwitch *_switch, char *s) {
    size_t i;
    for (i = 0; strlen((*_switch)[i].str) != 0; ++i) {
        if (strcmp((*_switch)[i].str, s) == 0) {
            //Found s in our cases
            if ((*_switch)[i].func != NULL) {
                (*_switch)[i].func(s);
            }
            return;
        }
    }
    //Default
    (*_switch)[i].func(s);
}

void
case_add (const char *s) {
    printf("ADD\n");
}

void
case_sub (const char *s) {    
    printf("SUB\n");
}

void
case_default (const char *s) {
    char *end;
    long num = strtol(s, &end, 10);
    if ((*end == 0) && (num <= INT_MAX) && (strlen(s) != 0)) {
        printf("NUMBER\n");
    } else {
        printf("UNKNOWN\n");
    }
}



int
main (int argc, char **argv) {
    //argc = 3;
    //argv[1] = "add";
    //argv[2] = "sub";

    argc--;
    argv++;

    const StrSwitch _switch = {
        {"add", case_add}, 
        {"sub", case_sub},
        {"", case_default},
    };


    for (int i = 0; i < argc; ++i) {
        run_str_switch(&_switch, argv[i]);
    }
    
    return 0;
}
