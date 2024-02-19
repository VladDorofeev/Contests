#include <stdio.h>
#include <stdlib.h>

struct functions
{
    int (*__remove)();
    int (*__begin)();
    int (*__end)();
    int (*__push_back)();
    int (*__same)();
    int (*__get)();
    int (*__next)();
};

//Like a runner
#define XABRT(e) ({ \
    if (e) { \
        fprintf(stderr, "Failed function\n"); \
        abort();\
    }\
})

//Run func with same name
#define REMOVE(s) ({ XABRT(((struct functions *)(s))->__remove(s)); })
#define BEGIN(s) ({ void *i; XABRT(((struct functions *)(s))->__begin((s),(&i))); i; })
#define END(s) ({ void *i; XABRT(((struct functions *)(s))->__end((s),(&i))); i; })
#define PUSH_BACK(s,v) ({ XABRT(((struct functions *)(s))->__push_back((s),(v))); })
#define SAME(i,j) ({ void *r; XABRT((((struct functions *)(i))->__same((i),(j),(&r)))); *(int *)r; })
#define GET(i) ({ int r; XABRT(((struct functions *)(i))->__get((i),(&r))); r; })
#define NEXT(i) ({ void *r; XABRT(((struct functions *)(i))->__next((i),(&r))); r; })


//Main struct
typedef struct {
    struct functions tb;
    int *nums;
    int cap;
    int sz;
} Vector;

//Vector iterator
typedef struct {
    struct functions ti;
    int *ptr;
} VectorIterator;

#define NEWVECTORITERATOR(v) ({ \
    VectorIterator *it = malloc(sizeof *it); \
    it->ti = (struct functions){.__remove = VectorIterator__remove, \
                                .__same = VectorIterator__same, \
                                .__get = VectorIterator__get, \
                                .__next = VectorIterator__next \
                                }; \
    it->ptr = (v); \
    it; \
})

int VectorIterator__remove();
int VectorIterator__same();
int VectorIterator__get();
int VectorIterator__next();


int Vector__remove();
int Vector__begin();
int Vector__end();
int Vector__push_back();
int VectorS__push_back();

#define VECTOR(v) Vector v = {.nums = 0, .cap = 0, .sz = 0, .tb = { \
        .__remove = Vector__remove, \
        .__begin = Vector__begin, \
        .__end = Vector__end, \
        .__push_back = Vector__push_back, \
}}

#define VECTORS(v) Vector v = {.nums = 0, .cap = 0, .sz = 0, .tb = { \
        .__remove = Vector__remove, \
        .__begin = Vector__begin, \
        .__end = Vector__end, \
        .__push_back = VectorS__push_back, \
}}

int Vector__remove(Vector *v) {
    free(v->nums);
    return 0;
}

int Vector__begin(Vector *v, void **i) {
    *i = NEWVECTORITERATOR(v->nums);
    return 0;
}

int Vector__end(Vector *v, void **i) {
    if (v->sz == 0) {
        *i = NEWVECTORITERATOR(0); /////////////////////////// IF EMPTY THEN ITER->ptr = NULL 
    } else {
        *i = NEWVECTORITERATOR(v->nums + v->sz); ///////////// IF NOT EMPTY THEN ITER->ptr = after vector (non init int)
    }
    return 0;
}

int Vector__push_back(Vector *v, int n) {
    if (v->cap == v->sz) {
        if (v->cap == 0) {
            v->cap = 100;
        } else {
            v->cap = 2 * v->cap;
        }
        void *t = realloc(v->nums, v->cap * sizeof *v->nums);
        if (!t) {
            return 1;
        }
        v->nums = t;
    }
    v->nums[v->sz++] = n;
    return 0;
}

int VectorS__push_back(Vector *v, int n) {
    if (v->cap == v->sz) {
        if (v->cap == 0) {
            v->cap = 100;
        } else {
            return 2;
        }
        void *t = realloc(v->nums, v->cap * sizeof *v->nums);
        if (!t) {
            return 1;
        }
        v->nums = t;
    }
    v->nums[v->sz++] = n;
    return 0;
}

int VectorIterator__remove(VectorIterator *i) {
    free(i);
    return 0;
}

int VectorIterator__same(VectorIterator *i, VectorIterator *j, int *r) {
    *r = (i->ptr == j->ptr);
    return 0;
}

int VectorIterator__get(VectorIterator *i, int *v) {
    *v = *i->ptr;
    return 0;
}

int VectorIterator__next(VectorIterator *i) {
    ++i->ptr;
    return 0;
}

int
max(void *s, int *m)
{
    int ret = 1;
    void *it = BEGIN(s);
    void *e = END(s);
    while (!SAME(it, e)) {
        if (ret) {
            ret = 0;
            *m = GET(it);
        } else if (*m < GET(it)) {
            *m = GET(it);
        }
        NEXT(it);
    }
    REMOVE(it);
    REMOVE(e);
    return ret;
}

int
main(void)
{
    VECTOR(v);
    int n;
    while (scanf("%d", &n) == 1) {
        PUSH_BACK(&v, n);
    }
    VECTORS(w); {
        void *i = BEGIN(&v);
        void *e = END(&v);
        while (!SAME(i, e)) {
            PUSH_BACK(&w, GET(&v));
            i = NEXT(&v);
        }
        REMOVE(i);
        REMOVE(e);
    }

    int m1;
    if (max(&v, &m1) == 0) {
        printf("%d\n", m1);
    } else {
        printf("empty\n");
    }

    int m2;
    if (max(&w, &m2) == 0) {
        printf("%d\n", m2);
    } else {
        printf("empty\n");
    }

    REMOVE(&v);
    REMOVE(&w);
}
