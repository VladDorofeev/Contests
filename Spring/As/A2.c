#include <stdio.h>
#include <string.h>

enum { STRING_SZ = 82 };

int
main(int argc, char **argv) {

    char substr[STRING_SZ];
    fgets(substr, STRING_SZ, stdin);
    char *p;
    if ((p = strchr(substr, '\n')) != NULL) {
        *p = 0;    
    }

    char **filename = argv + 1;

    FILE *fp = fopen(*filename, "r");

    FILE *tmpfp = tmpfile();

    char line[STRING_SZ];
    while (fgets(line, STRING_SZ, fp) != NULL) {
        if (strstr(line, substr) != NULL) {
            fwrite(line, 1, strlen(line), tmpfp);
        }
    }

    fclose(fp);
    fp = fopen(*filename, "w");
    fseek(tmpfp, 0, SEEK_SET);

    while(fgets(line, STRING_SZ, tmpfp) != NULL) {
        fwrite(line, 1, strlen(line), fp);
    }

    fclose(fp);
    fclose(tmpfp);

    return 0;
}