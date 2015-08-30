#include "time_measures.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

int timevalSubtract(OUT struct timeval *result, IN struct timeval *t2, IN struct timeval *t1) {
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return diff < 0;
}

double timevalToMilliseconds(IN struct timeval* convertMe) {
    char time[100];
    double timeInMilliseconds;
    
    sprintf(time, "%ld.%06ld", (long int)convertMe->tv_sec, (long int)convertMe->tv_usec);

    timeInMilliseconds = atof(time); /* Convert to double */

    timeInMilliseconds *= 1000; /* Convert to Milliseconds */

    return timeInMilliseconds;
}

double getCPUTime(IN clock_t start, IN clock_t end) {
    double cpuTime;

    cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC; /* Calculate time in seconds with high precision */

    cpuTime *= 1000; /* Convert to milliseconds */

    return cpuTime;
}

double getWallClockTime(IN struct timeval* begin, IN struct timeval* end) {
    struct timeval result;
    double wallClockTime;
    
    timevalSubtract(&result, end, begin); /* Get difference between those two times */

    wallClockTime = timevalToMilliseconds(&result); /* Convert to milliseconds */


    return wallClockTime;
}

void printStatitics(IN struct timeval* wallClockBegin, IN struct rusage* usage) {
    struct rusage usageAfter;
    struct timeval wEnd;
    double wallTime;
    struct timeval cpuTime;
    struct timeval sysTime;

    gettimeofday(&wEnd, NULL);

    getrusage(RUSAGE_CHILDREN, &usageAfter);

    timevalSubtract(&cpuTime, &usageAfter.ru_utime, &usage->ru_utime);
    timevalSubtract(&sysTime, &usageAfter.ru_stime, &usage->ru_stime);

    wallTime = getWallClockTime(wallClockBegin, &wEnd);

    puts("");
    puts("Statics:");
    printf("Voluntary context switches: %ld\n", usageAfter.ru_nvcsw - usage->ru_nvcsw);
    printf("Involuntary context switches: %ld\n", usageAfter.ru_nivcsw - usage->ru_nivcsw);
    printf("System CPU time used: %f ms\n", timevalToMilliseconds(&sysTime));
    printf("User CPU time used: %f ms\n", timevalToMilliseconds(&cpuTime));
    printf("Wall clock time: %f ms\n", wallTime);
}
