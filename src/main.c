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

void timeTesting() {
    clock_t begin, end;
    struct timeval tvBegin, tvEnd;
    double cpuTime, wallClockTime;
    int i;
    struct rusage usage;
    
    gettimeofday(&tvBegin, NULL);
    begin = clock();

    for (i = 0; i < 1000000000; ++i) {
        continue;
    }

    gettimeofday(&tvEnd, NULL);
    end = clock();

    cpuTime = getCPUTime(begin, end);
    wallClockTime = getWallClockTime(&tvBegin, &tvEnd);

    getrusage(RUSAGE_SELF, &usage);

    printf("Voluntary context Switches %ld\n", usage.ru_nvcsw);
    printf("Involuntary context switches %ld\n", usage.ru_nivcsw);
    printf("SYSTEM CPU Time: %f\n", timevalToMilliseconds(&usage.ru_stime));
    printf("USER CPU Time: %f\n", timevalToMilliseconds(&usage.ru_utime));
    
    
    printf("CPU Time: %f\n", cpuTime);
    printf("Wall-clock Time: %f\n", wallClockTime);
}

void sigHandler(int sig) {
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

    signal(SIGINT, sigHandler);

    fputs("NSH VERSION 0.0.0.0.1\n", stdout);
    
    while(1) {
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
        
        free(parsedArgs);
        strcpy(line, "");
    }

    fputs("\nGood bye, please come again!", stdout);

    return 0;
}
