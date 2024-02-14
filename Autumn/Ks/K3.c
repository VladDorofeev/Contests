#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

int 
main (int argc, char **argv) {
    argc--;
    argv++;

    char *prog = *argv;
    char *file_to_write = *(argv + 1);

    if (fork() == 0) {
        int fd = creat(file_to_write, 0666);
        dup2(fd, 1);
        close(fd);
        execlp(prog, prog, (char *)NULL);
        return 0;
    }
    wait(NULL);
    return 0;
}