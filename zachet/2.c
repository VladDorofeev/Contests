#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>

enum { END_CMD = 1, END_READ = 2 }; 

void
add_zero(char *s, size_t sz) {
    if (s != NULL) {
        sz++;
        s = realloc(s, sz);
        s[sz - 1] = 0;
    }
}

char *
read_word(int *status) {
    *status = 0;
    size_t sz = 0;
    char *s = NULL;
    char sym;
    int res_read;
    while (1) {
        res_read = read(STDIN_FILENO, &sym, 1);
        if (res_read == 0) {
            *status = END_READ;
            break;
        }
        if (sym == '\n') {
            *status = END_CMD;
            break;
        }
        if (sym == ' ') {
            if (s != NULL) {
                break;
            } else {
                continue;
            }
        }
        if (sym == ';') {
            *status = END_CMD;
            break;
        }
        sz++;
        s = realloc(s, sz);
        s[sz - 1] = sym;
    }

    add_zero(s, sz);
    return s;
}

int
main(void) {
    char *s = NULL;
    
    char **prog_args = NULL;
    size_t arg_sz = 0;

    int status = 0;
    int status_read = 0;

    while (1) {
        s = read_word(&status_read);
        if ((status_read == END_CMD) || (status_read == END_READ)) {
            if ((status_read == END_READ) && (prog_args == NULL)) {
                break;
            }
            //If cmd end at \n
            if (s != NULL) {
                arg_sz++;
                prog_args = realloc(prog_args, arg_sz * (sizeof *prog_args));
                prog_args[arg_sz - 1] = malloc(strlen(s) + 1);
                strcpy(prog_args[arg_sz - 1], s);
            }
            //End read cmd so run cmd
            arg_sz++;
            prog_args = realloc(prog_args, arg_sz * sizeof *prog_args);
            prog_args[arg_sz - 1] = NULL;
            if (fork() == 0) {
                execvp(prog_args[0], prog_args);
                exit(127);
            }
            wait(&status);

            //Free all
            for (int i = 0; i < arg_sz; i++) {
                free(prog_args[i]);
            }
            free(prog_args);
            prog_args = NULL;
            arg_sz = 0;
        } else {
            if (s == NULL) { 
                break;
            }
            arg_sz++;
            prog_args = realloc(prog_args, arg_sz * (sizeof *prog_args));
            
            prog_args[arg_sz - 1] = malloc(strlen(s) + 1);
            strcpy(prog_args[arg_sz - 1], s);
        }

        free(s);
        s = NULL;
    }


    if (WIFEXITED(status)) {
        exit(WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)){
        exit(WTERMSIG(status) + 128);
    } else {
        exit(1);
    }
}