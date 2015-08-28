#include "time_measures.h"
#include <stdio.h>
#include <stdlib.h>

int timevalSubtract(OUT struct timeval *result, IN struct timeval *t2, IN struct timeval *t1) {
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return diff < 0;
}

double timevalToMilliseconds(IN struct timeval* convertMe) {
    char time[10];
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
