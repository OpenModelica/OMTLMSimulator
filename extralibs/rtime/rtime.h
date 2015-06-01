#include <time.h>   

#ifndef RTIME_H
#define RTIME_H

#ifdef __cplusplus
extern "C" {
#endif

/* timespeccmp does not work for >= or <= */
#define timespeccmp(tvp, uvp, cmp) \
        ((tvp)->tv_sec cmp (uvp)->tv_sec || \
        ((tvp)->tv_sec == (uvp)->tv_sec && \
        (tvp)->tv_nsec cmp (uvp)->tv_nsec))
 
#define timespecclear(tvp)         (tvp)->tv_sec = (tvp)->tv_nsec = 0

#define timespecisset(tvp)         ((tvp)->tv_sec || (tvp)->tv_usec)

#ifdef _AIX32
#define _POSIX_SOURCE
#include <sys/types.h>
#include <sys/timers.h>
#undef _POSIX_SOURCE
#define NANOSEC NS_PER_SEC
#define clock_settime setclock
#define clock_gettime getclock
#define clock_getres restimer
typedef struct timespec timespec_t;
struct timespec dummy;
#endif

#undef SEC     
#undef MILLISEC
#undef MICROSEC
#undef NANOSEC 
#define SEC             1
#define MILLISEC        1000
#define MICROSEC        1000000
#define NANOSEC         1000000000

/* Since PARIX desn't define any OS dependent preprocessor macro */
#ifdef __MCC__
#define __PARIX__
#endif

#ifdef __PARIX__
#define CLOCK_REALTIME 0

typedef struct timespec {
    time_t        tv_sec;	/* seconds */
    long          tv_nsec;	/* nanoseconds */
} timespec_t;

typedef int clockid_t;
#endif


#ifdef _UNICOS
#define CLOCK_REALTIME 0
typedef struct timespec timespec_t;
typedef int clockid_t;
#endif


#if defined (__CYGWIN__)
#include <sys/time.h>

#include <stdio.h>
/* for perror() */
#if 0
/* Vadim 040915 - these already in CygWin */

#define CLOCK_REALTIME 0
typedef struct timespec {
    time_t        tv_sec;	/* seconds */
    long          tv_nsec;	/* nanoseconds */
} timespec_t;

typedef int clockid_t;
#endif

#endif


#if defined (_MSC_VER) && defined(NO_POSIX_THREADS)
#define CLOCK_REALTIME 0
typedef struct timespec {
    time_t        tv_sec;	/* seconds */
    long          tv_nsec;	/* nanoseconds */
} timespec_t;

typedef int clockid_t;

int clock_gettime(clockid_t clock_id, struct timespec *tp);

#endif

#if (defined (_MSC_VER) && !defined(NO_POSIX_THREADS)) || defined(__MINGW32__)

#include <pthread.h>
#define CLOCK_REALTIME 0
typedef struct timespec timespec_t;
typedef int clockid_t;
int clock_gettime(clockid_t clock_id, struct timespec *tp);
#endif


#if defined(__linux__) 

#define CLOCK_REALTIME 0
#include <sys/time.h>
#include <unistd.h>
typedef struct timespec timespec_t;
typedef int clockid_t;

#endif



/* ----------------------------------------------------------------------- */

#ifdef __PARIX__

/* Read the system clock (RTCU & RTCL) =0 at program start. */
void rtctime(struct timespec *t);

/* --------------------------------------------------- */
/* POSIX */

int clock_settime(clockid_t clock_id, struct timespec *tp);
int clock_gettime(clockid_t clock_id, struct timespec *tp);
int clock_getres(clockid_t clock_id, struct timespec *tp);

/* --------------------------------------------------- */

#endif


/* sum = t1 + t2 */
void timespecadd(timespec_t *sum, timespec_t *t1, timespec_t *t2);

/* diff = t1 - t2 */
void timespecdiff(struct timespec *diff, 
		  struct timespec *t1, 
		  struct timespec *t2);




#ifdef __cplusplus
}
#endif



#endif
