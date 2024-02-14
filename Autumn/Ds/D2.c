#include <stdio.h>
#include <string.h>


char *
mystrcat(char *s1, char *s2) {
    char *s = s1;
    while (*s) {
        s++; 
    }
    char *ss = s2;
    while (*ss){
        *s++ = *ss++;
    }
    *s = '\0';
    return s1;
}


int
main (void) {
    char str1[161];
    char str2[82];
    char *s1; 
    char *s2;
    s1 = fgets(str1, sizeof(str1), stdin);
    s2 = fgets(str2, sizeof(str2), stdin);

    char *s = strchr(s1, '\n');
    if (s != NULL) {
        *s = '\0';
    }
    s = strchr(s2, '\n');
    if (s != NULL) {
        *s = '\0';
    }
    printf("%s\n", mystrcat(str1, str2));
}