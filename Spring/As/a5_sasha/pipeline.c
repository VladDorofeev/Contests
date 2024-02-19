#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "pipeline.h"

char*** pipeline_seq = 0;
int seq_size = 0;

int
add_to_pipeline(const char *array[], int size) {
    seq_size++;
    if ((pipeline_seq = realloc(pipeline_seq, seq_size * sizeof(char **))) == NULL) {
        return 1;
    }
    pipeline_seq[seq_size - 1] = NULL;
    if ((pipeline_seq[seq_size - 1] = realloc(pipeline_seq[seq_size - 1],
                                    (size + 1) * sizeof(char*))) == NULL) {
        return 1;
    }
    for(int i = 0; i < size; ++i) {
        pipeline_seq[seq_size - 1][i] = NULL;
        if ((pipeline_seq[seq_size - 1][i] = realloc(pipeline_seq[seq_size - 1][i],
                                        (strlen(array[i]) + 1) * sizeof(char))) == NULL) {
            return 1;
        }
        strcpy(pipeline_seq[seq_size - 1][i], array[i]);                                    
    }
    pipeline_seq[seq_size - 1][size] = NULL;
    return 0;
}

int run_pipeline(void) {
    int fd1[2] = {};
    int fd2[2] = {};
    for(int i = 0; i < seq_size; ++i) {
        pid_t pid = -1;
        if (i % 2 == 0) {
            if (i > 0) {
                close(fd1[0]);
                close(fd1[1]);
            }
            pipe(fd1);
        } else if (i % 2 == 1) {
            if (i > 1) {
                close(fd2[0]);
                close(fd2[1]);
            }
            pipe(fd2);
        }
        if ((pid = fork()) == 0) {
            if (seq_size > 1) {
                if (i % 2 == 0){
                    if (i < (seq_size - 1)) {
                        dup2(fd1[1], 1);
                    }
                    if (i > 0) {
                        dup2(fd2[0], 0);
                    }
                } else if (i % 2 == 1) {
                    dup2(fd1[0], 0);
                    if (i < (seq_size - 1)) {
                        dup2(fd2[1], 1);
                    }
                }
                close(fd1[0]);
                close(fd1[1]);
                close(fd2[0]);
                close(fd2[1]);
            }
            execvp(pipeline_seq[i][0], &pipeline_seq[i][0]);
            exit(0);
        } else if (pid == -1) {
            return 1;
        }
    }
    close(fd1[1]);
    close(fd1[0]);
    if (seq_size > 1) {
        close(fd2[1]);
        close(fd2[0]);  
    }
    while(wait(NULL) != -1) {
    }
    return 0;
}

void free_pipeline(void) {
    for(int i = 0; i < seq_size; ++i) {
        for(int j = 0; pipeline_seq[i][j] != NULL; ++j) {
            free(pipeline_seq[i][j]);
        }
        free(pipeline_seq[i]);
    }
    free(pipeline_seq);
}