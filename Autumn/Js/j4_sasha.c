#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void
do_process(int num, FILE* file) {
    int exitst_num = 2;
    if (num == 1) {
        while(fread(&exitst_num, sizeof(exitst_num), 1, file) != 1) {
            fseek(file, 0, SEEK_SET);
            usleep(1234);
        }
    } else {
        fwrite(&exitst_num, sizeof(exitst_num), 1, file);
    }
    fclose(file);
    printf("%d\n", num);
}

int
main(void) {
    FILE* my_file = tmpfile();
    int status;
    if (fork() == 0) {
        do_process(1, my_file);
        exit(0);
    } else {
        if (fork() == 0) {
            do_process(2, my_file);
            exit(0);
        }
        wait(&status);
        wait(&status);
    }
    fclose(my_file);
    return 0;
}