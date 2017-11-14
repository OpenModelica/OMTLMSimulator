

//#ifdef _MSC_VER
// This is g2c library missing in Windows.
#include <assert.h>
#include "f2c.h"
#undef abs

#include <math.h>
// This is because this is a C file, and <cmath> is not available from there.

#ifndef F2C_BEAST
#error "Wrong f2c.h included. Check your Makefile"
#endif

#if 0
integer s_cmp(char *a0, char *b0, ftnlen la, ftnlen lb)
{ 
    // Note: this is taken directly from lib/eigen library 
    // If you have multiple definition in linker just remove one of definitions.

    register unsigned char *a, *aend, *b, *bend;
    a = (unsigned char *)a0;
    b = (unsigned char *)b0;
    aend = a + la;
    bend = b + lb;

    if(la <= lb)
    {
        while(a < aend)
            if(*a != *b)
                return( *a - *b );
            else
            { ++a; ++b; }

            while(b < bend)
                if(*b != ' ')
                    return( ' ' - *b );
                else	++b;
    }

    else
    {
        while(b < bend)
            if(*a == *b)
            { ++a; ++b; }
            else
                return( *a - *b );
        while(a < aend)
            if(*a != ' ')
                return(*a - ' ');
            else	++a;
    }
    return(0);
}

#endif

#pragma GCC diagnostic ignored "-Wunused-parameter"
integer s_wsfe(cilist *p1){
    // Corresponds to FORMAT statement in fortran    
    return 0;
}
;

integer s_stop(char*z, long int n){
    // Corresponds to FORMAT statement in fortran
   assert(0);
    return 0;
}
;
integer e_wsfe( void){
    // Corresponds to impliceit eond-of-line in WRITE statement in fortran    
    return 0;
}
; 

integer do_fio(integer * p1, char * p2, ftnlen p3){
    // Corresponds to WRITE 6,P2, where p2 is double value
    // we just skip it
    return 0;}
;  

double pow_dd(doublereal * p1, doublereal * p2){
    return pow(*p1,*p2); 
}
;               

integer i_len(char * p1, ftnlen p2){
    // we guess that when character strings are passed in this GUI, they always are represented
    // by two arguments - pointer to start of the strings , and the length.
    return p2; 
}  
;           

doublereal etime_(real * p1){
    // returns time from system function etime
    // As this is needed just for statistics purpose, we do not care.

    return 0;

}
;               


//#endif

