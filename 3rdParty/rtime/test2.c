#include <stdio.h>
#include <stdlib.h>
#include "rtime.h"


int main (int argc, char **argv)
{
    int i;
    timespec_t ti1, ti2;


    clock_getres(CLOCK_REALTIME, &ti1);
    fprintf(stdout, "Time = %u %u\n", ti1.tv_sec, ti1.tv_nsec);
    for (i=0; i<1000; i++) {
	clock_gettime(CLOCK_REALTIME, &ti1);
	clock_gettime(CLOCK_REALTIME, &ti2);
	fprintf(stdout, "Time = %u %u %u\n", 
		ti1.tv_sec, ti1.tv_nsec, ti2.tv_nsec);
    }
    exit(0);
}
