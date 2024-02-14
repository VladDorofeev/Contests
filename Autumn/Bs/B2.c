#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>

static void
solution(void)
{
    /// WRITE HERE
    unsigned long long b;
    unsigned sum = 0;
    scanf("%llu", &b);
    while (b > 0) {
        sum += b % 2; 
        b /= 2;
    }
    printf("%u", sum);
    ///
}

int
main(void)
{
    typedef struct {
        unsigned long long n;
        int answer;
    } Test;

    static Test tests[] = {
        { .n = 0, .answer = 0 },
        { .n = 1, .answer = 1 },
        { .n = 2, .answer = 1 },
        { .n = 3, .answer = 2 },
        { .n = 5, .answer = 2 },
        { .n = ~0ULL, .answer = CHAR_BIT * sizeof tests[0].n },
        { .n = ~0ULL << 1, .answer = CHAR_BIT * sizeof tests[0].n - 1 }
    };

    for (unsigned i = 0; i < sizeof tests / sizeof *tests; ++i) {
        int fd[2], fd_in[2];
        pipe(fd);
        pipe(fd_in);
        if (fork() == 0) {
            dup2(fd[1], 1);
            dup2(fd_in[0], 0);
            close(fd[0]);
            close(fd[1]);
            close(fd_in[0]);
            close(fd_in[1]);
            solution();
            return 0;
        }
        close(fd[1]);
        close(fd_in[0]);
        FILE *out = fdopen(fd_in[1], "w");
        if (!out) {
            perror("fdopen-out");
            return 1;
        }
        fprintf(out, "%llu\n", tests[i].n);
        fclose(out);
        close(fd_in[1]);

        FILE *in = fdopen(fd[0], "r");
        if (!in) {
            perror("fdopen");
            return 1;
        }

        int d;
        if (fscanf(in, "%d", &d) != 1) {
            printf("TEST %d FAILED: not a decimal at output\n", i);
            return 0;
        }
        if (d != tests[i].answer) {
            printf("TEST %d FAILED: WRONG OUTPUT: "
                "expected %d, gotten %d\n", i, tests[i].answer, d);
            return 0;
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
