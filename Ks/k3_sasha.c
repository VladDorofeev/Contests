#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int
main(int argc, char *argv[]) {
    int file = open(argv[2], O_TRUNC | O_WRONLY | O_CREAT, 0666);
    if (!fork()) {
        dup2(file, 1);
        execlp(argv[1], argv[1], NULL);
        exit(1);
    }
    wait(NULL);
    close(file);
    return 0;
}