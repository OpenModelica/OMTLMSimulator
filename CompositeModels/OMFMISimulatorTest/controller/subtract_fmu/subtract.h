#ifndef TEST_FMU_1_H
#define TEST_FMU_1_H

#ifdef __cplusplus
extern "C" {
#endif

void    setReal(int idx, double value);
double  getReal(int idx);

int takeStep(double t1);

#ifdef __cplusplus
}
#endif



#endif // TEST_FMU_1_H

