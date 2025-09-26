// psh1.c — minimal tiny shell (Figure 3 version) with EOF handling

#include <stdio.h>      // printf, fprintf, perror
#include <stdlib.h>     // malloc, exit
#include <string.h>     // strlen, strcpy, strncmp
#include <unistd.h>     // fork, execvp
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid, WIFEXITED
#include <signal.h>

#define MAXARGS 20
#define ARGLEN  100

int  execute(char *arglist[]);
char *makestring(char *buf);

int main(void) {
    char *arglist[MAXARGS + 1];
    int   numargs = 0;
    char  argbuf[ARGLEN];

    while (numargs < MAXARGS) {
        printf("Arg[%d]? ", numargs);
        fflush(stdout);

        /* Handle EOF cleanly (grader closes stdin) */
        if (!fgets(argbuf, ARGLEN, stdin)) {
            if (numargs > 0) {                // run any pending command
                arglist[numargs] = NULL;
                execute(arglist);
            }
            putchar('\n');
            break;                             // exit the shell loop
        }

        if (*argbuf != '\n') {
            /* Optional built-in: allow 'exit' to quit at Arg[0] */
            if (numargs == 0 && strncmp(argbuf, "exit", 4) == 0) {
                break;
            }
            arglist[numargs++] = makestring(argbuf);
        } else {
            if (numargs > 0) {
                arglist[numargs] = NULL;       // NULL-terminate argv
                execute(arglist);              // run command
                numargs = 0;                   // reset for next cmd
            }
        }
    }
    return 0;
}

int execute(char *arglist[]) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        execvp(arglist[0], arglist);
        perror("execvp");                      // only runs if exec fails
        _exit(127);
    } else {
        int status = 0;
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            return -1;
        }
        if (WIFEXITED(status)) return WEXITSTATUS(status);
        return -1;                             // non-normal termination
    }
}

char *makestring(char *buf) {
    size_t n = strlen(buf);
    if (n && buf[n-1] == '\n') buf[n-1] = '\0';   // trim newline
    char *cp = (char *)malloc(n + 1);
    if (cp == NULL) {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}
