#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list_item
{
    char *str;
    struct list_item *next, *prev;
};

void
print_list(struct list_item **list) {
    struct list_item *cur_node = *list;
    struct list_item *first = cur_node;
    while(1) {
        printf("%s\n", cur_node->str);
        cur_node = cur_node->next;
        if (first == cur_node) {
            break;
        }
    }
    printf("============================\n");
}

void 
free_list(struct list_item **list) {
    struct list_item *cur_node = *list;
    struct list_item *temp;
    struct list_item *first = cur_node;
    while(1) {
        temp = cur_node->next;
        free(cur_node->str);
        free(cur_node);
        cur_node = temp;
        if (first == cur_node) {
            break;
        }
    }
}

void
add_node(struct list_item **list, char *str, size_t *sz) {
    (*sz)++;
    struct list_item *new_node = malloc(sizeof *new_node);
    new_node->str = malloc(sizeof(strlen(str) + 1));
    strcpy(new_node->str, str);

    if (*sz == 1) {
        *list = new_node;
        new_node->next = new_node;
        new_node->prev = new_node;
        return;
    }

    struct list_item *last_node = (*list)->prev;

    last_node->next = new_node;
    
    new_node->prev = last_node;
    new_node->next = *list;

    (*list)->prev = new_node;
    return;
}

void 
go_end(struct list_item **list, struct list_item *node) {
    //If node last
    if ((*list)->prev == node) {
        return;
    }

    //if node first
    if (*list == node) {
        *list = (*list)->next;
        return;
    }
    struct list_item *last = (*list)->prev;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    last->next = node;
    (*list)->prev = node;

    node->prev = last;
    node->next = *list;
}

struct list_item *
dup_node(struct list_item *node) {
    struct list_item *new_node = malloc(sizeof *new_node);

    new_node->str = malloc(strlen(node->str) + 1);
    strcpy(new_node->str, node->str);

    new_node->prev = node;
    new_node->next = node->next;

    node->next->prev = new_node;
    node->next = new_node;


    return new_node;
}

void 
process(struct list_item **list, const char *s) {
    if (list == NULL) {
        return;
    }
    if (*list == NULL) {
        return;
    }
    if (s == NULL) {
        return;
    }

    struct list_item *temp;
    struct list_item *cur_node = *list;
    struct list_item *last = (*list)->prev;
    size_t sz_str;
    if (last != cur_node) {
        while(1) {
            if (strstr(cur_node->str, s) != NULL) {
                //Finded s in list item
                sz_str = strlen(cur_node->str);
                for (int i = 0; i < sz_str; i++) {
                    cur_node = dup_node(cur_node);
                }
                cur_node = cur_node->next;
            } else {
                //List item go end
                temp = cur_node->next;
                go_end(list, cur_node);
                cur_node = temp;
            }
            if (cur_node == last) {
                break;
            }
        }
    }
    
    if (strstr(cur_node->str, s) != NULL) {
        //Finded s in list item
        sz_str = strlen(cur_node->str);
        for (int i = 0; i < sz_str; i++) {
            cur_node = dup_node(cur_node);
        }
        cur_node = cur_node->next;
    } else {
        //List item go end
        temp = cur_node->next;
        go_end(list, cur_node);
        cur_node = temp;
    }
}


int
main(int argc, char **argv) {
    // char *arg[] = {"a.out", "abc", "abc"};
    // argc = 3;
    // argv = arg;
    struct list_item *list = NULL;
    if (argc == 1) {
        printf("NO ARGS, GO NULL TESTS\n");
        process(NULL, "NULL");
        process(NULL, NULL);
        process(&list, NULL);
        process(&list, "NULL");
        exit(0);
    }
    size_t sz = 0;
    for (int i = 1; i < argc - 1; i++) {
        add_node(&list, argv[i], &sz);
    }

    print_list(&list);

    process(&list, argv[argc - 1]);
    
    print_list(&list);

    free_list(&list);
    return 0;
}