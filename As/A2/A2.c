#include <stdio.h>
int
intersect(double x1, double y1, double w1,
        double x2, double y2, double w2)
{
    
    return (
        /// WRITE HERE
        ((x2 - x1 - w1) * (x2 - x1 + w2) <= 0) && ((y2 - y1 - w1) * (y2 - y1 + w2) <= 0)
        ///
    );
}


int
main(void)
{
    typedef struct {
        double x, y, w;
    } Square;
    typedef struct {
        Square sq1, sq2;
        int answer;
    } Test;
    static Test tests[] = {
        { {0, 0, 1}, {0, 0, 1}, 1 },
        { {0, 0, 1}, {0, 0, 2}, 1 },
        { {0, 0, 1.3}, {1, 1, 0.7}, 1 },
        { {0, 0, 1.3}, {-1, 1, 1.2}, 1 },
        { {0, 0, 1.3}, {1, -1, 1.1}, 1 },
        { {0, 0, 1.3}, {-1, -1, 1.1}, 1 },
        { {0.5, 0.5, 0.1}, {1, 1, 0.7}, 0 },
        { {0.5, 0.5, 0.1}, {-1, 1, 1.2}, 0 },
        { {0.5, 0.5, 0.1}, {1, -1, 1.1}, 0 },
        { {0.5, 0.5, 0.1}, {-1, -1, 1.1}, 0 },
        { {0, 0, 2}, {0, 0, 1}, 1 },
        { {1, 1, 0.7}, {0, 0, 1.3}, 1 },
        { {-1, 1, 1.2}, {0, 0, 1.3}, 1 },
        { {1, -1, 1.1}, {0, 0, 1.3}, 1 },
        { {-1, -1, 1.1}, {0, 0, 1.3}, 1 },
        { {1, 1, 0.7}, {0.5, 0.5, 0.1}, 0 },
        { {-1, 1, 1.2}, {0.5, 0.5, 0.1}, 0 },
        { {1, -1, 1.1}, {0.5, 0.5, 0.1}, 0 },
        { {-1, -1, 1.1}, {0.5, 0.5, 0.1}, 0 },
    };

    for (size_t i = 0; i < sizeof tests / sizeof *tests; ++i) {
        int got = intersect(tests[i].sq1.x, tests[i].sq1.y, tests[i].sq1.w,
            tests[i].sq2.x, tests[i].sq2.y, tests[i].sq2.w);
        if (got != tests[i].answer) {
            printf("TEST FAILED: x1=%g, y1=%g, w1=%g, "
                "x2=%g, y2=%g, w2=%g: expected %d, gotten %d\n",
                    tests[i].sq1.x, tests[i].sq1.y, tests[i].sq1.w,
                    tests[i].sq2.x, tests[i].sq2.y, tests[i].sq2.w,
                    tests[i].answer, got);
            return 0;
        }
    }

    printf("OK\n");
}
