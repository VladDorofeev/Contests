#include <stdio.h>
#include <stdlib.h>

enum { START_BUFF_SIZE = 2 };

int
main (void) {
    int cur_buf = START_BUFF_SIZE;
    int *arr;
    arr = (int *)calloc(cur_buf, sizeof &arr);
    int cnt = 0;
    while (scanf("%d", &arr[cnt]) == 1) {
        if (cnt == cur_buf - 1) {
            cur_buf *= 2;
            arr = (int *)realloc(arr, sizeof &arr * cur_buf);
        }
        cnt++;
    }
    cnt--;
    for (; cnt >= 0; --cnt) {
        printf("%d ", arr[cnt]);
    }
    printf("\n");
    free(arr);
    return 0;
}