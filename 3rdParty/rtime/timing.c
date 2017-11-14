/* 
   Sets up a number of time counters for meassuring time.
   
   Unsafe
   
   Since the same thread that have started a measurement also should stop the 
   measurement, there is no need for this package to be MT-Safe.

   */

#include <limits.h>
#include "timing.h"


#ifdef DEBUG
#include <stdio.h>
#endif


/* ------------------------------------------------------------------ */


/* Number of meassurements to calculate the correction */
#define CORR_NR 10000


void TM_Init(tTM_Info *p)
{
    int j;

    /* Correcting for the time of measurement */
    timespecclear(&p->internal.corr);
    TM_Clear(p);

    for (j=0; j<CORR_NR; j++) {
	TM_Start(p);
	TM_Stop(p);
    }
    TM_Avg(p, &p->internal.corr);
    TM_Clear(p);

    p->internal.corr.tv_sec = p->internal.corr.tv_nsec = 0 ;
    
#ifdef DEBUG
    fprintf(stderr, "correction = %us %uns\n", 
	    p->internal.corr.tv_sec, p->internal.corr.tv_nsec);
#endif
}


void TM_Clear(tTM_Info *p)
{
    timespecclear(&p->total);
    timespecclear(&p->last);
    timespecclear(&p->max);
    p->min.tv_sec = LONG_MAX;
    p->min.tv_nsec = LONG_MAX;
    p->nr = 0;
}

void TM_Start(tTM_Info *p)
{
    clock_gettime(CLOCK_REALTIME, &p->internal.start);
}
		
void TM_Stop(tTM_Info *p)
{
    timespec_t now, time;

    clock_gettime(CLOCK_REALTIME, &now);
    /* time = now - start - corr */
    timespecdiff(&time, &now, &p->internal.start);
    timespecdiff(&time, &time, &p->internal.corr);
    /* total += t */
    timespecadd(&p->total, &p->total, &time);

    p->last = time;
    if (timespeccmp(&time, &p->min, < ))
	p->min = time;
    if (timespeccmp(&time, &p->max, > ))
	p->max = time;
    (p->nr)++;
}
		
void TM_Avg(tTM_Info *p, timespec_t *avg)
{
    if (!avg) return;
    avg->tv_sec = p->total.tv_sec / p->nr;
    avg->tv_nsec = (p->total.tv_sec % p->nr) * (NANOSEC / p->nr) + 
	(p->total.tv_nsec / p->nr);
}






