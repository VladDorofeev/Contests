#include <stdio.h>
#include <float.h>

enum {MAX_SIZE = 10000};

double *
find_max(double *b_arr, double *e_arr) {
    double max = -DBL_MAX;
    double *ans = NULL;
    while (b_arr <= e_arr) {
        if (*b_arr > max) {
            max = *b_arr;
            ans = b_arr;
        }
        b_arr++;
    }
    return ans;
}

void 
swap (double *ptr1, double *ptr2) {
    double temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}


void
arr_sort (double *arr, int length) {
    double *arr_end = arr + length - 1; 
    for (int i = 0; i < length; i++) {
        double *max = find_max(arr, arr_end);
        swap(arr, max);
        arr++;
    }
}

int 
main (void) {
    static double arr[MAX_SIZE];
    int len;
    double *ptr = &arr[0];
    scanf("%d", &len);
    for (int i = 0; i < len; ++i) {
        scanf("%lf", ptr++);
    }
    //Find max between 5 18
    ptr = &arr[0];
    if (len >= 18) {
        printf("%.1lf\n", *find_max(ptr + 4, ptr + 17));
    }
    //Sorted array
    arr_sort(ptr, len);
    for (int i = 0; i < len; ++i) {
        printf("%.1lf ", *ptr++);
    }
    printf("\n");
}