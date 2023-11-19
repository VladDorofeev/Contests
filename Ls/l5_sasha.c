#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

void
do_process_a(void) {
    int nums[2];
    int calculations[2];
    while(read(0, &nums[0], 2 * sizeof(int)) == 2 * sizeof(int)) {
        calculations[0] = nums[0] + nums[1];
        calculations[1] = nums[0] - nums[1];
        write(1, &calculations[0], 2 * sizeof(int));
    }
}

void
do_son_process(int fd,int nums[]) {
    int caclilations[2];
    write(fd, &nums[0], 2 * sizeof(int));
    read(0, &caclilations[0], 2 * sizeof(int));
    printf("%d %d %d %d\n", nums[0], nums[1], caclilations[0], caclilations[1]);
}

int
main(void) {
    setbuf(stdin, 0);
    int fd_nums_to_a[2];
    int fd_calc_from_a[2];
    pipe(fd_nums_to_a);
    pipe(fd_calc_from_a);
    //process A
    if (fork() == 0) {
        close(fd_nums_to_a[1]);
        close(fd_calc_from_a[0]);
        dup2(fd_nums_to_a[0], 0);
        dup2(fd_calc_from_a[1], 1);
        close(fd_nums_to_a[0]);
        close(fd_calc_from_a[1]);
        do_process_a();
        exit(0);
    }
    //other sons
    int fd_ban_all_son[2];
    int two_nums[2];
    pid_t pid;
    pipe(fd_ban_all_son);
    char open_byte = 1;
    write(fd_ban_all_son[1], &open_byte, sizeof(char));
    close(fd_nums_to_a[0]);
    close(fd_calc_from_a[1]);
    //sons
    while(scanf("%d%d", &two_nums[0], &two_nums[1]) == 2) {
        while((pid = fork()) == -1) {
            wait(NULL);
        }
        if (pid == 0) {
            dup2(fd_calc_from_a[0], 0);
            close(fd_calc_from_a[0]);
            //if process can read then it can start and other sons blocks 
            read(fd_ban_all_son[0], &open_byte, sizeof(char));
            do_son_process(fd_nums_to_a[1] ,two_nums);
            //process done then one of the others son processes can start
            write(fd_ban_all_son[1], &open_byte, sizeof(char));
            close(fd_nums_to_a[1]);
            close(fd_ban_all_son[0]);
            close(fd_ban_all_son[1]);
            exit(0);
        }
    }
    close(fd_nums_to_a[1]);
    close(fd_calc_from_a[0]);
    close(fd_ban_all_son[0]);
    close(fd_ban_all_son[1]);
    while(wait(NULL) != -1) {
    }
    return 0;
}