/*
    gettime - get time via clock_gettime
    N.B.: OS X does not support clock_gettime

    Paul Krzyzanowski
*/

// gcc -o gettime main.cpp -lrt -std=c++11
#include <cstdio>  /* for printf */
#include <cstdint> /* for uint64 definition */
#include <cstdlib> /* for exit() definition */
#include <ctime>   /* for clock_gettime */
#include <unistd.h>
#include <sched.h>

#define BILLION 1000000000L
using namespace std;

int localpid(void) {
    static int a[9] = { 0 };
    return a[0];
}

int main(int argc, char **argv)
{
    uint64_t diff, diff2;
    struct timespec start, end, cpu_start, cpu_end;
    uint64_t i, j, count;
    int cpu;

    count = 0;


    /* measure monotonic time */
    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_start);    /* mark start time */

    for (i = 0; i < 199999; i++)
        for (j = 0; j < 199999; j++) {
            count++;
            if (count > 999999)
                count = 0;
        }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_end);      /* mark the end time */
    clock_gettime(CLOCK_MONOTONIC, &end);   /* mark the end time */
    
    cpu = sched_getcpu();
    printf("on cpu %d\n", cpu);

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

    diff2 = BILLION * (cpu_end.tv_sec - cpu_start.tv_sec) + cpu_end.tv_nsec - cpu_start.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) diff2);
    printf("cpu usage: %f\n\n", (double) diff2/diff);

    exit(0);
}