#include <stdio.h>
#include <stdlib.h>

typedef struct my_list
{
    int value;
    struct my_list *next;
    struct my_list *prev;
}my_list;

void
delete_elem(my_list *list, my_list *elem){
    if (!((elem == list) && (list->next == list) && (list->prev == list))) {
        elem->prev->next = elem->next;
        elem->next->prev = elem->prev;
    }
}

void
append_elem(my_list *list, my_list *elem){
    elem->prev = list->prev;
    elem->next = list;
    list->prev->next = elem;
    list->prev = elem;
}

my_list*
change_list(my_list *list) {
    my_list *ptr = list->prev;
    my_list *second_ptr = list;
    my_list *ptr_th;
    while(1){
        if ((ptr->value < 100) && (ptr->value % 2 != 0)) {
            if (ptr == list) {
                if (list->next != list) {
                    ptr_th = list->next;
                    ptr_th->prev = list->prev;
                    delete_elem(list, ptr);
                    free(list);
                    return ptr_th;
                } else {
                    free(list);
                    return NULL;
                }
            } else {
                ptr_th = ptr->prev;
                delete_elem(list, ptr);
                free(ptr);
                ptr = ptr_th;
                continue;
            }
        }
        if (ptr->value > 100) {
            if (ptr != list) {
                delete_elem(list, ptr);
                append_elem(second_ptr, ptr);
            } else {
                if (list->next != list){
                        ptr_th = list->next;
                        if (ptr_th->value <= 100) {
                            if (second_ptr != list) { 
                                delete_elem(ptr_th, ptr);
                                append_elem(second_ptr, ptr);
                            } else {
                                delete_elem(ptr_th, ptr);
                                append_elem(ptr_th, ptr); 
                            }
                        } else {
                            ptr_th = ptr_th->prev;
                        }
                        return ptr_th;
                } else {
                    return list;
                }
            }
            second_ptr = ptr;
        }
        if (ptr == list) {
            return list;
        }
        ptr = ptr->prev;
    }
}

int
main(void){
    my_list *head_elem = NULL;
    my_list *curr_elem = NULL;
    my_list *prev_elem = NULL;
    int number;
    int len_list = 0;
    while(scanf("%d", &number) == 1) {
        len_list++;
        curr_elem = (my_list*)malloc(sizeof(my_list));
        if (head_elem != NULL) {
            prev_elem->next = curr_elem;
            curr_elem->prev = prev_elem;
        }
        if (head_elem == NULL) {
            head_elem = curr_elem;
        }
        curr_elem->value = number;
        curr_elem->next = NULL;
        prev_elem = curr_elem;
        curr_elem = curr_elem->next;
    }
    if (head_elem != NULL) {
        head_elem->prev = prev_elem;
        prev_elem->next = head_elem;
        head_elem = change_list(head_elem);
        if (head_elem != NULL) {
            curr_elem = head_elem->prev;
            while (1) {
                printf("%d ", curr_elem->value);
                if (curr_elem == head_elem) {
                    free(curr_elem);
                    break;
                }
                prev_elem = curr_elem;
                curr_elem = curr_elem->prev;
                free(prev_elem);
            }
        }
    }
    printf("\n");
    return 0;
}
