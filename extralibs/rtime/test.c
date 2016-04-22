#include <stdio.h>

#include "rtime.h"


int main (int argc, char **argv)
{
    timespec_t ti, now, next;

    ti.tv_nsec = 0;
    ti.tv_sec = time(NULL);

    clock_settime(CLOCK_REALTIME, &ti);
    fprintf(stderr, "time = %u\n", ti.tv_sec);
    
    for (;;) {
	clock_gettime(CLOCK_REALTIME, &now);
	clock_gettime(CLOCK_REALTIME, &next);
	fprintf(stdout, "Time = %u %u %u\n", 
		now.tv_sec, now.tv_nsec, next.tv_nsec);
	fflush(stdout);
    }

    exit(0);
}
