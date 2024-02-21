#include "vector.h"
#include <stdio.h>

int
max(Vector v, int *m)
{
    int ret = 1;
    VectorIterator it = begin(v);
    VectorIterator e = end(v);
    while (!same(it, e)) {
        if (ret) {
            ret = 0;
            *m = get_num(it);
        } else if (*m < get_num(it)) {
            *m = get_num(it);
        }
        next(it);
    }
    remove_iter(it);
    remove_iter(e);
    return ret;
}

int
main(void)
{
    Vector v = init_vector();
    int n;
    while (scanf("%d", &n) == 1) {
        push_back(v, n);
    }
    Vector w = init_vector_limited();
    {
        VectorIterator i = begin(v);
        VectorIterator e = end(v);
        while (!same(i, e)) {
            push_back(w, get_num(i));
            next(i);
        }
        remove_iter(i);
        remove_iter(e);
    }

    int m1;
    if (max(v, &m1) == 0) {
        printf("%d\n", m1);
    } else {
        printf("empty\n");
    }

    int m2;
    if (max(w, &m2) == 0) {
        printf("%d\n", m2);
    } else {
        printf("empty\n");
    }

    remove_vector(v);
    remove_vector(w);
}
