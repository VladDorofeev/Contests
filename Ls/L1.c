#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>

int
is_prog (char *name) {
    if (name[0] != '-') {
        return 1;
    } else {
        if (name[0] != '-') {
            return 1;
        }
    }
    return 0;
}

enum {LAST_PROG = -1, FIRST_LAST_PROG = -2, FIRST_PROG = 1};

int 
main (int argc, char **argv) {
    int j;
    pid_t cur_pid;
    char *next_prog_name;
    int cnt_prog = FIRST_PROG;
    int fd[2];
    pipe(fd);

    for (int i = 1; i < argc; ++i) {
        if (is_prog(argv[i]) == 1) {
            for (j = i + 1; (j != argc) && (is_prog(argv[j]) == 0); ++j);

            //There we are cathing last program
            if (j == argc) {
                if (cnt_prog == FIRST_PROG) {
                    cnt_prog = FIRST_LAST_PROG;                
                } else {
                    cnt_prog = LAST_PROG;
                }
            }

            next_prog_name = argv[j];
            *(argv + j) = (char *)NULL;
            
            while ((cur_pid = fork()) == -1) {
                wait(NULL);
            }
            if (cur_pid == 0) {
                if ((cnt_prog != LAST_PROG) && (cnt_prog != FIRST_LAST_PROG)) {
                    dup2(fd[1], STDOUT_FILENO);
                }
                
                if ((cnt_prog != FIRST_PROG) && (cnt_prog != FIRST_LAST_PROG)) {
                    dup2(fd[0], STDIN_FILENO);
                }
                
                close(fd[0]);
                close(fd[1]);
                
                execvp(argv[i], &argv[i]);
                return 0;
            }
            wait(NULL);
            cnt_prog++;
            *(argv + j) = next_prog_name;
        }
    }

    while (wait(NULL) != -1);
    
    close(fd[0]);
    close(fd[1]);
    
    return 0;
}