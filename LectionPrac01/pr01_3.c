#include <stdio.h>


int
main (void) {
    int a;
    int b;
    unsigned int n;
    int i;
    int j;


    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &n);

    for (j = a; j < b; ++j) {
        if (j == a) {
            printf("%*s ", n, "");
        }
        if (j == (b - 1)) {
            printf("%*d", n, j);
            printf("\n");
            continue;
        }
        printf("%*d ", n, j);
    }
    
    for (i = a; i < b; ++i) {
        for (j = a; j < b; ++j) {
            if (j == a) {
                printf("%*d ", n, i);
            }
            if (j == (b - 1)) {
                printf("%*lld", n, (long long)j * i);
                printf("\n");
                continue;
            }
            printf("%*lld ", n, (long long)j * i);
        }
    }

    return 0;
}