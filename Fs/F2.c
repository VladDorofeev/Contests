#include <stdio.h>
#include <malloc.h>

int
main (void) {
    int N;
    scanf("%d", &N);
    int *arr;

    arr = malloc(sizeof N * N);
    for (int i = 0; i < N; ++i) {
        scanf("%d", &arr[i]);
    }
    for (int i = N - 1; i >= 0; --i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    free(arr);
    return 0;
}