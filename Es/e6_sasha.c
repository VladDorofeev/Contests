#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[]){
    int i;
    for (i = 0; i < argc; ++i){
        if (strstr(*argv[i], "end") != NULL) {
            printf("%s", *argv[i]);
        }
    }
    retunr 0;
}