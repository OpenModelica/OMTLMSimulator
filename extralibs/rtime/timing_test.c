#include <stdio.h>

#include "timing.h"

tTM_Info t;

int main(int argc, char **argv)
{
    int i;
    double a = 1.2;
    TM_Init(&t);

    for (i=0; i < 1000000; i++) {
    TM_Start(&t);
    TM_Stop(&t);
    }
    printf("a = %g, Time = %ds %dns\n", a, 
	   TM_Tot(&t).tv_sec, TM_Tot(&t).tv_nsec);

    TM_Clear(&t);
    TM_Start(&t);
    for (i=0; i < 1000000; i++) {
	a *= 1.000001;
    }
    TM_Stop(&t);
    printf("a = %g, Time = %ds %dns\n", a, 
	   TM_Tot(&t).tv_sec, TM_Tot(&t).tv_nsec);

    TM_Clear(&t);
    a = 1.2;
    for (i=0; i < 1000000; i++) {
	TM_Start(&t);
	a *= 1.000001;
	TM_Stop(&t);
    }
    printf("a = %g, Time = %ds %dns\n", a, 
	   TM_Tot(&t).tv_sec, TM_Tot(&t).tv_nsec);

    fflush(stdout);

    exit(0);
}
