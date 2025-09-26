// psh2.c — tiny shell, Figure 4 version (fork/execvp/wait with status print)

#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>    
#include <unistd.h>    
#include <sys/types.h>  
#include <sys/wait.h>  
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

        if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n') {
            arglist[numargs++] = makestring(argbuf);
        } else {
            if (numargs > 0) {          
                arglist[numargs] = NULL;     
                execute(arglist);           
                numargs = 0;                 
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
    int pid, exitstatus;   


    pid = fork();

    switch (pid) {
    case -1:                         
        perror("fork failed");
        exit(1);

    case 0:                          
        execvp(arglist[0], arglist); 
        perror("execvp failed");     
        exit(127);                 

    default:                         
        while (wait(&exitstatus) != pid)
            ;                        

        printf("child exited with status %d,%d\n",
               exitstatus >> 8, exitstatus & 0377);
        break;
    }
}

char *makestring(char *buf) {
    size_t n = strlen(buf);
    if (n && buf[n-1] == '\n') buf[n-1] = '\0';
    char *cp = malloc(n + 1);
    if (cp == NULL) {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}
