#include <stdio.h>
#include <string.h>
#include <ctype.h>


int
main (int argc, char *argv[], char *argp[]) {
    for (int i = 1; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }
    for (int i = 1; i < argc; ++i) {
        char *ptr = strstr(argv[i], "end");
        if (ptr != NULL) {
            ptr += 3;
            ptr = strstr(ptr, "end");
            if ((ptr != NULL) && (strlen(ptr + 3) != 0)) {
                printf("%s\n", ptr + 3);
            }
        }
    }
    return 0;
}