#include <stdio.h>
#include <stdlib.h>

enum { START_BUFF_SIZE = 2 };

int
main (void) {
    int cur_buf = START_BUFF_SIZE;
    int *arr = NULL;
    int cnt = 0;
    int num;
    while (scanf("%d", &num) == 1) {
        cnt++;
        if (cnt == cur_buf - 1) {
            cur_buf *= 2;
            arr = (int *)realloc(arr, sizeof arr[0] * cur_buf);
        }
        arr[cnt - 1] = num;
        
    }
    cnt--;
    for (; cnt >= 0; --cnt) {
        printf("%d ", arr[cnt]);
    }
    printf("\n");
    free(arr);
    return 0;
}