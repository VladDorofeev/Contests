#include <stdio.h>
#include <stdlib.h>

int
main(void){
    int number = 0;
    int i = 0;
    int cur_size = 1;
    int *dynamic_array = (int*)calloc(1, sizeof(int));
    while(scanf("%d", &number) == 1) {
        i++;
        if (cur_size == i) {
            cur_size *= 3;
            dynamic_array = (int*)realloc(dynamic_array, cur_size * sizeof(int));
        }
        dynamic_array[i - 1] = number;
    }
    for (; i > 0; --i) {
        printf("%d ", dynamic_array[i - 1]);
    }
    if (i != 0) {
        printf("\n");
    }
    free(dynamic_array);
    return 0;
}