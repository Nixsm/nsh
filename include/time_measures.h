#ifndef TIME_MEASURES_H
#define TIME_MEASURES_H

#include <time.h>
#include <sys/time.h>
#include "config.h"

/* Stolen from GNU
 * @param result output of subtraction
 * @param t2 end time to subtract
 * @param t1 begin time to subtract
 * @return int 1 if the difference is negative, otherwise 0.
 * @see http://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_21.html
 */
int timevalSubtract(OUT struct timeval* result, IN struct timeval* t2, IN struct timeval* t1);

/* 
 * Converts a timeval to milliseconds
 * @param convertMe input timeval to be converted
 * @return double wall clock time in milliseconds
 */
double timevalToMilliseconds(IN struct timeval* convertMe);

/*
 * Calculates the CPU time in milliseconds
 * @param begin begin time
 * @param end ending time
 * @return double cpu time in milliseconds
 */
double getCPUTime(IN clock_t begin, IN clock_t end);

/*
 * Calculates the Wall clock time in milliseconds
 * @param begin begin time
 * @param end end time
 * @return double wall clock time in milliseconds
 * @see timeValToMilliseconds
 * @see timevalSubtract
 */
double getWallClockTime(IN struct timeval* begin, IN struct timeval* end);

#endif//TIME_MEASURES_H
