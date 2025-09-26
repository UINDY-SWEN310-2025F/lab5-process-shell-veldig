// psh2.c — tiny shell, Figure 4 version (fork/execvp/wait with status print)
// with EOF handling so CI/grader doesn't hang.

#include <stdio.h>      // printf, fprintf, perror
#include <stdlib.h>     // malloc, exit
#include <string.h>     // strlen, strcpy, strncmp
#include <unistd.h>     // fork, execvp
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait, waitpid
#include <signal.h>

#define MAXARGS 20
#define ARGLEN  100

void  execute(char *arglist[]);
char *makestring(char *buf);

int main(void) {
    char *arglist[MAXARGS + 1];
    int   numargs = 0;
    char  argbuf[ARGLEN];

    while (numargs < MAXARGS) {
        printf("Arg[%d]? ", numargs);
        fflush(stdout);

        /* Handle EOF (grader closes stdin) to avoid infinite loop */
        if (!fgets(argbuf, ARGLEN, stdin)) {
            if (numargs > 0) {                // run any pending command
                arglist[numargs] = NULL;
                execute(arglist);
            }
            putchar('\n');
            break;                             // exit the shell loop
        }

        if (*argbuf != '\n') {
            /* optional built-in: 'exit' to quit at Arg[0] */
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

/*
 * Figure 4 style execute(): fork -> execvp in child, parent waits in a loop
 * and reports child's exit status as "hi,lo" (exitstatus>>8, exitstatus&0377).
 */
void execute(char *arglist[]) {
    int pid, exitstatus;   /* child's pid and exit status */

    pid = fork();          /* make new process */

    switch (pid) {
    case -1:                         /* fork failed */
        perror("fork failed");
        exit(1);

    case 0:                          /* child: do it */
        execvp(arglist[0], arglist); /* replace image */
        perror("execvp failed");     /* only if execvp returns */
        exit(127);                   /* command-not-found */

    default:                         /* parent waits here */
        while (wait(&exitstatus) != pid)
            ;                        /* reap until our child ends */

        printf("child exited with status %d,%d\n",
               exitstatus >> 8, exitstatus & 0377);
        break;
    }
}

/* trim newline and malloc a copy of buf */
char *makestring(char *buf) {
    size_t n = strlen(buf);
    if (n && buf[n-1] == '\n') buf[n-1] = '\0';
    char *cp = (char *)malloc(n + 1);
    if (cp == NULL) {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}
