#include <stdio.h>
#include <string.h>

int 
endcmp(const char *s1, const char *s2) {
    if (strlen(s1) > strlen(s2)) {
        return 0;
    }
    char *end_s2 = strchr(s2, 0);
    end_s2 -= strlen(s1);
    while (*end_s2 != 0){
        if (*end_s2 != *s1) {
            return 0;
        }
        end_s2++;
        s1++;
    }
    return 1;
}

int
main (void) {
    char str1[82];
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
    if (endcmp(s1, s2)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
}