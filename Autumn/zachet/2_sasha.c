#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <limits.h>

int
main(void) {
    char **argv = NULL;
    char *cur_word = NULL;
    char cur_byte;
    int read_word = 0;
    int cnt = 0;
    int status = -1;
    int argc = 0;
    int max_argc = 0;
    int command_exist;
    int eof_not_exist = 0;
    while(1) {
        eof_not_exist = read(0, &cur_byte, sizeof(char));
        if (cur_byte == '\n' || cur_byte == ' ' || !eof_not_exist) {
            if (read_word) {
                //printf("%s\n", cur_word);
                argc++;
                read_word = 0;
                if ((argc + 1) > max_argc) {
                    max_argc = argc + 1;
                    argv = realloc(argv, max_argc * sizeof(char*));
                }
                argv[argc - 1] = cur_word;
                argv[argc] = NULL;
                cur_word = NULL;
                cnt = 0;
                if (argc == 1) {
                    command_exist = 1;
                }
            }
        }
        if (cur_byte == '\n' || cur_byte == ';' || !eof_not_exist) {
            if (fork() == 0) {
                execvp(argv[0], argv);
                exit(INT_MAX);
            }
            wait(&status);
            for(int i = 0; i < argc; ++i) {
                free(argv[i]);
            }
            command_exist = 0;
            argc = 0;
            if (!eof_not_exist) {
                break;
            }
        } else if (cur_byte != ' '){
            cnt++;
            if (!read_word) {
                read_word = 1;
            }
            cur_word = realloc(cur_word, (cnt + 1) * sizeof(char));
            cur_word[cnt - 1] = cur_byte;
            cur_word[cnt] = '\0';
        }
    }
    free(argv);
    if (WIFEXITED(status) != 0) {
        if (WEXITSTATUS(status) == INT_MAX) {
            return 127;
        } else {
            return WEXITSTATUS(status);
        }
    }
    if (WIFSIGNALED(status)) {
        return (128 + WTERMSIG(status));
    }
    return 1;
}