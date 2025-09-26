// psh1.c  — minimal tiny shell (Figure 3 version)

#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h>   
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

int execute(char *arglist[]) {
    pid_t pid = fork();
    if (pid < 0) {                    
        perror("fork");
        return -1;
    } else if (pid == 0) {            
        execvp(arglist[0], arglist);  
        perror("execvp");            
        _exit(127);
    } else {                         
        int status = 0;
        if (waitpid(pid, &status, 0) < 0) {
            perror("waitpid");
            return -1;
        }
        if (WIFEXITED(status)) return WEXITSTATUS(status);
        return -1;
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
