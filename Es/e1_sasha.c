#include <stdio.h>
#include <float.h>

enum {BUF_SIZE = 10000};

double*
max_num_mas(double my_massive[], int len){
    if (len == 0){
        return NULL;
    }
    double *max_num_m = my_massive;
    for (int i = 0; i < len; ++i) {
        if (my_massive[i] > *max_num_m) {
            max_num_m = &my_massive[i];    
        }
    }
    return max_num_m;
}

void
swape_elem(double *first_elem, double *second_elem){
    double temp = *first_elem;
    *first_elem = *second_elem;
    *second_elem = temp;
}

void
sorting_mas(double my_massive[], int len){
    double *cur_max_elem;
    for (int i = 0; i < len; ++i) {
        cur_max_elem = max_num_mas(&my_massive[i], len - i);
        swape_elem(cur_max_elem, &my_massive[i]);
    }
}

int
main(void){
    int size_mas;
    int i;
    double *max_n_m;
    scanf("%d", &size_mas);
    double massive[BUF_SIZE];
    for (i = 0; i < size_mas; ++i) {
        scanf("%lf", &massive[i]);
    }
    max_n_m = max_num_mas(massive, size_mas);
    if (max_n_m != NULL) {
        if (size_mas >= 18){
            printf("%.1lf\n", *max_num_mas(&massive[4], 14));
        }
        sorting_mas(massive, size_mas);
        for (i = 0; i < size_mas; ++i) {
            printf("%.1lf ", massive[i]);
        }
        printf("\n");
    }
}