#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

// it is eligible to change fsm array
void
solution(int fsm[], unsigned fsm_size)
{
    //We will coding state and label
    //code = state and label in one number
    //int state = 0;
    //int label = 0;
    //long code = ((long)(state) << 16) + label;

    printf("#include <stdio.h>\n");
    printf("int\n");
    printf("main (void) {\n");
    printf("    int state = 0;\n");
    printf("    long code;\n");
    printf("    int label;\n");
    char format_str[] = "%d";
    printf("    scanf(\"%s\", &label);\n", format_str);
    printf("    while (label != 0) {\n");
    printf("        code = (long)((long)(state + 1) << 15) + (long)(label);\n");
    printf("        switch (code) {\n");
    long code;
    for (unsigned i = 0; i < fsm_size; ++i){
        code = (long)((long)(fsm[3 * i] + 1) << 15) + (long)(fsm[3 * i + 1]);
        printf("        case %ld:\n", code);
        printf("            state = %d;\n", fsm[3 * i + 2]);
        printf("            break;\n");
    }
    printf("            default:\n");
    printf("                printf(\"NO\\n\");\n");
    printf("                return 0;\n");
    printf("        }\n");
    printf("        scanf(\"%s\", &label);\n", format_str);
    printf("    }\n");
    printf("    if (state == 1) {\n");
    printf("        printf(\"YES\\n\");\n");
    printf("    } else {\n");
    printf("        printf(\"NO\\n\");\n");
    printf("    }\n");
    printf("}\n");
}


enum { MAX_LEN = 128 };
enum { MODE = 0600 };

int
main(void) 
{
    static const char source[] = "prog.c";
    static const char binary[] = "./prog";
    static const char *files[] = { source, binary, };

    {
        for (unsigned i = 0; i < sizeof files / sizeof *files; ++i) {
            struct stat sb;
            if (stat(files[i], &sb) == 0) {
                fprintf(stderr, "%s exists\n", files[i]);
                return 1;
            }
            int fd = creat(files[i], MODE);
            if (fd == -1) {
                perror("creat");
                return 1;
            }
            close(fd);
            unlink(files[i]);
        }
    }

    typedef struct {
        unsigned len;
        int *input;
        int answer;
    } IntStringTest;

    typedef struct {
        unsigned fsm_size;
        int *fsm;
        unsigned strings_size;
        IntStringTest strings[MAX_LEN];
    } Test;

    static int fsm1[] = {
        0, 1, 1,
    };
    static int fsm2[] = {
        0, 1, 0,
    };
    static int fsm3[] = {
        0, 1, 0,
        0, 2, 1,
    };
    static int fsm4[] = {
        0, 1, 2,
        2, 2, 1,
    };
    static int fsm5[] = {
        0, 1, 2,
        2, 1, 0,
        0, 2, 1,
    };
    static int fsm6[] = {
        0, 1, 2,
        0, 2, 1,
        2, 1, 0,
    };
    enum { fsmL_size = 9001 };
    static int fsmL[3 * 100000]; {
        int i;
        for (i = 0; i < 9000; ++i) {
            fsmL[3 * i] = 0;
            fsmL[3 * i + 1] = i + 1;
            fsmL[3 * i + 2] = 0;
        }
        fsmL[3 * i] = 0;
        fsmL[3 * i + 1] = i + 1;
        fsmL[3 * i + 2] = 1;
    }

    static int a1[] = {1, 0};
    static int a2[] = {2, 0};
    static int a_1[] = {-1, 0};
    static int a11[] = {1, 1, 0};
    static int a12[] = {1, 2, 0};
    static int a21[] = {2, 1, 0};
    static int a22[] = {2, 2, 0};
    static int a13[] = {1, 3, 0};
    static int a112[] = {1, 1, 2, 0};
    static int a122[] = {1, 2, 2, 0};
    static int a121[] = {1, 2, 1, 0};
    static int a211[] = {2, 1, 1, 0};
    static int a9000[10002]; {
        int i;
        for (i = 0; i < 10000; ++i) {
            a9000[i] = 9000;
        }
        a9000[i++] = 9001;
        a9000[i++] = 0;
    }

    static Test tests[] = {
        {
            .fsm_size = 0, .strings_size = 2, .strings = {
                { 2, a1, 0, },
                { 2, a_1, 0, },
            },
        },
        {
            .fsm_size = 1, .fsm = fsm1, .strings_size = 3, .strings = {
                { 2, a1, 1, },
                { 2, a_1, 0, },
                { 3, a11, 0, },
            },
        },
        {
            .fsm_size = 1, .fsm = fsm2, .strings_size = 3, .strings = {
                { 2, a1, 0, },
                { 2, a_1, 0, },
                { 3, a11, 0, },
            },
        },
        {
            .fsm_size = 2, .fsm = fsm3, .strings_size = 5, .strings = {
                { 2, a1, 0, },
                { 3, a13, 0, },
                { 3, a12, 1, },
                { 4, a112, 1, },
                { 4, a122, 0, },
            },
        },
        {
            .fsm_size = 2, .fsm = fsm4, .strings_size = 3, .strings = {
                { 3, a12, 1, },
                { 4, a112, 0, },
                { 4, a122, 0, }, 
            },
        },
        {
            .fsm_size = 3, .fsm = fsm5, .strings_size = 9, .strings = {
                { 2, a1, 0, },
                { 2, a2, 1, },
                { 3, a11, 0, },
                { 3, a12, 0, },
                { 3, a21, 0, },
                { 3, a22, 0, },
                { 4, a112, 1, },
                { 4, a121, 0, },
                { 4, a211, 0, }, 
            },
        },
        {
            .fsm_size = 3, .fsm = fsm6, .strings_size = 9, .strings = {
                { 2, a1, 0, },
                { 2, a2, 1, },
                { 3, a11, 0, },
                { 3, a12, 0, },
                { 3, a21, 0, },
                { 3, a22, 0, },
                { 4, a112, 1, },
                { 4, a121, 0, },
                { 4, a211, 0, }, 
            },
        },
        {
            .fsm_size = fsmL_size, .fsm = fsmL, .strings_size = 1, .strings = {
                { sizeof a9000 / sizeof *a9000, a9000, 1, },
            },
        },
    };

    int ret = 0;
    for (unsigned i = 0; i < sizeof tests / sizeof *tests; ++i) {
        {
            int fd[2];
            pipe(fd);
            if (fork() == 0) {
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                solution(&tests[i].fsm[0], tests[i].fsm_size);
                return 0;
            }
            close(fd[1]);
            int prog = creat(source, MODE);
            if (prog == -1) {
                perror(source);
                ret = 1;
                goto FINALLY;
            }

            char buf[4096];
            ssize_t sz;
            while ((sz = read(fd[0], &buf[0], sizeof buf)) > 0) {
                write(prog, &buf[0], sz);
            }
            close(prog);
            close(fd[0]);
            int status;
            wait(&status);
            if (! WIFEXITED(status)) {
                perror("wait");
                ret = 1;
                goto FINALLY;
            }
        }

        {
            if (fork() == 0) {
                return execlp("gcc", "gcc", source, "-o", binary,
                "-Wformat", "-Werror", "-std=gnu11", "-fsanitize=address",
                "-fno-sanitize-recover", "-ftrapv", "-fsanitize=undefined",
                "-Wformat-security", "-Wignored-qualifiers", "-Winit-self",
                "-Wswitch-default", "-Wfloat-equal", "-Wshadow", "-Wpointer-arith",
                "-Wtype-limits", "-Wempty-body", "-Wlogical-op", "-Wstrict-prototypes",
                "-Wold-style-declaration", "-Wold-style-definition",
                "-Wmissing-parameter-type", "-Wmissing-field-initializers",
                "-Wnested-externs", "-Wno-pointer-sign", "-lm",
                (char *)0);
            }
            int status;
            if (wait(&status) == -1) {
                perror("wait");
                ret = 1;
                goto FINALLY;
            }
            if (! WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                printf("TEST %d FAILED: exec gcc error or compilation error\n", i);
                goto FINALLY;
            }
        }

        for (unsigned k = 0; k < tests[i].strings_size; ++k) {
            IntStringTest *test = &tests[i].strings[k];
            int fd_out[2], fd_in[2];
            pipe(fd_out);
            pipe(fd_in);
            if (fork() == 0) {
                dup2(fd_out[1], 1);
                dup2(fd_in[0], 0);
                close(fd_out[0]);
                close(fd_out[1]);
                close(fd_in[0]);
                close(fd_in[1]);
                return execlp(binary, binary, (char *)0);
            }
            close(fd_out[1]);
            close(fd_in[0]);
            FILE *out = fdopen(fd_in[1], "w");
            if (!out) {
                perror("fdopen-out");
                ret = 1;
                goto FINALLY;
            }
            for (unsigned q = 0; q < test->len; ++q) {
                fprintf(out, "%d\n", test->input[q]);
            }
            fclose(out);
            close(fd_in[1]);

            FILE *in = fdopen(fd_out[0], "r");
            if (!in) {
                perror("fdopen-in");
                ret = 1;
                goto FINALLY;
            }

            char buf[MAX_LEN] = {0};
            char *expected = (test->answer ? "YES\n" : "NO\n");
            if (!fgets(&buf[0], sizeof buf, in)) {
                printf("TEST %d FAILED: "
                        "compiled program output is empty "
                        "at string %d\n", i, k);
                goto FINALLY;
            } else if (!strchr(buf, '\n')) {
                printf("TEST %d FAILED: "
                        "compiled program output without newline "
                        "at string %d\n", i, k);
                goto FINALLY;
            } else if (strcmp(buf, expected) != 0) {
                printf("TEST %d FAILED: "
                        "compiled program output is wrong "
                        "at string %d: expected %s, gotten %s\n", i, k, expected, buf);
                goto FINALLY;
            }
            fclose(in);
            close(fd_out[0]);
            int status;
            wait(&status);
            if (! WIFEXITED(status)) {
                perror("wait");
                ret = 1;
                goto FINALLY;
            }

        }
    }

    printf("OK\n");

FINALLY:
    unlink(source);
    unlink(binary);
    return ret;
}
