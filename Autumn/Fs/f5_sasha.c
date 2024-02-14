#include <stdio.h>

enum {GETVAL = 1, SETVAL = 2, BUFSIZE = 10000};

union Argument{
    int value_f;
    int *value_s;
};

typedef struct{
    int id;
    int value;
}array_cell;

static array_cell data[BUFSIZE];

int
command(int id, int cmd, union Argument un_perem){
    for (int i = 0; i < BUFSIZE; ++i) {
        if (data[i].id == id) {
            switch (cmd)
            {
            case GETVAL: *un_perem.value_s = data[i].value;
                return 0;
            case SETVAL: data[i].value = un_perem.value_f;
                return 0;
            default:
                return 2;
            }
        }
    }
    return 1;
}

int
main(void){
    int i;
    int id;
    int value;
    int d;
    int func_impl;
    union Argument arg;
    arg.value_s = &value;
    for (int j = 0; j < 3; j++) {
        if (j < 2) {
            scanf("%d %d %d", &i, &id, &value);
            data[i].id = id;
            data[i].value = value;
        } else {
            scanf("%d", &d);
            func_impl = command(d, GETVAL, arg);
            if (func_impl != 0) {
                printf("NONE\n");
                return 0;
            } else {
                arg.value_f = *arg.value_s + 1;
                func_impl = command(d, SETVAL, arg);
                printf("%d\n", arg.value_f);
            }
        }
    }
    return 0;
}