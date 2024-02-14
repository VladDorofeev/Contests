#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int
main(int argc, char *argv[]) {
    int file = open(argv[2], O_RDONLY);
    if (!fork()) {
        dup2(file, 0);
        execlp(argv[1], argv[1], NULL);
        exit(1);
    }
    wait(NULL);
    close(file);
    return 0;
}