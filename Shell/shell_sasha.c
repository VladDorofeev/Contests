#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

enum {KIND_SIMPLE, KIND_PIPELINE,
    KIND_REDIRECT, KIND_SEQ2,
    KIND_SEQ1, RD_OUTPUT,
    RD_INPUT, RD_APPEND,
    OP_CONJUNCT, OP_BACKGROUND,
    OP_DISJUNCT, OP_SEQ};

typedef struct Command{
    int kind;
    union {
        struct {
            int argc;
            char **argv;
        };
        struct {
            int pipeline_size;
            struct Command* pipeline_commands;
        };
        struct {
            int rd_mode;
            char *rd_path;
            struct Command* rd_command;
        };
        struct {
            int seq_size;
            struct Command* seq_commands;
            int *seq_operations;
        };
    };
}Command;

int
run_command(Command *);

int
do_pipeline(Command *);

void
do_simple(Command * simple_command) {
    execvp(simple_command->argv[0], simple_command->argv);
    exit(1);
}

void
do_redirect(Command * redirect_command) {
    int fd;
    switch (redirect_command->rd_mode) {
    case RD_INPUT:
        fd = open(redirect_command->rd_path, O_RDONLY);
        dup2(fd, 0); 
        close(fd);
        break;
    case RD_OUTPUT: 
        fd = open(redirect_command->rd_path, O_WRONLY | O_TRUNC | O_CREAT, 0600);
        dup2(fd, 1);
        close(fd);
        break;
    case RD_APPEND:
        fd = open(redirect_command->rd_path, O_WRONLY | O_APPEND | O_CREAT, 0600);
        dup2(fd, 1);
        close(fd);
        break;
    default:
        break;
    }
    do_simple(redirect_command->rd_command);
}

void
do_seq2(Command * seq2_command) {
    int status;
    //printf("in seq2\n");
    status = do_pipeline(&(seq2_command->seq_commands[0]));
    for (int i = 1; i < seq2_command->seq_size; ++i) {
        if (((seq2_command->seq_operations[i - 1] == OP_DISJUNCT) && 
            ((WIFEXITED(status) == 0) || (WEXITSTATUS(status) != 0))) ||
            ((seq2_command->seq_operations[i - 1] == OP_CONJUNCT) && 
            ((WIFEXITED(status) != 0) && (WEXITSTATUS(status) == 0)))) {
            status = do_pipeline(&(seq2_command->seq_commands[i]));
        }
    }
}

int
do_seq1(Command * seq1_command) {
    pid_t pid;
    //printf("in seq1\n");
    for (int i = 0; i < seq1_command->seq_size; ++i) {
        if ((pid = fork()) == 0) {
            run_command(&(seq1_command->seq_commands[i]));
            exit(0);
        } else {
            if (seq1_command->seq_operations[i] == OP_SEQ) {
                waitpid(pid, NULL, 0);
            }
        }
    }
    return 0;
}

int
do_pipeline(Command * pipeline_command) {
    int status;
    int ret_status = 0;
    int fd1[2];
    int fd2[2];
    pid_t pid = 0;
    pid_t do_wait = 0;
    for (int i = 0; i < pipeline_command->pipeline_size; ++i) {
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
            if (pipeline_command->pipeline_size > 1) {
                if (i % 2 == 0){
                    if (i < (pipeline_command->pipeline_size - 1)) {
                        dup2(fd1[1], 1);
                    }
                    if (i > 0) {
                        dup2(fd2[0], 0);
                    }
                } else if (i % 2 == 1) {
                    dup2(fd1[0], 0);
                    if (i < (pipeline_command->pipeline_size - 1)) {
                        dup2(fd2[1], 1);
                    }
                }
                close(fd1[0]);
                close(fd1[1]);
                close(fd2[0]);
                close(fd2[1]);
            }
            run_command(&(pipeline_command->pipeline_commands[i]));
            exit(0);
        }
    }
    close(fd1[1]);
    close(fd1[0]);
    if (pipeline_command->pipeline_size > 1) {
        close(fd2[1]);
        close(fd2[0]);  
    }
    while((do_wait = wait(&status)) != -1) {
        if (pid == do_wait) {
            ret_status = status;
        }
    }
    return ret_status;
}

int
run_command(Command * cur_command) {
    switch(cur_command->kind) {
        case KIND_SIMPLE: 
            do_simple(cur_command);
            break;
        case KIND_REDIRECT: 
            do_redirect(cur_command);
            break;
        case KIND_PIPELINE: 
            do_pipeline(cur_command);
            break;
        case KIND_SEQ1: 
            do_seq1(cur_command);
            break;
        case KIND_SEQ2: 
            do_seq2(cur_command);
            break;
        default:
            break;
    }
    return 0;
}

int
main(void) {
    return 0;
}