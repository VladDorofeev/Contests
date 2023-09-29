#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

enum { MAX_LEN = 100 };

static void
solution(const int numbers[], const unsigned len)
{
    /// WRITE HERE
    for (unsigned i = 0; i < len; i++){
        if (numbers[i] % 2 == 0){
            printf("%d ", numbers[i]);
        }
    }
    for (int i = len - 1; i >= 0; i--){
        if (numbers[i] % 2 != 0){
            printf("%d ", numbers[i]);
        }
    }
    ///
}

int
main(void)
{
    typedef struct {
        unsigned len;
        int input[MAX_LEN];
        int answer[MAX_LEN];
    } Test;

    static Test tests[] = {
        { .len = 0, .input = {}, .answer = {} },
        { .len = 1, .input = {0}, .answer = {0} },
        { .len = 1, .input = {1}, .answer = {1} },
        { .len = 2, .input = {0, 2}, .answer = {0, 2} },
        { .len = 2, .input = {2, 0}, .answer = {2, 0} },
        { .len = 2, .input = {1, 3}, .answer = {3, 1} },
        { .len = 2, .input = {1, 2}, .answer = {2, 1} },
        { .len = 6, .input = {1, 3, -4, 2, -1, 8}, .answer = {-4, 2, 8, -1, 3, 1} }
    };
    for (unsigned i = 0; i < sizeof tests / sizeof *tests; ++i) {
        int fd[2];
        pipe(fd);
        if (fork() == 0) {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            solution(&tests[i].input[0], tests[i].len);
            return 0;
        }
        close(fd[1]);
        FILE *in = fdopen(fd[0], "r");
        if (!in) {
            perror("fdopen");
            return 1;
        }

        for (unsigned k = 0; k < tests[i].len; ++k) {
            int d;
            if (fscanf(in, "%d", &d) != 1) {
                printf("TEST %d FAILED: not a decimal at position %d\n", i, k);
                return 0;
            }
            if (d != tests[i].answer[k]) {
                printf("TEST %d FAILED: WRONG OUTPUT at position %d: "
                    "expected %d, gotten %d\n", i, k, tests[i].answer[k], d);
                return 0;
            }
        }
        if (fscanf(in, " %*c") >= 0) {
            printf("TEST %d FAILED: EXCESS TEXT AT THE END\n", i);
            return 0;
        }
        fclose(in);
        close(fd[0]);
        int status;
        wait(&status);
        if (! WIFEXITED(status)) {
            perror("wait");
            return 1;
        }
    }

    printf("OK\n");
}
