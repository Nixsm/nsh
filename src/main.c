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
    signal(signum, handler);
}

int main(int argc, char** argv){

    int signalCount = 0;
    int signo;
    int signum = SIGXCPU;

    sigset_t sigset;

    if((sigemptyset(&sigset) == -1) || (sigaddset(&sigset, signum) == -1) || (sigprocmask(SIG_BLOCK, &sigset, NULL) == -1 ))
        perror("Failed to block signals before sigwait\n");

    fprintf(stderr, "This process has ID %ld\n", (long)getpid());

    for ( ; ; ) {
        if (fork() == 0){
            for (int i = 0; i < 10000; ++i) {
                continue;
            }
        } else {
            printf("MEGA FOR LOOP RUNNING\n");
        }
        if (sigwait(&sigset, &signo) == -1) {
            perror("failed to wait using sigwait");
            return 1;
        }

        signalCount++;
        fprintf(stderr, "Number of signals so far: %d\n", signalCount);

        continue;
    }

    return 0;
    signal(SIGVTALRM, handler);
    signal(SIGALRM, handler);
    signal(SIGPROF, handler);
    signal(SIGXCPU, handler);


    alarm(2);

    timeTesting();
   
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
