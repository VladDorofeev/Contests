#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>

enum Type{
    INT,
    DOUBLE
};

void
vmax(enum Type this_type, size_t cnt, ...){
    va_list arg_list;
    va_start(arg_list, cnt);
    int result_i = INT_MIN;
    double result_d = - (DBL_MAX - 1);
    for (int i = 0; i < cnt; i++){
        if (this_type == INT){
            int arg_i = va_arg(arg_list, int);
            if (arg_i > result_i){
                result_i = arg_i;
            }
        }
        if (this_type == DOUBLE){
            double arg_d = va_arg(arg_list, double);
            if (arg_d > result_d){
                result_d = arg_d;
            }
        }
    }
    if (this_type == DOUBLE){
        printf("%g\n", result_d);        
    } else {
        printf("%d\n", result_i);        
    }
}




int 
main(void)
{
    vmax(INT, 4, 0, 1, 2, 10);
    vmax(INT, 4, 10, -9, 8, 10);
    vmax(INT, 1, 10);
    vmax(INT, 5, 0, 0, 0, 10, -10);

    vmax(DOUBLE, 4, 0.0, -1.12, 2.4, 100.0);
    vmax(DOUBLE, 4, 100.0, -2.22507e-308, -134.531, 8,99999);
    vmax(DOUBLE, 1, 100.0);
    vmax(DOUBLE, 4, 1.0e2, -1.1231241411, 2.12351, 4.13431e1);

}