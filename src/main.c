#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "makeargs.h"
#include "time_measures.h"
#include <sys/resource.h>

#include <signal.h>

#include "job_list.h"

#define QUIT_PHRASE "exit"
#define LINE_SIZE 1024
#define BACKGROUND_SYMBOL '&'


/* Handles the SIGINT */
void sigHandler(int sig) {
    printf("\r");
    signal(sig, sigHandler);
}

int main(int argc, char** argv){
    Jobs job = createList();

    add(&job, 0, "ls", FOREGROUND);

    //showJobs(&job);


    return 0;
    char line[LINE_SIZE];
    char* username;
    char curDir[1024];
    char** parsedArgs;
    char promptMsg[LINE_SIZE];
    int len;
    int argsLen;
    struct timeval wallClockBegin;
    struct rusage usage;
    int returnStatus;
    int background;

    fputs("NSH VERSION 0.0.0.0.1\n", stdout);
    
    while(1) {
        signal(SIGINT, sigHandler);

        getcwd(curDir, sizeof(curDir));
        username = getenv("USER");
        
        sprintf(promptMsg, "%s -- %s >>> ", username, curDir);
        
        if (fputs(promptMsg, stdout) == EOF) { /* check for errors at fputs */
            continue;
        }
        
        if (fgets(line, LINE_SIZE, stdin) == NULL) { /* if our fgets finds a EOF, continue */
            break;
        }

        len = strlen(line); /* get the lenght of the line to remove the last \n */

        if (line[len - 1] == '\n') { /* removes the \n from fgets */
            line[len - 1] = '\0';
        }

        if (strcmp(line, QUIT_PHRASE) == 0){ /* check if the current command isnt our exit command */
            break;
        }

        if (strchr(line, BACKGROUND_SYMBOL) != 0) {
            line[len - 2] = '\0';
            background = 1;
        } else {
            background = 0;
        }

        argsLen = makeArgs(line, " ", &parsedArgs);

        if (argsLen > 0){
            if (strcmp(parsedArgs[0], "cd") == 0) {
                chdir(parsedArgs[1]);
            }else{
                gettimeofday(&wallClockBegin, NULL);
                getrusage(RUSAGE_CHILDREN, &usage);
                if (fork() == 0){
                    execvp(parsedArgs[0], parsedArgs);
                    printf("nsh: command not found: %s\n", parsedArgs[0]);
                    return 1; /* for some strange reason this will return 256 later */
                }
                if (!background) {
                    waitpid(-1, &returnStatus, 0);
                    if (returnStatus != 256) {
                        printStatitics(&wallClockBegin, &usage); //Print statics when child return
                    }
                }
                while(waitpid(-1, &returnStatus, WNOHANG) > 0);
            }
        }
        
        free(parsedArgs);
        strcpy(line, "");
    }

    fputs("\nGood bye, please come again!", stdout);

    return 0;
}
