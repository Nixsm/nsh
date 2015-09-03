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

#define QUIT_PHRASE "exit"
#define LINE_SIZE 1024
#define BACKGROUND_SYMBOL "&"


/* Handles the SIGINT */
void sigHandler(int sig) {
  printf("ctrl-c received");
  printf("\r");
  signal(sig, sigHandler);

}

int main(int argc, char** argv){

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

        argsLen = makeArgs(line, " ", &parsedArgs);

        if (argsLen > 0){
            if (strcmp(parsedArgs[0], "cd") == 0) {
                chdir(parsedArgs[1]);
            }else{
	      if (strcmp(parsedArgs[argsLen - 1], BACKGROUND_SYMBOL) == 0) {
		puts("Tried to run in background");
		parsedArgs[argsLen - 1];
		if (fork() == 0){ //Creates a process that never "returns"
		  if (fork() == 0) {
		    printf("Process %d started!\n", getpid());
		    execvp(parsedArgs[0], parsedArgs);


		    return 1;
		  } else {
		    int pid = wait(NULL);
		    printf("Process with id %d finished!\n", pid);

		    return 1;
		  }
		}
	      }else{
		gettimeofday(&wallClockBegin, NULL);
                getrusage(RUSAGE_CHILDREN, &usage);
                if (fork() == 0){
		  execvp(parsedArgs[0], parsedArgs);
		  printf("nsh: command not found: %s\n", parsedArgs[0]);
		  return 1; /* for some strange reason this will return 256 later */
                }
                waitpid(-1, &returnStatus, 0);
                if (returnStatus != 256) {
		  printStatitics(&wallClockBegin, &usage); //Print statics when child return
                }
	      }
            }
        }
        
        free(parsedArgs);
        strcpy(line, "");
    }

    fputs("\nGood bye, please come again!", stdout);

    return 0;
}
