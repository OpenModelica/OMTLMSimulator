#ifndef TIMING_H
#define TIMING_H

#include "rtime.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef OMATH
#define TM_Init     om_TimeInit 
#define TM_Clear    om_TimeClear
#define TM_Start    om_TimeStart
#define TM_Stop     om_TimeStop 
#define TM_Tot      om_TimeTot
#define TM_Nr       om_TimeNr
#define TM_Min      om_TimeMin
#define TM_Max      om_TimeMax
#define TM_Last     om_TimeLast
#endif

#define TM_Tot(p) ((p)->total)	/* Total time */
#define TM_Nr(p) ((p)->nr)	/* Number of measurements */
#define TM_Min(p) ((p)->min)	/* Shortest measurement */
#define TM_Max(p) ((p)->max)	/* Longest measurement */
#define TM_Last(p) ((p)->last)	/* Last measurement */

typedef struct tTM_Info {
    struct {
	timespec_t start;
	timespec_t corr;
    } internal;
    timespec_t total;
    timespec_t last;
    timespec_t min;
    timespec_t max;
    unsigned long nr;
} tTM_Info;

/* Calls TM_Clear. Calculates the correction */
void TM_Init(tTM_Info *p);

/* Clears all fields of the tTM_Info struct */
void TM_Clear(tTM_Info *p);

/* Starts a time measurement */
void TM_Start(tTM_Info *p);

/* Stops the measurement */
void TM_Stop(tTM_Info *p);

/* Calculates the average of all measurements since last call to TM_Clear */
void TM_Avg(tTM_Info *p, timespec_t *avg);

#ifdef __cplusplus
}
#endif


#endif


