#include <stdio.h>
#include <string.h>

int
mycmp (const char *s1, const char *s2) {
    char *ptr_1 = strchr(s1, s1[0]);
    char *ptr_2 = strchr(s2, s2[0]);

    while ((*ptr_1 != 0) || (*ptr_2 != 0)) {
        while (*ptr_1 == ' '){
            ptr_1++;
            if (*ptr_1 == 0) {
                return 0;
            }
        }
        
        while (*ptr_2 == ' ') {
            ptr_2++;
            if (*ptr_2 == 0) {
                return 0;
            }
        }
        
        if (*ptr_1++ != *ptr_2++) {
            return 0;
        }
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
    if (mycmp(s1, s2)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
}