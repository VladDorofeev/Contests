#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdarg.h>

static void output(const char buf[])
{
    printf("%s", buf);
    fflush(stdout);
}

enum { BUF_SIZE = 8 };

/// WRITE HERE

void
print(const char format[], ...){
    va_list arg_list;
    va_start(arg_list, format);

    static char buffer[BUF_SIZE];
    int cur_num = 0;
    //Initialitation for buffer
    for (int j = 0; j < BUF_SIZE; j++){
        if (buffer[j] == 0){
            cur_num = j;
            break;
        }
    }
    int i = 0;
    char sym;
    int need_print = 0;
    while (format[i] != 0) {
        if (cur_num == BUF_SIZE - 1){
            buffer[cur_num] = 0;
            output(buffer);
            cur_num = 0;
        }
        //Add new symbol to buffer
        if (format[i] == '%'){
            sym = va_arg(arg_list, int);
            buffer[cur_num] = sym;
        } else {
            buffer[cur_num] = format[i];
            //Check "\n"
            if (format[i] == '\n'){
                need_print = 1;
            }
        }
        cur_num++;

        if (need_print) {
            buffer[cur_num] = 0;
            output(buffer);
            need_print = 0;
            cur_num = 0;
        }
        i++;
    }
    buffer[cur_num] = 0;
    va_end(arg_list);
}

///

static void
action0(void)
{
    print("123\n");
}
static const char * const responce0 = "123\n";

static void
action1(void)
{
    print("%+%\n", 'A', 'B');
}
static const char * const responce1 = "A+B\n";

static void
action2(void)
{
    print("12");
    print("34\n");
}
static const char * const responce2 = "1234\n";

static void
action3(void)
{
    print("12\n");
    print("34");
}
static const char *const responce3 = "12\n";

static char buf4[BUF_SIZE];
static void action4(void)
{
    print(&buf4[0]);
}
static const char * const responce4 = "";

static char buf5[BUF_SIZE + 1];
static void action5(void)
{
    print(&buf5[0]);
}
static const char * const responce5 = buf4;

static void action6(void)
{
    print("12");
    printf("$$"); fflush(stdout);
    print("34\n");
}
static const char * const responce6 = "$$1234\n";

static void action7(void)
{
    print("12\n34\n");
}
static const char * const responce7 = "12\n34\n";

static void action8(void)
{
    print("12\n3");
    printf("!!"); fflush(stdout);
    print("45\n");
}
static const char * const responce8 = "12\n!!345\n";

static void action9(void)
{
    print("1234\nABCDE");
    printf("@@"); fflush(stdout);
    print("X\n");
}
static const char * const responce9 = "1234\n@@ABCDEX\n";

static void action10(void)
{
    print("01234567890123456789");
    printf("**"); fflush(stdout);
}
static const char * const responce10 = "01234567890123**";

int
main(void)
{
    typedef struct {
        void (*action)(void);
        const char *responce;
    } Test;

    static Test tests[] = {
        { action0, responce0, },
        { action1, responce1, },
        { action2, responce2, },
        { action3, responce3, },
        { action4, responce4, },
        { action5, responce5, },
        { action6, responce6, },
        { action7, responce7, },
        { action8, responce8, },
        { action9, responce9, },
        { action10, responce10, },
    };

    {
        for (size_t i = 0; i < BUF_SIZE - 1; ++i) {
            buf4[i] = 'q';
        }
        for (size_t i = 0; i < BUF_SIZE; ++i) {
            buf5[i] = 'q';
        }
    }

    int ok = 1;
    for (size_t i = 0; i < sizeof tests / sizeof *tests; ++i) {
        int fd[2];
        if (pipe(fd) == -1) {
            perror("pipe");
            return 1;
        }
        if (fork() == 0) {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            tests[i].action();
            return 0;
        }
        close(fd[1]);

        FILE *in = fdopen(fd[0], "r");
        if (!in) {
            perror("fdopen-in");
            return 1;
        }

        static char buf[1024];
        memset(&buf[0], '\0', sizeof buf);
        size_t overflow = 0;
        {
            size_t start = 0;
            while (1) {
                if (start >= sizeof buf) {
                    overflow = 1;
                    break;
                } else if (!fgets(&buf[start], sizeof buf - start, in)) {
                    break;
                } else {
                    start += strlen(&buf[start]);
                }
            }
        }
        if (overflow) {
            printf("TEST %lu FAILED: large output\n", i);
            ok = 0;
        } else if (strcmp(buf, tests[i].responce) != 0) {
            printf("%s\n", &buf[0]);
            fflush(stdout);
            printf("TEST %lu FAILED: wrong output (gotten len = %lu)\n", i, strlen(buf));
            ok = 0;
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

    if (ok) {
        printf("OK\n");
    }
}
