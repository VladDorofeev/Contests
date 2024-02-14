#include <stdio.h>

enum {MAX_SIZE = 10000};

int
main (void) {
    //DECLARE
    static double arr1[MAX_SIZE];
    static double arr2[MAX_SIZE];
    double *ptr_1 = &arr1[0];
    double *ptr_2 = &arr2[0];

    double *f_neg = NULL;
    double *l_pos = NULL;


    int size1;

    scanf("%d", &size1);
    for (int i = 0; i < size1; ++i) {
        scanf("%lf", ptr_1);
        if ((*ptr_1 < 0) && (f_neg == NULL)) {
            f_neg = ptr_1;
        }
        ptr_1++;
    }


    int size2;

    scanf("%d", &size2);
    for (int i = 0; i < size2; ++i) {
        scanf("%lf", ptr_2);
        if (*ptr_2 > 0) {
            l_pos = ptr_2;
        }
        ptr_2++;
    }

    if ((f_neg != NULL) && (l_pos != NULL)) {
        double temp = *f_neg;
        *f_neg = *l_pos;
        *l_pos = temp;
    }

    //PRINT
    ptr_1 = &arr1[0];
    ptr_2 = &arr2[0];

    for (int i = 0; i < size1; ++i) {
        printf("%.1lf ", *ptr_1++);
    }
    printf("\n");
    for (int i = 0; i < size2; ++i) {
        printf("%.1lf ", *ptr_2++);
    }
    printf("\n");
}