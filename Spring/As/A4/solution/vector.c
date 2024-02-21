#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

struct vector_funcs
{
    int (*__remove)(Vector v);
    int (*__begin)(Vector v, VectorIterator *iter);
    int (*__end)(Vector v, VectorIterator *iter);
    int (*__push_back)(Vector v, int n);
};

struct vector_iter_funcs
{
    int (*__remove)(VectorIterator iter);
    int (*__same)(VectorIterator i, VectorIterator j, int *r) ;
    int (*__get)(VectorIterator iter, int *num);
    int (*__next)(VectorIterator iter);
};

struct Vector {
    struct vector_funcs tb;
    int *nums;
    int cap;
    int sz;
} ;

struct VectorIterator{
    struct vector_iter_funcs ti;
    int *ptr;
};

int Vector__remove(Vector v);
int Vector__begin(Vector v, VectorIterator *iter);
int Vector__end(Vector v, VectorIterator *iter);
int Vector__push_back(Vector v, int num);
int VectorS__push_back(Vector v, int num);

int VectorIterator__remove(VectorIterator iter);
int VectorIterator__same(VectorIterator i, VectorIterator j, int *r) ;
int VectorIterator__get(VectorIterator iter, int *num);
int VectorIterator__next(VectorIterator iter);


//Like a default constructor for Vector
Vector
init_vector(void) {
    struct Vector *v = malloc(sizeof *v);
    v->nums = NULL;
    v->cap = 0;
    v->sz = 0;
    v->tb = (struct vector_funcs){
        .__remove = Vector__remove, 
        .__begin = Vector__begin,
        .__end = Vector__end,
        .__push_back = Vector__push_back,
    };
    return v;
}

Vector
init_vector_limited(void) {
    struct Vector *v = malloc(sizeof *v);
    v->nums = NULL;
    v->cap = 0;
    v->sz = 0;
    v->tb = (struct vector_funcs){
        .__remove = Vector__remove, 
        .__begin = Vector__begin,
        .__end = Vector__end,
        .__push_back = VectorS__push_back,
    };
    return v;
}

//Like a default constructor for VectorIterator
VectorIterator
init_vector_iter(int *nums) {
    struct VectorIterator *iter = malloc(sizeof *iter);
    iter->ti = (struct vector_iter_funcs){.__remove = VectorIterator__remove, 
                                .__same = VectorIterator__same, 
                                .__get = VectorIterator__get, 
                                .__next = VectorIterator__next 
                                }; 
    
    iter->ptr = nums;
    return iter;
}


///////////////
//Destructors//
///////////////

int Vector__remove(Vector v) {
    free(v->nums);
    free(v);
    v = NULL;
    return 0;
}
int VectorIterator__remove(VectorIterator iter) {
    free(iter);
    return 0;
}


///////////
//Methods//
///////////

int Vector__push_back(Vector v, int num) {
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
    v->nums[v->sz++] = num;
    return 0;
}

int VectorS__push_back(Vector v, int num) {
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
    v->nums[v->sz++] = num;
    return 0;
}

int Vector__begin(Vector v, VectorIterator *iter) {
    *iter = init_vector_iter(v->nums);
    return 0;
}

int Vector__end(Vector v, VectorIterator *iter) {
    if (v->sz == 0) {
        *iter = init_vector_iter(NULL);
    } else {
        *iter = init_vector_iter(v->nums + v->sz);
    }
    return 0;
}

int VectorIterator__get(VectorIterator iter, int *num) {
    *num = *(iter->ptr);
    return 0;
}

int VectorIterator__next(VectorIterator iter) {
    ++(iter->ptr);
    return 0;
}

int VectorIterator__same(VectorIterator i, VectorIterator j, int *r) {
    *r = (i->ptr == j->ptr);
    return 0;
}


////////////////////
//Headers function//
////////////////////
void
xabrt(int n) {
    if (n) {
        fprintf(stderr, "Failed function\n"); 
        abort();
    }
}

void
remove_vector(Vector v) {
    xabrt(v->tb.__remove(v));
}
void
remove_iter(VectorIterator iter) {
    xabrt(iter->ti.__remove(iter));
}

void
push_back(Vector v, int n) {
    xabrt(v->tb.__push_back(v, n));
}

VectorIterator
begin(Vector v) {
    VectorIterator iter;
    xabrt(v->tb.__begin(v, &iter));
    return iter;
}

VectorIterator
end(Vector v) {
    VectorIterator iter;
    xabrt(v->tb.__end(v, &iter));
    return iter;
}

int
get_num(VectorIterator iter) {
    int num = 0;
    xabrt(iter->ti.__get(iter, &num));
    return num;
}

void
next(VectorIterator iter) {
    xabrt(iter->ti.__next(iter));
}

int
same(VectorIterator i, VectorIterator j) {
    int r;
    xabrt(i->ti.__same(i, j, &r));
    return r;
}

//For debugging
void
print_vector(Vector v) {
    for (int i = 0; i < v->sz; i++) {
       printf("%d ", v->nums[i]);
    }
    printf("\n");
}