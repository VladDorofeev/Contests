#include <stdio.h>
#include <stdlib.h>

struct List_Element {
    int item;
    struct List_Element *next;
    struct List_Element *prev;
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
rev_print_list (List *list) {
    if (list == NULL) {
        return;
    }
    struct List_Element *head = list->first;
    if (head == NULL) {
        return;
    }
    struct List_Element *cur_elem = head->prev;
    while (cur_elem != head) {
        printf("%d ", cur_elem->item);
        cur_elem = cur_elem->prev;
    }
    printf("%d ", head->item);
    printf("\n");
}


void
create_list (List **list) {
    struct List_Element *new_elem = NULL;
    int item;
    while (scanf("%d", &item) == 1) {
        //Get memory for new element
        new_elem = (struct List_Element *)malloc(sizeof(struct List_Element));
        
        
        //Initialization new element 
        new_elem->item = item;
        new_elem->next = new_elem;
        new_elem->prev = new_elem;

        //Add to list
        //if we are adding first element 
        if (*list == NULL) {
            *list = (List *)malloc(sizeof list);
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
elem_greater_100(List *list, struct List_Element *cur_elem) {
    struct List_Element *temp;
    temp = cur_elem->next;
    delete_element(list, cur_elem);
    if (list->first == NULL) {
        list->first = temp;
    }
    add_element(list, cur_elem);
    return temp;
}
struct List_Element *
elem_less_100_odd(List *list, struct List_Element *cur_elem) {
    struct List_Element *temp;
    delete_element(list, cur_elem);
    if (list->first == NULL) {
        //Check len list > 1
        if (cur_elem->next == cur_elem) {
            free(cur_elem);
            list->first = NULL;
            return NULL;
        }   
        list->first = cur_elem->next;
    }
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
    struct List_Element *first_greater_100 = NULL;
    int was_greater_100 = 0;
    struct List_Element *cur_elem;

    //Working on first element
    while (1) {
        if (first_elem->item > 100){
            //Check len list > 1
            if (first_elem->next == first_elem) {
                return;
            }

            //Save first greater 100
            if (!was_greater_100) {
                was_greater_100 = 1;
                first_greater_100 = first_elem;
            } else if (first_elem == first_greater_100) {
                //If all nums greater 100
                return;
            }
            first_elem = elem_greater_100(list, first_elem);
            continue;
        }
        if ((first_elem->item < 100) && (((first_elem->item) % 2) != 0)) {
            first_elem = elem_less_100_odd(list, first_elem);
            if (first_elem == NULL) {
                return;
            }
        } else {
            cur_elem = first_elem->next;
            break;
        }
    }

    //Iteration by the list
    while ((list->first != cur_elem) && ((!was_greater_100) || (first_greater_100 != cur_elem))){
        if (cur_elem->item > 100){
            //Save first greater 100
            if (!was_greater_100) {
                was_greater_100 = 1;
                first_greater_100 = cur_elem;
            } else if (cur_elem == first_greater_100) {
                //If last nums greater 100
                return;
            }
            cur_elem = elem_greater_100(list, cur_elem);
            continue;
        }
        if ((cur_elem->item < 100) && (((cur_elem->item) % 2) != 0)) {
            cur_elem = elem_less_100_odd(list, cur_elem);
            if (cur_elem == NULL) {
                return;
            }        
            continue;
        } else {
            cur_elem = cur_elem->next;
            continue;
        }
    }

}


void
free_list (List *list) {
    if (list == NULL) {
        return;
    }
    struct List_Element *head = list->first;
    if (head == NULL) {
        free(list);
        return;
    }

    struct List_Element *cur_elem;
    cur_elem = head->prev;
    struct List_Element *temp;
    while (cur_elem != head) {
        temp = cur_elem;
        cur_elem = cur_elem->prev;
        free(temp);
    }
    free(head);
    free(list);
}

int
main (void) {

    List *list = NULL;
    create_list(&list);
    edit_list(list);
    rev_print_list(list);
    free_list(list);
    return 0;
}