#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Huge_Num {
    size_t size;
    const char *body;
    int was_find;
};


struct Huge_Num
find_num (const char *s) {
    struct Huge_Num num = {0, NULL, 0};
    size_t cnt = 0;

    while (*s) {
        if (isdigit(*s)) {
            num.was_find = 1;
            num.body = s;
            while (isdigit(*s)) {
                cnt ++;
                s++;
            }
            
            //Clear usless zeros
            const char *fix = num.body;
            while (*fix == '0') {
                cnt--;
                fix++;
            }
            if (isdigit(*fix)) { 
                num.body = fix;
                num.size = cnt;
            } else { 
                //Num is zero
                num.body = fix - 1;
                num.size = 1;
            }
            return num;
        }
        s++;
    }
    return num;

}

struct Huge_Num
cmp_nums (struct Huge_Num num1, struct Huge_Num num2) {
    const char *ptr1 = num1.body;
    const char *ptr2 = num2.body;

    if (num1.size > num2.size) {
        return num1;
    } else if (num1.size != num2.size) {
        return num2;
    } 

    while (*ptr1) {
        if (*ptr1 > *ptr2) {
            return num1;
        }
        if (*ptr1 < *ptr2) {
            return num2;
        }
        ptr1++; 
        ptr2++;
    }
    return num1;
}


int
main (void) {
    char str1[82];
    char *s1; 
    s1 = fgets(str1, sizeof(str1), stdin);
    
    struct Huge_Num ans;
    struct Huge_Num cur_num;
    char str2[82];
    char *s2 = &str2[0];

    cur_num = find_num(s1);
    ans = cur_num;
    while (cur_num.was_find != 0) {
        ans = cmp_nums(ans, cur_num);
        cur_num = find_num(cur_num.body + cur_num.size);
    }
    if (ans.was_find == 1) {
        strncpy(s2, ans.body, ans.size);
        printf("%s\n", s2);
    }
    return 0;
}