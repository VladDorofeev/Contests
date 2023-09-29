#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

static const char *type[] = {
    /// TYPE1 HERE AS STRING LITERAL
    "float", "unsigned"
    /// TYPE2 HERE AS STRING LITERAL
};

static const char file[][95] = {
    {35, 105, 110, 99, 108, 117, 100, 101, 32, 60, 115, 116,
    100, 105, 111, 46, 104, 62, 10, 105, 110, 116, 32, 109, 97,
    105, 110, 40, 118, 111, 105, 100, 41, 32, 123, 32, 37, 115,
    32, 113, 59, 32, 102, 108, 111, 97, 116, 32, 42, 112, 32, 61,
    32, 38, 113, 59, 32, 105, 102, 32, 40, 112, 41, 32, 112, 114,
    105, 110, 116, 102, 40, 34, 79, 75, 92, 110, 34, 41, 59, 32,
    114, 101, 116, 117, 114, 110, 32, 48, 59, 32, 125, 0, 0, 0, 0},
    {35, 105, 110, 99, 108, 117, 100, 101, 32, 60, 115, 116, 100,
    105, 111, 46, 104, 62, 10, 105, 110, 116, 32, 109, 97, 105, 110,
    40, 118, 111, 105, 100, 41, 32, 123, 32, 37, 115, 32, 113, 59, 32,
    117, 110, 115, 105, 103, 110, 101, 100, 32, 42, 112, 32, 61, 32, 38,
    113, 59, 32, 105, 102, 32, 40, 112, 41, 32, 112, 114, 105, 110, 116,
    102, 40, 34, 79, 75, 92, 110, 34, 41, 59, 32, 114, 101, 116, 117,
    114, 110, 32, 48, 59, 32, 125, 0}
};

int
main(void)
{
    static char path[] = "prog.c";
    static char path_bin[] = "prog";
    static char buf[1024] = {0};

    struct stat sb;
    if (stat(path, &sb) != -1) {
        fprintf(stderr, "Please rename file %s to a different name\n", path);
        return 1;
    }

    if (stat(path_bin, &sb) != -1) {
        fprintf(stderr, "Please rename file %s to a different name\n", path_bin);
        return 1;
    }

    for (int i = 0; i < sizeof file / sizeof *file; ++i) {
        size_t buf_sz = sprintf(buf, file[i], type[i]);
        int f = creat(path, 0600);
        if (f == -1) {
            perror(path);
            return 1;
        }
        write(f, &buf[0], buf_sz);
        close(f);

        if (fork() == 0) {
            close(1);
            int efd = dup(2);
            close(2);
            execlp("gcc", "gcc", "-Wall", "-Werror", path, "-o", path_bin, (char *)0);
            int e = errno;
            dup2(efd, 2);
            errno = e;
            perror("gcc");
            abort();
        }
        int status;
        wait(&status);
        unlink(path);
        unlink(path_bin);
        if (!WIFEXITED(status)) {
            return 1;
        }
        if (WEXITSTATUS(status) != 0) {
            printf("type%d is WRONG\n", i + 1);
            return 0;
        }
    }

    printf("OK\n");
    return 0;
}
