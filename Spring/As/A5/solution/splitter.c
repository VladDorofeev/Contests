#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static char *delim = NULL;
static int len;
static const char **arr = NULL;


void set_array(int argc, const char *argv[]) {
    arr = argv;
    len = argc;
}

void set_delimiter(const char s[]) {
    delim = malloc(strlen(s) + 1);
    strcpy(delim, s);
}

int next_block(int *begin, int *end) {
    static int pos = 0;
    int empty = 1;
    *begin = pos;
    while (pos < len) {
        empty = 0;
        if (!strcmp(arr[pos++], delim)) {
            *end = pos - 2;
            return 0; 
        }
    }
    *end = pos - 1;
    
    return empty;

    
}
void free_splitter(void) {
    free(delim);
}
