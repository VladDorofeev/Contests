#include <stdio.h>
#include <stdlib.h>

enum
{
    BUF_SIZE = 10000,
    GETVAL = 0,
    SETVAL = 1,
};

typedef struct Element {
    int value;
    int id;
} Element;

typedef union Argument {
    int number;
    int *pointer;
} Argument;

static Element data[BUF_SIZE];

int
command(int id, int cmd, Argument arg)
{
    Element *temp = NULL;
    for (int i = 0; i < BUF_SIZE; ++i) {
        if (data[i].id == id) {
            temp = &data[i];
            break;
        }
    }
    if (temp == NULL) {
        return 1;
    }
    if (cmd == SETVAL) {
        temp->value = arg.number;
    } else if (cmd == GETVAL) {
        *arg.pointer = temp->value;
    } else {
        return 2;
    }
    return 0;
}

int
insert_to_buff(int i, int id, int value)
{
    if (i >= BUF_SIZE) {
        return 1;
    }
    data[i].id = id;
    data[i].value = value;
    return 0;
}

int
main(void)
{
    for (int i = 0; i < BUF_SIZE; ++i) {
        data[i].value = 0;
        data[i].id = 0;
    }
    
    int i, id, value, d;

    if (scanf("%d%d%d", &i, &id, &value) != 3) {
        return 1;
    }
    if (insert_to_buff(i, id, value) == 1) {
        return 1;
    }
    if (scanf("%d%d%d", &i, &id, &value) != 3) {
        return 1;
    }
    if (insert_to_buff(i, id, value) == 1) {
        return 1;
    }
    if (scanf("%d", &d) != 1) {
        return 1;
    }

    int loc;
    Argument arg;
    arg.pointer = &loc;

    if (command(d, GETVAL, arg)) {
        printf("NONE\n");
        return 0;
    }
    arg.number = *arg.pointer + 1;
    command(d, SETVAL, arg);
    printf("%d\n", arg.number);
    return 0;
}