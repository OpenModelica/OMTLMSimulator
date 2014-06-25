/* 

   This package uses the RTCU and RTCL registers in PowerPC 601 to meassure
   time. Note: Other PowerPC's don't have these registers.
   
   The resolution of the RTCU is 1 second and of the RTCL is 128
   nanoseconds.
   
   It wraps around after 2^32 seconds = ca. 136 years.
   
   
   Safe 

   The PARIX-1.3 call TimeNow() also use the information in these
   registers but it truncates the result to microseconds. Also, since
   the result is returned in a unsigned int, it wraps around after
   ca. 71 minutes.
*/

#include "rtime.h"

#ifdef __PARIX__

#include <errno.h>
#include <sys/sem.h>

static Semaphore_t sem = {1, 0, 0};
static volatile timespec_t Diff;

#endif

/* ---------------------------------------------------------------- */

void timespecadd(timespec_t *sum, timespec_t *t1, timespec_t *t2)
{
    sum->tv_nsec = t1->tv_nsec + t2->tv_nsec;
    sum->tv_sec  = t1->tv_sec  + t2->tv_sec;
    if (sum->tv_nsec > NANOSEC) {
	sum->tv_nsec -= NANOSEC;
	sum->tv_sec ++;
    }
}

void timespecdiff(timespec_t *diff, timespec_t *t1, timespec_t *t2)
{
    diff->tv_nsec = t1->tv_nsec - t2->tv_nsec;
    diff->tv_sec  = t1->tv_sec  - t2->tv_sec;
    if (diff->tv_nsec < 0) {
	diff->tv_nsec += NANOSEC;
	diff->tv_sec --;
    }
}

#ifdef __PARIX__
int clock_settime(clockid_t clock_id, timespec_t *tp)
{
    timespec_t diff, now;

    if (clock_id != CLOCK_REALTIME ||
	tp->tv_nsec < 0 || tp->tv_nsec >= NANOSEC) {
	errno = EINVAL;
	return -1;
    }
    rtctime(&now);
    timespecdiff(&diff, tp, &now);
    Wait(&sem);
    Diff = diff;
    Signal(&sem);

    return 0;
}

int clock_gettime(clockid_t clock_id, timespec_t *tp)
{
    timespec_t now, diff;

    if (clock_id != CLOCK_REALTIME) {
	errno = EINVAL;
	return -1;
    }
    Wait(&sem);
    diff = Diff;
    Signal(&sem);
    rtctime(&now);
    timespecadd(tp, &diff, &now);
    
    return 0;
}

int clock_getres(clockid_t clock_id, timespec_t *res)
{
    if (clock_id != CLOCK_REALTIME) {
	errno = EINVAL;
	return -1;
    }
    res->tv_sec = 0;
    res->tv_nsec = 128;
    
    return 0;
}

#endif


#ifdef _UNICOS
/** 
     Added Patrik Nordling in the year of the horse......
     Based on gettimeofday(2)
 **/

#include <errno.h>

int clock_settime(clockid_t clock_id, timespec_t *tp)
{
    struct timeval t ;
    if (clock_id != CLOCK_REALTIME ||
	tp->tv_nsec < 0 || tp->tv_nsec >= NANOSEC) {
        errno = EINVAL;
	return -1;
    }
    if(gettimeofday(&t, NULL)){
        perror("gettimeofday failed") ; }
    tp->tv_sec = t.tv_sec ;
    tp->tv_nsec = t.tv_usec*1000 ;
    return 0;
}

int clock_gettime(clockid_t clock_id, timespec_t *tp)
{
    struct timeval t ;
    if (clock_id != CLOCK_REALTIME) {
	errno = EINVAL;
	return -1;
    }
    if(gettimeofday(&t, NULL)){
        perror("gettimeofday failed") ; }    
    tp->tv_sec = t.tv_sec ;
    tp->tv_nsec = t.tv_usec*1000 ;
    return 0;
}

int clock_getres(clockid_t clock_id, timespec_t *res)
{
    if (clock_id != CLOCK_REALTIME) {
	errno = EINVAL;
	return -1;
    }
    res->tv_sec = 0;
    res->tv_nsec = 128;
    return 0;
}

#endif

/*#if defined (__linux__) || defined (__CYGWIN__)*/
/* Alex: Linux-2.2 implementes clock_settime in time.h*/
/* Hakan: But need to link -lrt  */

#if defined (__CYGWIN__)

#include <errno.h>
#include <stdlib.h>

int clock_settime(clockid_t clock_id, timespec_t *tp)
{
    struct timeval t ;
    if (clock_id != CLOCK_REALTIME ||
	tp->tv_nsec < 0 || tp->tv_nsec >= NANOSEC) {
        errno = EINVAL;
	return -1;
    }
    if(gettimeofday(&t, NULL)){
        perror("gettimeofday failed") ; }
    tp->tv_sec = t.tv_sec ;
    tp->tv_nsec = t.tv_usec*1000 ;
    return 0;
}

int clock_gettime(clockid_t clock_id, timespec_t *tp)
{
    struct timeval t ;
    if (clock_id != CLOCK_REALTIME) {
	errno = EINVAL;
	return -1;
    }
    if(gettimeofday(&t, NULL)){
        perror("gettimeofday failed") ; }    
    tp->tv_sec = t.tv_sec ;
    tp->tv_nsec = t.tv_usec*1000 ;
    return 0;
}

int clock_getres(clockid_t clock_id, timespec_t *res)
{
    if (clock_id != CLOCK_REALTIME) {
	errno = EINVAL;
	return -1;
    }
    res->tv_sec = 0;
    res->tv_nsec = 128;
    return 0;
}

#endif


#if defined (_MSC_VER) || defined(__MINGW32__)

/* 
   New approach for MSC using clock()
   clock() returns the proces time 
   since process start.
   A.Siemers 00.01.25
 */

#include <time.h>
#include <winsock.h>
#include <errno.h>

int clock_settime(clockid_t clock_id, timespec_t *tp)
{
    clock_t secs;

    if (clock_id != CLOCK_REALTIME ||
	tp->tv_nsec < 0 || tp->tv_nsec >= NANOSEC) {
        errno = EINVAL;
	return -1;
    }

    secs = clock();

//    secs = secs / CLOCKS_PER_SEC;
//    tp->tv_sec = secs;
//    tp->tv_nsec = secs*1000;

    tp->tv_sec = secs / CLOCKS_PER_SEC;
    tp->tv_nsec = (secs % CLOCKS_PER_SEC)*(NANOSEC/CLOCKS_PER_SEC);


    return 0;
}

int clock_gettime(clockid_t clock_id, timespec_t *tp)
{
    clock_t secs;

    if (clock_id != CLOCK_REALTIME) {
	errno = EINVAL;
	return -1;
    }

    secs = clock();
//    secs = secs / CLOCKS_PER_SEC;
//    tp->tv_sec = secs;
//    tp->tv_nsec = secs*1000;

    tp->tv_sec = secs / CLOCKS_PER_SEC;
    tp->tv_nsec = (secs % CLOCKS_PER_SEC)*(NANOSEC/CLOCKS_PER_SEC);

    return 0;
}

int clock_getres(clockid_t clock_id, timespec_t *res)
{
    if (clock_id != CLOCK_REALTIME) {
	errno = EINVAL;
	return -1;
    }
    res->tv_sec = 0;
//    res->tv_nsec = 128;
    res->tv_nsec= (NANOSEC/CLOCKS_PER_SEC);
    return 0;
}

#endif
