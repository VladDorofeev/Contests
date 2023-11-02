#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

void
child_proc(int num, FILE *fp) {
    char byte = 1;
    if (num == 2) {
        fwrite(&byte, 1, 1, fp);
        printf("2\n");
    } else {
        while (fread(&byte, 1, 1, fp) != 1) {
            fseek(fp, 0, SEEK_SET);
            usleep (100);
        }
        printf("1\n");
    }
    fclose(fp);
}


int
main (void) {
    FILE *fp = tmpfile();
    if (fork() == 0) {
        child_proc(1, fp);
        return 0;
    }
    if (fork() == 0) {
        child_proc(2, fp);
        return 0;
    }
    wait(NULL);
    wait(NULL);
    
    fclose(fp);
    return 0;
}