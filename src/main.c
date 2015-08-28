#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv){
    clock_t start, end;
    time_t startTime, endTime;
    double cpuTime;
    int i;
    
    end = clock();
    
    startTime = time(NULL); //wall clock
    start = clock(); //cpu 
    
    //for (i = 0; i < 1000000000; ++i)
    //    continue;

    
    endTime = time(NULL);
    end = clock();
    
    printf("WALL-CLOCK TIME: %f\n", difftime(endTime, startTime));
    cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("CPU time %f\n", cpuTime);

    return 0;
}
