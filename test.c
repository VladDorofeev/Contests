#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>


int
main(void)
{
    int fd[2];
    int fd_2[2];
    pipe(fd);
    pipe(fd_2);

    size_t int_s = sizeof(int);

    if (fork() == 0) {
        //pipe_1[read]
        //pipe_2[write]
        dup2(fd[0], STDIN_FILENO);
        dup2(fd_2[1], STDOUT_FILENO);

        close(fd[0]);
        close(fd[1]);
        close(fd_2[0]);
        close(fd_2[1]);

        int num;
        while(read(STDIN_FILENO, &num, int_s) == int_s) {
            write(STDOUT_FILENO, &num, int_s);
        }
        return 0;
    }
    
    int num = 1;

    //Unused pipe
    close(fd[0]);
    close(fd_2[1]);

    //Write to process A
    write(fd[1], &num, int_s);
    close(fd[1]);


    sleep(1);


    //Read from process A
    read(fd_2[0], &num, int_s);
    printf("main num = %d\n", num);
    close(fd_2[0]);


    while(wait(NULL) != -1);
    return 0;
}