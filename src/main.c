#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "time_measures.h"

#include <signal.h>

void timeTesting() {
    clock_t begin, end;
    struct timeval tvBegin, tvEnd;
    double cpuTime, wallClockTime;
    int i;

    gettimeofday(&tvBegin, NULL);
    begin = clock();

    for (i = 0; i < 1000000000; ++i) {
        continue;
    }

    gettimeofday(&tvEnd, NULL);
    end = clock();

    cpuTime = getCPUTime(begin, end);
    wallClockTime = getWallClockTime(&tvBegin, &tvEnd);

    printf("CPU Time: %f\n", cpuTime);
    printf("Wall-clock Time: %f\n", wallClockTime);
}

void handler(int signum){
    printf("SIGNAL %d\n", signum);
}

int main(int argc, char** argv){
    timeTesting();

    char *argexe[]={"ls", "-la", NULL};

    while(1){
        signal(SIGVTALRM, handler);
        signal(SIGALRM, handler);
        signal(SIGPROF, handler);

        for (int i = 0; i < 10000000; ++i){
            continue;
        }
        //execvp(argexe[0], argexe);
        //        printf("did not worked\n");
        

    }
    
    return 0;
}
