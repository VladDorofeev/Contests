#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node
{
    struct Node *prev, *next;
    char *elem;
};
struct List
{
    struct Node *first, *last;
};




void
delete_item(struct List *pl, struct Node *delete) {
    if ((delete == pl->first) && (delete == pl->last)) {
        //Only one element in list
        pl->first = NULL;
        pl->last = NULL;
        free(delete->elem);
        free(delete);
        return;
    }
    if (delete == pl->first) {
        //Deleting first element (more than one element in list)
        pl->first = delete->next;
        delete->next->prev = NULL;

        free(delete->elem);
        free(delete);
        return;
    }
    if (delete == pl->last) {
        //Deleting last element (more than one element in list)
        pl->last = delete->prev;
        delete->prev->next = NULL;

        free(delete->elem);
        free(delete);
        return;
    }


    //Deleting not first, not last element and more than one element in list
    delete->next->prev = delete->prev;
    delete->prev->next = delete->next;
    free(delete->elem);
    free(delete);
}

void 
item_to_end(struct List *pl, struct Node *to_end) {
    struct Node *last = pl->last;

    //If we have one element in list or to_end is last element
    if (to_end == last) {
        return;
    }

    //If to_end - first element
    if (to_end == pl->first) {
        to_end->next->prev = NULL;
        pl->first = to_end->next;
        to_end->next = NULL;
        to_end->prev = last;
        last->next = to_end;
        pl->last = to_end;
        return;
    }

    //If to_end not first and not last element
    to_end->next->prev = to_end->prev;
    to_end->prev->next = to_end->next;
    to_end->next = NULL;
    to_end->prev = last;
    last->next = to_end;
    pl->last = to_end;

}

void 
process(struct List *pl, const char *str) {
    struct Node *last = pl->last;
    struct Node *node = pl->first;
    struct Node *temp = node;
    if (node == NULL) {
        return;
    }
    if (last == NULL) {
        return;
    }

    //Iterations through the list
    for (; node != last; node = temp) {
        temp = node->next;
        if (strcmp(node->elem, str) == 0) {
            delete_item(pl, node);
            continue;
        }
        if (strcmp(node->elem, str) > 0) {
            item_to_end(pl, node);
            continue;
        }
    }
    //Processing last element
    if (strcmp(node->elem, str) == 0) {
        delete_item(pl, node);
        return;
    }
    if (strcmp(node->elem, str) > 0) {
        item_to_end(pl, node);
        return;
    }
}   
void 
add_node (struct List *pl, char *str) {
    struct Node *new_node = malloc(sizeof *new_node);
    new_node->elem = str;

    if (pl->first == NULL) {
        pl->first = new_node;
        pl->last = new_node;
        new_node->next = NULL;
        new_node->prev = NULL;
        return;
    }
    struct Node *last = pl->last;
    last->next = new_node;
    new_node->prev = last;
    new_node->next = NULL;
    pl->last = new_node;
}

void
print_list (struct List *pl) {
    struct Node *node = pl->first;
    for (; node != NULL; node = node->next) {
        printf("%s", node->elem);
    }
}

int
main (void) {
    struct List *pl = malloc(sizeof *pl);
    pl->last = NULL;
    pl->first = NULL;
    char *str;
    for (int i = 0; i < 3; ++i) {
        str = calloc(80, 1);
        str = fgets(str, 80, stdin);
        add_node(pl, str);
    }
    printf("\n");
    print_list(pl);
    printf("\n");


    str = calloc(80, 1);
    str = fgets(str, 80, stdin);
    process(pl, str);


    printf("\n");
    print_list(pl);
    return 0;
}
