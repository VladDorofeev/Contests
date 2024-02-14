#include "print.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


char buffer[BUF_SIZE];

static void output(const char buf[])
{
    printf("%s", buf);
    fflush(stdout);
}

void
flush(void){
    printf("%s", buffer);
    fflush(stdout);
    for (int i = 0; i < BUF_SIZE; i++){
        buffer[i] = 0;
    }
}

void
print(const char format[], ...){
    va_list arg_list;
    va_start(arg_list, format);

    int cur_num = 0;
    //Initialitation for buffer
    for (int j = 0; j < BUF_SIZE; j++){
        if (buffer[j] == 0){
            cur_num = j;
            break;
        }
    }
    int i = 0;
    char sym;
    int need_print = 0;
    while (format[i] != 0) {
        if (cur_num == BUF_SIZE - 1){
            buffer[cur_num] = 0;
            output(buffer);
            cur_num = 0;
        }
        //Add new symbol to buffer
        if (format[i] == '%'){
            sym = va_arg(arg_list, int);
            buffer[cur_num] = sym;
        } else {
            buffer[cur_num] = format[i];
            //Check "\n"
            if (format[i] == '\n'){
                need_print = 1;
            }
        }
        cur_num++;

        if (need_print) {
            buffer[cur_num] = 0;
            output(buffer);
            need_print = 0;
            cur_num = 0;
        }
        i++;
    }
    buffer[cur_num] = 0;
    va_end(arg_list);
}
