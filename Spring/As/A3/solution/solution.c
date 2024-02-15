#include "narray.h"
#include <stdio.h>
void
read_numbers(int max)
{
    int n;
    while (get_s() < max && scanf("%d", &n) == 1) {
        add_num(n);
    }
}

void
shrink_numbers(void)
{
    while (get_s() >= 2 && get_num(get_s() - 1) == get_num(get_s() - 2)) {
        delete_last();
    }
}

int
main(void)
{
    read_numbers(20);
    shrink_numbers();
    read_numbers(200);
    print_numbers();
}


