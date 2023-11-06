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
    char *file_to_read = *(argv + 1);

    if (fork() == 0) {
        int fd = open(file_to_read, O_RDONLY);
        dup2(fd, 0);
        close(fd);
        execlp(prog, prog, (char *)NULL);
        return 0;
    }
    wait(NULL);
    return 0;
}