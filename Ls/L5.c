#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>


void
process_a(void) {
    int cnt_readed_byte;
    int buf_input[2];
    int buf_output[2];


    //Read and write 2 nums
    while ((cnt_readed_byte = read(STDIN_FILENO, &buf_input[0], 
        2 * sizeof(int))) == 2 * sizeof(int)) {
        buf_output[0] = buf_input[0] + buf_input[1];
        buf_output[1] = buf_input[0] - buf_input[1];
        write(STDOUT_FILENO, &buf_output[0], 2 * sizeof(int));
    }
}

void
process_son(int fd_read, int fd_write, int num1, int num2) {
    //Send to process A and get from process A two nums, then print it
    int buf[2];
    buf[0] = num1; 
    buf[1] = num2;

    write(fd_write, &buf[0], 2 * sizeof(int));
    close(fd_write);

    //Process A working...

    read(fd_read, &buf[0], 2 * sizeof(int));
    close(fd_read);

    printf("%d %d %d %d\n", num1, num2, buf[0], buf[1]);

}

int 
main (void) {
    setbuf(stdin, 0);
    int to_a[2];
    int from_a[2];
    pipe(to_a);
    pipe(from_a);

    if (fork() == 0) {
        //Process A
        dup2(to_a[0], STDIN_FILENO);
        dup2(from_a[1], STDOUT_FILENO);

        close(to_a[0]);
        close(to_a[1]);
        close(from_a[0]);
        close(from_a[1]);

        process_a();
        return 0;
    }

    close(from_a[1]);
    close(to_a[0]);


    pid_t pid;
    int num1, num2;

    while (scanf("%d%d", &num1, &num2) == 2) {
        while ((pid = fork()) == -1) {
            wait(NULL);
        }

        if (pid == 0) {
            process_son(from_a[0], to_a[1], num1, num2);
            return 0;
        }
        wait(NULL);
    }

    close(from_a[0]);
    close(to_a[1]);

    while(wait(NULL) != -1);

    return 0;
}



/*
    if (fork() == 0) {
        process_son(from_a[0], to_a[1], num1, num2);
    }
*/
/*

    write(to_a[1], &num1, sizeof(int));
    write(to_a[1], &num2, sizeof(int));

    close(to_a[1]);

    int num3, num4;

    read(from_a[0], &num3, sizeof(int));
    read(from_a[0], &num4, sizeof(int));

    close(from_a[0]);

    printf("main nums = %d %d\n", num3, num4);
*/