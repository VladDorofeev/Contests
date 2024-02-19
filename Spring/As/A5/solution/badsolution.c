#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int
main(int argc, char *argv[])
{ 
    ++argv;
    --argc;
    int comm_start = 0, fd_next[2] = {0, 1}, fd_prev[2];
    const char *divider = "--";
    for (int i = 0; i <= argc; ++i)
    {
        if ((i == argc) || (strcmp(argv[i], divider) == 0))
        {
            argv[i] = NULL;
            fd_prev[0] = fd_next[0];
            fd_prev[1] = fd_next[1];
            if (i != argc)
            {
                pipe(fd_next);
            }
            if (fork() == 0)
            {
                if (comm_start > 0)
                { 
                    dup2(fd_prev[0], 0);
                    close(fd_prev[0]);
                    close(fd_prev[1]);
                }
                if (i != argc)
                {
                    dup2(fd_next[1], 1);
                    close(fd_next[0]);
                    close(fd_next[1]);
                }
                execvp(argv[comm_start], argv + comm_start);
                return 0;
            }
            if ((comm_start > 0) || (i == argc))
            {
                close(fd_prev[0]);
                close(fd_prev[1]);
            }
            comm_start = i + 1;
        }
    }

    while(wait(NULL) != -1);

    return 0;
}