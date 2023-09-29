#include <stdio.h>
#include <string.h>

const char *
my_strstr (const char *s1, const char *s2) {
    char *ptr_str = strchr(s1, s1[0]);
    char *ptr_find = strchr(s2, s2[0]);
    if (*ptr_find == 0){
        return ptr_str;
    }
    while (*ptr_str != 0) {
        if (*ptr_str == *ptr_find) {
            int clear = 1;
            char *ptr_cycle = ptr_str;
            while (*ptr_find != 0) {
                if (*ptr_find++ != *ptr_cycle++) {
                    clear = 0;
                }
            }
            if (clear) {
                printf("s = %s\n", ptr_cycle);
                return ptr_cycle - strlen(s2);
            }
            ptr_find = strchr(s2, s2[0]);
        }
        ptr_str++;
    }
    return NULL;
}
int
main (void) {
    char str1[82];
    char str2[] = "end";
    char *s1; 
    s1 = fgets(str1, sizeof(str1), stdin);
    char *s = strchr(s1, '\n');
    if (s != NULL) {
        *s = '\0';
    }
    const char *cur_str = my_strstr(s1, str2);
    const char *answer;
    if ((cur_str == NULL) || (str2 == "")){
        printf("%s\n", str1);
        return 0;
    }
    while (1){
        /*if (cur_str == NULL) {
            printf("%s\n", str1);
            return 0;
        }
        cur_str = my_strstr(cur_str, str2);
        if (!strcmp(cur_str, answer)) {
            printf("%s\n", cur_str + strlen(str2));
            return 0;
        } else {
            answer = cur_str;
        }*/
        if (my_strstr(cur_str, str2) == NULL){
            printf("%s\n", cur_str);
            return 0;
        }
        cur_str = my_strstr(cur_str, str2) + strlen(str2);
    }
}