#include "my_prog.h"
#include <stdio.h>
#include <stdlib.h>
 
int* nums = 0;
int c = 0, s = 0;
 
void read_numbers(int max)
{
    int n;
    while (s < max && scanf("%d", &n) == 1) {
        if (c == s) {
            if (c == 0) {
                c = 100;
            } else {
                c = 2 * c;
            }
            nums = realloc(nums, c * sizeof * nums);
        }
        nums[s++] = n;
    }
}
 
void shrink_numbers(void)
{
    while (s >= 2 && nums[s - 1] == nums[s - 2]) {
        nums = realloc(nums, (--s) * sizeof * nums);
        c = s;
    }
}
 
void print_numbers(void)
{
    for (int k = 0; k < s; ++k) {
        printf("%d\n", nums[k]);
    }
}