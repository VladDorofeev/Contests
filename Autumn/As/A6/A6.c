#include <stdio.h>

unsigned
expression1(unsigned n)
{
    /// EXPRESSION-1 HERE
    n = (n & 0b11111111111000001111111111110001) + ((~n) & 0b00000000000111110000000000000000);
    ///
    return n;
}

int
expression2(unsigned n, int i)
{
    return (
        /// EXPRESSION-2 HERE
        (n << (31 - i)) >> 31
        ///
    );
}

int
expression3(short s1, short s2)
{
    return (
        /// EXPRESSION-3 HERE
        (((unsigned int)(unsigned short)(s2)) << 16) | (unsigned short)(s1)
        ///
    ); 
}

int
test_expression1(void)
{
    typedef struct {
        unsigned dat, ans;
    } Test;
    static Test tests[] = {
        { 0x0u, 0x1F0000u },
        { 0x1u, 0x1F0001u },
        { 0x2u, 0x1F0000u },
        { 0xFu, 0x1F0001u },
        { 0x10u, 0x1F0010u },
        { 0xE0u, 0x1F00E0u },
        { 0x10000u, 0x1E0000u },
        { 0x20000u, 0x1D0000u },
        { 0xD0000u, 0x120000u },
        { 0x1A0000u, 0x50000u },
        { 0x3B0000u, 0x240000u },
        { 0xFFFFFFu, 0xE0FFF1u }
    };

    for (int i = 0; i < sizeof tests / sizeof *tests; ++i) {
        unsigned r = expression1(tests[i].dat);
        if (r != tests[i].ans) {
            printf("%d: WRONG EXPRESSION1 for n = %u: expected %u, gotten %u\n",
                    i, tests[i].dat, tests[i].ans, r);
            return 1;
        }
    }
    return 0;
}

int
test_expression2(void)
{
    typedef struct {
        unsigned dat_n;
        int dat_i;
        int ans;
    } Test;
    static Test tests[] = {
        { 0, 0, 0 },
        { 1, 0, 1 },
        { 1, 1, 0 },
        { 2, 0, 0 },
        { 2, 1, 1 },
        { 2, 2, 0 },
        { 0xABC, 10, 0 },
        { 0xABC, 31, 0 },
        { ~0u, 31, 1 },
        { ~0u, 30, 1 },
        { ~0u, 0, 1 },
        { ~1u, 31, 1 },
        { ~1u, 30, 1 },
        { ~1u, 0, 0 }
    };

    for (int i = 0; i < sizeof tests / sizeof *tests; ++i) {
        int r = expression2(tests[i].dat_n, tests[i].dat_i);
        if (r != tests[i].ans) {
            printf("%d: WRONG EXPRESSION2 for n = %u, i = %d: "
                    "expected %d, gotten %d\n",
                    i, tests[i].dat_n, tests[i].dat_i, tests[i].ans, r);
            return 1;
        }
    }
    return 0;
}

int
test_expression3(void)
{
    typedef struct {
        short dat_s1, dat_s2;
        int ans;
    } Test;
    static Test tests[] = {
        { 0, 0, 0 },
        { 1, 0, 1 },
        { 0, 1, 0x10000u },
        { 1, 1, 0x10001u },
        { 0xFFFFu, 0xFFFFu, 0xFFFFFFFFu },
        { 0xA000u, 0xA000u, 0xA000A000u }
    };

    for (int i = 0; i < sizeof tests / sizeof *tests; ++i) {
        int r = expression3(tests[i].dat_s1, tests[i].dat_s2);
        if (r != tests[i].ans) {
            printf("%d: WRONG EXPRESSION3 for n = %d, i = %d: "
                    "expected %d, gotten %d\n",
                    i, tests[i].dat_s1, tests[i].dat_s2, tests[i].ans, r);
            return 1;
        }
    }
    return 0;
}

int
main(void)
{
    int ret = 0;
    ret = test_expression1() || ret;
    ret = test_expression2() || ret;
    ret = test_expression3() || ret;

    if (ret == 0) {
        printf("OK\n");
    }
    return 0;
}
