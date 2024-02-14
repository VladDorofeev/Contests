#include <stdio.h>
#include <stdlib.h>

enum { DATA_SIZE = 10000 };
enum {GETVAL = 1, SETVAL = 2};
union Argument {
    int value;
    int *ptr;
} ;

typedef struct {
    int id;
    int value;
} Data_Node;

Data_Node data[DATA_SIZE];

int
command (int id, int cmd, union Argument arg) {
    for (int i = 0; i < DATA_SIZE; ++i) {
        if (data[i].id == id) {
            if (cmd == GETVAL) {
                *arg.ptr = data[i].value;
                return 0;
            }
            if (cmd == SETVAL) {
                data[i].value = arg.value;
                return 0;
            }
            return 2;
        }
    }
    return 1;
}
void 
set_data_node(void) {
    int i;
    int id;
    int value;
    scanf("%d", &i);
    scanf("%d", &id);
    scanf("%d", &value);
    data[i].id = id;
    data[i].value = value;
}

int
main (void) {
    set_data_node();
    set_data_node();
    int d;
    scanf("%d", &d);    
    
    int temp;
    union Argument arg;
    arg.ptr = &temp;

    
    if (!command(d, GETVAL, arg)) {
        arg.value = *(arg.ptr) + 1;
        command(d, SETVAL, arg);
        printf("%d\n", arg.value);
    } else {
        printf("NONE\n");
    }

    return 0;
}