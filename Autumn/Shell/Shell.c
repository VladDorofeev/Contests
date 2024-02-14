#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>


//Types of command
enum { 
    KIND_SIMPLE = 1,
    KIND_PIPELINE = 2, 
    KIND_REDIRECT = 3, 
    KIND_SEQ1 = 4, 
    KIND_SEQ2 = 5,
};

//Types of redirect 
enum {
    RD_INPUT = 1,//<
    RD_OUTPUT = 2,//>
    RD_APPEND = 3,//>>
};

//Types of operations in seq
enum {
    OP_CONJUNCT = 1,//&&
    OP_DISJUNCT = 2,//||
    OP_BACKGROUND = 3,//&
    OP_SEQ = 4,//;
};

struct Command
{
    int kind;

    union 
    {  
        //Simple command
        struct {
            char **argv;
            int argc;
        };

        //Pipeline command
        struct {
            int pipeline_size;
            struct Command *pipeline_commands;
        };
        
        //Redirect command
        struct {
            int rd_mode;
            char *rd_path;
            struct Command *rd_command;
        };
        
        //Sequence command
        struct {
            int seq_size;
            struct Command *seq_commands;
            int *seq_operations;
        };
    };
};

typedef struct Command Command;

int
run_command(Command *);

int
run_simple(Command *c) {
    execvp(c->argv[0], c->argv);
    exit(1);
}

int
run_redirect(Command *c) {
    int fd = -1;
    int stream = -1;
    switch (c->rd_mode)
    {
    case RD_INPUT:
        fd = open(c->rd_path, O_RDONLY);
        stream = STDIN_FILENO;
        break;
    case RD_OUTPUT:
        fd = open(c->rd_path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
        stream = STDOUT_FILENO;
        break;
    case RD_APPEND:
        fd = open(c->rd_path, O_CREAT | O_WRONLY | O_APPEND, 0666);
        stream = STDOUT_FILENO;
        break;
    default:
        break;
    }


    dup2(fd, stream);
    close(fd);
    run_command(c->rd_command);
    
    return 0;
}

pid_t
run_pipeline(Command *c) {
    pid_t pid = 0;
    int exit_code[2];
    pipe(exit_code);

    if (c->pipeline_size == 1) {
        if ((pid = fork()) == 0) {
            run_command(c->pipeline_commands);
            exit(0);
        }
        return pid;
    }
    
    int pipe_1[2];
    int pipe_2[2];

    pipe(pipe_1);
    pipe(pipe_2);

    for (int i = 0; i < c->pipeline_size; i++) {
        if (i % 2 == 0) {
            if (i != 0){
                close(pipe_1[0]);
                close(pipe_1[1]);
                pipe(pipe_1);
            }
        } else {
            close(pipe_2[0]);
            close(pipe_2[1]);
            pipe(pipe_2);
        }
        if ((pid = fork()) == 0) {
            if (i != 0) {
                dup2(i % 2 == 0 ? pipe_2[0] : pipe_1[0], STDIN_FILENO);
            }
            if (i != c->pipeline_size - 1) {
                dup2(i % 2 == 0 ? pipe_1[1] : pipe_2[1], STDOUT_FILENO);
            }
        
            close(pipe_1[0]);
            close(pipe_1[1]);
            close(pipe_2[0]);
            close(pipe_2[1]);

            run_command(&(c->pipeline_commands[i]));
            exit(0);
        }

    }
    close(pipe_1[0]);
    close(pipe_1[1]);
    close(pipe_2[0]);
    close(pipe_2[1]);
    return pid;
}

int
run_seq1(Command *c) {
    int status;
    pid_t cmd_pid;

    for (int i = 0; i < c->seq_size; i++) {
        status = 0;
        cmd_pid = run_command(&(c->seq_commands[i]));
        
        if (c->seq_commands == NULL) {
            break;
        }

        if (c->seq_commands[i].kind != KIND_SEQ2) {
            if (c->seq_operations[i] == OP_SEQ) {
                waitpid(cmd_pid, &status, 0);
            }
        } else {
            status = cmd_pid;
            if (c->seq_operations[i] == OP_SEQ) {
                waitpid(cmd_pid, &status, 0);
            }
        } 
    }

    return status;
}

int
run_seq2(Command *c) {
    int status;
    pid_t cmd_pid;
    
    int cmd_exit = 0;
    int need_run = 1;

    for (int i = 0; i < c->seq_size; i++) {
        if (need_run) {
            cmd_pid = run_command(&(c->seq_commands[i]));
        }

        if (need_run) {
            waitpid(cmd_pid, &status, 0);
            if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)) {
                cmd_exit = 0;
            } else {
                cmd_exit = 1;
            }
        }

        if (i != c->seq_size - 1) {
            switch (c->seq_operations[i])
            {
            case OP_CONJUNCT:
                if (cmd_exit == 0) {
                    need_run = 1;
                } else {
                    need_run = 0;
                }
                break;
            case OP_DISJUNCT:
                if (cmd_exit == 1) {
                    need_run = 1;
                } else {
                    need_run = 0;
                }
                break;
            default:
                break;
            }
        }
        
    }

    return status;
}


int
run_command(Command *c) {
    switch (c->kind)
    {
    case KIND_SIMPLE:
        return run_simple(c);
    case KIND_REDIRECT:
        return run_redirect(c);
    case KIND_PIPELINE:
        return run_pipeline(c);
    case KIND_SEQ1:
        return run_seq1(c);
    case KIND_SEQ2:
        return run_seq2(c);
    default:
        break;
    }
    return 0;
}

int
main(void) {
    return 0;
}

/*

int
main(void)
{
    // command "uname"
    Command c1_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 1,
        .argv = (char *[]){"uname", 0}
    };
    Command c1_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c1_1_1,
    };
    //run_command(&c1_1);

    // command "echo 1 2 3 > file && wc < file &"

    Command c2_1_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 4,
        .argv = (char *[]) {"echo", "1", "2", "3", 0}
    };
    Command c2_1_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_OUTPUT,
        .rd_path = "file",
        .rd_command = &c2_1_1_1
    };
    Command c2_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c2_1_1,
    };
    Command c2_2_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 1,
        .argv = (char *[]) {"wc", 0},
    };
    Command c2_2_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_INPUT,
        .rd_path = "file",
        .rd_command = &c2_2_1_1,
    };
    Command c2_2 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c2_2_1,
    };
    Command c2 = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []){c2_1, c2_2},
        .seq_operations = (int []){OP_CONJUNCT},
    };
    Command c2_0 = {
        .kind = KIND_SEQ1,
        .seq_size = 1,
        .seq_commands = &c2,
        .seq_operations = (int []){OP_BACKGROUND},
    };
    //run_command(&c2_0);

    // command "echo 1 2 3 | wc | wc | wc | wc"

    Command c3 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 5,
        .pipeline_commands = (Command []) {c2_1_1_1, c2_2_1_1, c2_2_1_1, c2_2_1_1, c2_2_1_1}
    };
    //run_command(&c3);

    // command "echo 1 >> file || echo 2 >> file && cat file"
    Command c4_1_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]){"echo", "1", 0},
    };
    Command c4_1_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_APPEND,
        .rd_path = "file",
        .rd_command = &c4_1_1_1,
    };
    Command c4_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c4_1_1,
    };
    Command c4_2_1_1 = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]){"echo", "2", 0},
    };
    Command c4_2_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_APPEND,
        .rd_path = "file",
        .rd_command = &c4_2_1_1,
    };
    Command c4_2 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c4_2_1,
    };
    Command c4_3_1 = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]){"cat", "file", 0},
    };
    Command c4_3 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c4_3_1,
    };
    Command c4 = {
        .kind = KIND_SEQ2,
        .seq_size = 3,
        .seq_commands = (Command []) {c4_1, c4_2, c4_3},
        .seq_operations = (int []){OP_DISJUNCT, OP_CONJUNCT},
    };
    //run_command(&c4);

    // command "echo 1 2 3 | wc > file; cat file"
    Command c5_1_1 = {
        .kind = KIND_REDIRECT,
        .rd_mode = RD_OUTPUT,
        .rd_path = "file",
        .rd_command = &c2_2_1_1,
    };
    Command c5_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 2,
        .pipeline_commands = (Command []) {c2_1_1_1, c5_1_1},
    };
    Command c5 = {
        .kind = KIND_SEQ1,
        .seq_size = 2,
        .seq_commands = (Command []) {c5_1, c4_3},
        .seq_operations = (int []){OP_SEQ, OP_SEQ},
    };
    //run_command(&c5);

    // command "echo 1 || (echo 2 && echo 3)"
    Command c6_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c4_1_1_1,
    };
    Command c6_2_1_1 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c4_2_1_1,
    };
    Command c6_2_1_2_1 = {
        .kind = KIND_SIMPLE,
        .argc = 2,
        .argv = (char *[]) {"echo", "3", 0},
    };
    Command c6_2_1_2 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c6_2_1_2_1,
    };
    Command c6_2_1 = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []) {c6_2_1_1, c6_2_1_2},
        .seq_operations = (int []) {OP_CONJUNCT},
    };
    Command c6_2 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 1,
        .pipeline_commands = &c6_2_1,   
    };
    Command c6 = {
        .kind = KIND_SEQ2,
        .seq_size = 2,
        .seq_commands = (Command []) {c6_1, c6_2},
        .seq_operations = (int []) {OP_DISJUNCT},
    };
    //run_command(&c6);

    // command "yes | head"
    Command c7_1 = {
        .kind = KIND_SIMPLE,
        .argc = 1,
        .argv = (char *[]) {"yes", 0},
    };
    Command c7_2 = {
        .kind = KIND_SIMPLE,
        .argc = 1,
        .argv = (char *[]) {"head", 0},
    };
    Command c7 = {
        .kind = KIND_PIPELINE,
        .pipeline_size = 2,
        .pipeline_commands = (Command []) {c7_1, c7_2},
    };
    //run_command(&c7);
    return 0;
}
*/