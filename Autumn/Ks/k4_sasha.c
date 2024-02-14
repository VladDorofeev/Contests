#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[]) {
    int status;
    if (!fork()) {
        int file = open(argv[3], O_APPEND | O_CREAT | O_WRONLY, 0666);
        dup2(file, 1);
        close(file);
        if (!fork()) {
            execlp(argv[1], argv[1], NULL);
            exit(1);
        } else {
            wait(&status);
            if ((!WIFEXITED(status)) || (WEXITSTATUS(status))) {
                execlp(argv[2], argv[2], NULL);
                exit(1);
            }
            exit(0);
        }
    } else {
        wait(&status);
        if ((WIFEXITED(status)) && (!WEXITSTATUS(status))) {
            if (!fork()) {
                execlp(argv[4], argv[4], NULL);
                exit(1);
            } else {
                wait(&status);
            }
        }
    }
    return 0;
}