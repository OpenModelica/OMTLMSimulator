#include "SurrogateTimer.h"

#ifdef NO_RTIME

void TM_Init(tTM_Info *tInfo) {
  tInfo->total.tv_sec = 0;
  tInfo->total.tv_nsec = 0;
  tInfo->nAvg.zero();
}

void TM_Start(tTM_Info *tInfo) {
  tInfo->start = std::chrono::system_clock::now();
}

void TM_Stop(tTM_Info *tInfo) {
  std::chrono::system_clock::time_point stop = std::chrono::system_clock::now();
  long long elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop-tInfo->start).count();
  tInfo->total.tv_sec = (elapsed/1e6);
  tInfo->total.tv_nsec = (elapsed%(long long)1e6);
}

void TM_Avg(tTM_Info *tInfo, timespec_t *avg) {
  TM_Stop(tInfo);
  long long totalAvg = (tInfo->total.tv_sec*1e6+tInfo->total.tv_nsec+
              avg->tv_sec*1e6+avg->tv_nsec)/2.0;
  avg->tv_sec = totalAvg/1e6;
  avg->tv_nsec = totalAvg%(long long)1e6;
  TM_Start(tInfo);
}

void TM_Clear(tTM_Info *tInfo) {
  tInfo->total.tv_sec = 0;
  tInfo->total.tv_nsec = 0;
  tInfo->nAvg.zero();
}

#endif
