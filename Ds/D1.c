#include <stdio.h>

int
main (void) {
    char str1[82];
    char *s1; 
    s1 = fgets(str1, sizeof(str1), stdin);
    if (s1 == NULL){
        printf("EMPTY INPUT\n");
    } else {
        printf("%s", s1);
    }
}