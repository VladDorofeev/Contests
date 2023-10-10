#include <stdio.h>
#include <stdlib.h>

struct List_Element {
    struct List_Element *next;
    struct List_Element *prev;
    int item;
};

typedef struct {
    struct List_Element *first;
} List;


void
delete_element (List *list, struct List_Element *delete) {
    struct List_Element *prev = delete->prev;
    struct List_Element *next = delete->next;
    if (list->first == delete) {
        list->first = NULL;
        if (prev != list->first) {
            list->first = next;
        }
    }

    prev->next = next;
    next->prev = prev;
}

void
add_element (List *list, struct List_Element *add) {
    
    struct List_Element *head = list->first;
    struct List_Element *prev = (list->first)->prev;
    
    prev->next = add;
    add->next = head;

    head->prev = add;
    add->prev = prev;

}

void
print_list (List *list) {
    if (list == NULL) {
        return;
    }
    struct List_Element *head = list->first;
    if (head == NULL) {
        return;
    }
    printf("%d ", head->item);
    struct List_Element *cur_elem = head->next;
    while (cur_elem != head) {
        printf("%d ", cur_elem->item);
        cur_elem = cur_elem->next;
    }
    printf("\n");
}


void
create_list (List **list) {
    struct List_Element *new_elem = NULL;
    int item;
    while (scanf("%d", &item) == 1) {
        //Get memory for new element
        new_elem = (struct List_Element *)calloc(1, sizeof (*list)->first);
        
        //Initialization new element 
        new_elem->item = item;
        new_elem->next = new_elem;
        new_elem->prev = new_elem;

        //Add to list
        //if we are adding first element 
        if (*list == NULL) {
            *list = (List *)calloc(1, sizeof list);
            (*list)->first = new_elem;
        } else {
            add_element(*list, new_elem);
        }
    }

    //Loop list
    if (new_elem != NULL) {
        new_elem->next = (*list)->first;
        ((*list)->first)->prev = new_elem;
    }
}
struct List_Element *
elem_greater_100(List *list, struct List_Element * cur_elem) {
    struct List_Element *temp;
    temp = cur_elem->next;
    delete_element(list, cur_elem);
    add_element(list, cur_elem);
    return temp;
}
struct List_Element *
elem_less_100_odd(List *list, struct List_Element * cur_elem) {
    struct List_Element *temp;
    delete_element(list, cur_elem);
    temp = cur_elem->next;
    free(cur_elem);
    return temp;
}
void
edit_list (List *list) {
    if (list == NULL) {
        return;
    }
    struct List_Element *first_elem = list->first;
    struct List_Element *cur_elem;

    if (first_elem->item > 100) {
        cur_elem = elem_greater_100(list, first_elem);
    } else if ((first_elem->item < 100) && (((first_elem->item) % 2) != 0)) {
        cur_elem = elem_less_100_odd(list, first_elem);
    } else {
        cur_elem = first_elem->next;
    }

    //TODO: change to for
    while (list->first != cur_elem) {
        if (cur_elem->item > 100){
            cur_elem = elem_greater_100(list, cur_elem);
            continue;
        }
        if ((cur_elem->item < 100) && (((cur_elem->item) % 2) != 0)) {
            cur_elem = elem_less_100_odd(list, cur_elem);            
            continue;
        } else {
            cur_elem = cur_elem->next;
            continue;
        }
    }

}

int
main (void) {

    List *list = NULL;
    
    
    create_list(&list);
    printf("before:");
    print_list(list);
    
    edit_list(list);
    printf("after:");
    print_list(list);

    return 0;
}