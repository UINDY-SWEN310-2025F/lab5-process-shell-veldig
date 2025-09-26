#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // create a new process
    pid = fork();

    if (pid < 0) {
        // error in fork
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } else if (pid == 0) {
        // child process
        // use exec to run "ls -lt"
        execlp("ls", "ls", "-lt", (char *)NULL);

        // if exec fails
        perror("exec failed");
        exit(1);
    } else {
        // parent process
        // wait for child to finish
        wait(NULL);
        printf("Child process is done\n");
    }

    return 0;
}
