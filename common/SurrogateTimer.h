//This surrogate library is intended to replace rtime
//when it is not available (e.g. on Mac). It relies on
//the C++11 chrono library for measuring time.
//
// Robert Braun, 2018-03-29

#ifndef SURROGATETIMER_H
#define SURROGATETIMER_H

#include <chrono>

#ifdef NO_RTIME

class timespec_t
{
public:
    long long tv_sec;
    long long tv_nsec;
};

class tTM_Info
{
public:
  timespec_t total;
  std::chrono::system_clock::time_point start;
  std::chrono::duration<double> nAvg;
};

void TM_Init(tTM_Info *tInfo);
void TM_Start(tTM_Info *tInfo);
void TM_Stop(tTM_Info *tInfo);
void TM_Avg(tTM_Info *tInfo, timespec_t *avg);
void TM_Clear(tTM_Info *tInfo);

#endif

#endif // SURROGATETIMER_H
