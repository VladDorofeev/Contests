#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>


int 
main (int argc, char **argv) {
    argc--;
    argv++;
    if (argc < 4) {
        return 0;
    }
    char *prog1 = *argv;
    char *prog2 = *(argv + 1);
    char *file_to_write = *(argv + 2);
    char *prog3 = *(argv + 3);
    int status;

    if (fork() == 0) {
        //Change stdout to our file
        int fd = open(file_to_write, O_CREAT|O_WRONLY|O_APPEND, 0666);
        dup2(fd, 1);
        close(fd);
        
        //Execute prog1
        if (fork() == 0) {
            execlp(prog1, prog1, (char *)NULL);
            return 1;
        }
        
        //Check result of running prog1
        wait(&status);
        if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)) {
            return 0;
        }

        //If we have bad exit
        if (fork() == 0) {
            execlp(prog2, prog2, (char *)NULL);
        }
        wait(&status);
        if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)) {
            return 0;
        } else {
            return 1;
        }
    }

    //Check result of runnning (pr1 || pr2)
    wait(&status);
    if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)) {
        //If we have good exit
        if (fork() == 0) {
            execlp(prog3, prog3, (char *)NULL);
            return 1;
        }
        wait(NULL);
    }
    return 0;
}