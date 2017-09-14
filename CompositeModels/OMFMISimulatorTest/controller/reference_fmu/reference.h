#ifndef STEP_H
#define STEP_H

#ifdef __cplusplus
extern "C" {
#endif

void    setReal(int idx, double value);
double  getReal(int idx);

int takeStep(double t1);

#ifdef __cplusplus
}
#endif



#endif // STEP_H

