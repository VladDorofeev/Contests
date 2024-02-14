#include <stdio.h>
#include <stdlib.h>

int
main(void){
    unsigned N;
    scanf("%u", &N);
    int *dynamic_array = (int*)calloc(N, sizeof(int));
    int i;
    for (i = 0; i < N; ++i) {
        scanf("%d", &dynamic_array[i]);
    }
    for (i = N - 1; i >= 0; --i) {
        printf("%d ", dynamic_array[i]);
    }
    printf("\n");
    free(dynamic_array);
}