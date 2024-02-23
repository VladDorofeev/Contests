#include <stdio.h>
#include <stdlib.h>

#include "narray.h"

static int *nums = 0;
static int c = 0;
static int s = 0;

void 
add_num(int num) {
    if (c == s) {
        c = (c == 0) ? 100 : 2 * c;
        nums = realloc(nums, c * sizeof *nums);
    }
    nums[s++] = num;
}

int 
get_s(void) {
    return s;
}

int
get_num(int ipos) {
    //We can check range here
    return nums[ipos];
}


void 
delete_last(void) {
    //nums = realloc(nums, (--s) * sizeof *nums);
    //c = s;
    s--;
}

void
print_numbers(void)
{
    for (int k = 0; k < s; ++k) {
        printf("%d\n", nums[k]);
    }
}