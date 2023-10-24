#include <stdio.h>


int
main (void) {
    int chr;
    long long sum = 0;
    while ((chr = fgetc(stdin)) != EOF) {
        if ((chr >= 'a') && (chr <= 'f')) {
            sum += 10 + (chr - 'a');
        } else if ((chr >= 'A') && (chr <= 'F')) {
            sum += 10 + (chr - 'A');
        } else if ((chr >= '0') && (chr <= '9')) {
            sum += (chr - '0');
        }
    }
    printf("%lld\n", sum);
    return 0;
}