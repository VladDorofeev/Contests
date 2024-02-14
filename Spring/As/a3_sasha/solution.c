#include <stdio.h>
#include <stdlib.h>
#include "my_prog.h"

int
main(void)
{
    read_numbers(20);
    shrink_numbers();
    read_numbers(200);
    print_numbers();
    return 0;
}
