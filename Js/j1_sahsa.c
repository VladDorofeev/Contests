#include <stdio.h>

int
main(void){
    int n;
    scanf("%d", &n);
    switch (n)
    {
    case 0: printf("A\nB\nB\nC\nC\n");
        break;
    case 1: printf("A\nB\nC\nB\nC\n");
        break;
    default:
        printf("UNKNOWN\n");
        break;
    }
    scanf("%d", &n);
    switch (n)
    {
    case 0: printf("1\n2\n2\n");
        break;
    case 1: printf("2\n1\n2\n");
        break;
    case 2: printf("2\n2\n1\n");
        break;
    default:
        printf("UNKNOWN\n");
        break;
    }
    return 0;
}