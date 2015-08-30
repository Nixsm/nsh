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

int main(int argc, char** argv){
    char line[1024];
    char* username = getenv("USER");
    char curDir[1024];
    char** parsedArgs;
    
    while(1) {
        getcwd(curDir, sizeof(curDir));
        printf("%s -- %s >>> ", username, curDir); gets(line);

        if (feof(stdin)) {
            break;
        }
        
        makeArgs(line, " ", &parsedArgs);
        
        if (fork() == 0){
            if (strcmp(parsedArgs[0], "cd") == 0) {
                chdir(parsedArgs[1]);
            }else {
                execvp(parsedArgs[0], parsedArgs);
                printf("%s not found.\n", parsedArgs[0]);
            }
        }else {
            wait(NULL);
        }

        strcpy(line, "");
    }

    return 0;
    int size;
    char** result;
    int i;
    
    size = makeArgs("teste 123 x1", " ", &result);

    for (i = 0; i < size; ++i) {
        puts(result[i]);
    }
    
        //timeTesting();
    return 0;
   
    char *argexe[]={"ls", "-la", NULL};
    fork();
    fork();
    fork();
    fork();
    while(1){
        
        for (int i = 0; i < 10000000; ++i){
            continue;
        }
        //execvp(argexe[0], argexe);
        //        printf("did not worked\n");
        

    }
    
    return 0;
}
