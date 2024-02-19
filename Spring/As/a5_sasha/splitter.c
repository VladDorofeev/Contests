#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "splitter.h"

char** commands_seq = 0;
int cur_pos = 0;
char* delimiter = 0;
int seq_len = 0;

void
set_array(int cnt, const char** commands) {
    seq_len = cnt;
    commands_seq = realloc(commands_seq, (seq_len + 1) * sizeof(char*));
    commands_seq[seq_len] = NULL; 
    for(int i = 0; i < seq_len; ++i) {
        commands_seq[i] = NULL;
        commands_seq[i] = realloc(commands_seq[i],
        (strlen(commands[i]) + 1) * sizeof(char));
        strcpy(commands_seq[i], commands[i]);
    }
}

void
set_delimiter(const char * del) {
    delimiter = realloc(delimiter, (strlen(del) + 1) * sizeof(char));
    strcpy(delimiter, del);
}

int
next_block(int* num1, int* num2) {
    if (cur_pos >= seq_len) {
        return 1;
    } else {
        *num1 = cur_pos;
        for(;cur_pos <= seq_len; ++cur_pos) {
            if (commands_seq[cur_pos] == NULL ||
            strcmp(commands_seq[cur_pos], delimiter) == 0) {
                *num2 = cur_pos;
                cur_pos++;
                break;
            }
        }
    }
    return 0;
}

void
free_splitter(void) {
    for(int i = 0; i < seq_len; ++i) {
        free(commands_seq[i]);
    }
    free(commands_seq);
    free(delimiter);
}
