#include <stdio.h>
#include <string.h>
#include <limits.h>

static void
solution(long long n, char buf[])
{
    /// WRITE HERE
        char sym;
        unsigned pos = 0;
        unsigned cnt = 0;
        char bufrev[32];
        if (n < 0) {
            buf[pos] = '-';
            pos++;
        }
        do {
            sym = (n % 10);
            if (sym < 0 ){
                sym = -1 * sym;
            }
            sym += '0';
            n /= 10;
            bufrev[cnt] = sym;
            cnt++; 
        } while (n != 0);
        for (int i = cnt - 1; i >= 0; i--) {
            buf[pos] = bufrev[i];
            pos++;
        }
    ///
}

enum { BUF_SZ = 1024 };

int
main(void)
{
    typedef struct {
        long long n;
        char *answer;
    } Test;

    char llmax[BUF_SZ];
    sprintf(llmax, "%lld", LLONG_MAX);

    char llmaxm1[BUF_SZ];
    sprintf(llmaxm1, "%lld", LLONG_MAX - 1);

    char llmin[BUF_SZ];
    sprintf(llmin, "%lld", LLONG_MIN);

    Test tests[] = {
        { .n = 0, .answer = "0" },
        { .n = 1, .answer = "1" },
        { .n = 2, .answer = "2" },
        { .n = 10, .answer = "10" },
        { .n = 13, .answer = "13" },
        { .n = -45, .answer = "-45" },
        { .n = LLONG_MAX, .answer = llmax },
        { .n = LLONG_MAX - 1, .answer = llmaxm1 },
        { .n = LLONG_MIN, .answer = llmin },
    };

    for (unsigned i = 0; i < sizeof tests / sizeof *tests; ++i) {
        char buf[BUF_SZ] = {0};
        solution(tests[i].n, &buf[0]);
        
        if (!memchr(&buf[0], '\0', sizeof buf)) {
            printf("TEST %d FAILED: NO NIL BYTE AT THE END\n", i);
            return 0;
        } else if (strcmp(tests[i].answer, buf) != 0) {
            printf("TEST %d FAILED: WRONG OUTPUT: n = %lld, "
                "expected %s, gotten %s\n", i, tests[i].n, tests[i].answer, buf);
            return 0;
        }
    }

    printf("OK\n");
}
