#include <cassert>
#include "portability.h"
#include "ErrorLog.h"
#include "tostr.h"

#if defined(_MSC_VER)
#pragma optimize( "", off )
// It is important to keep this code non-optimized, at least for MVC++.
#endif

double multiply_pairs(const double & a1,const double & b1,
          const double & a2,const double & b2,
          const double & a3,const double & b3) {
 double ret=a1*b1+a2*b2+a3*b3;
 return ret;
        }

#if defined(_MSC_VER)
#pragma optimize( "", on )
#endif


#if defined(_MSC_VER)


unsigned int sleep(unsigned int secs)
{
  _sleep(secs*1000);
  return secs;
}

/*
int stat(const char *__path, struct _stat *__sbuf)
{
    return _stat(__path, __sbuf);
}
*/

FILE* popen(const char *command, const char* mode)
{
  return _popen(command, mode);
}

int pclose(FILE* stream)
{
  return _pclose(stream);
}


// If this function is defined here, we get warning C4273: 'unlink' : inconsistent dll linkage

/*
int unlink(const char *filename)
{
  return _unlink(filename);
}
*/

//! Windows usleep implementation.
//! Taken from stack-overflow
#include <windows.h>
void usleep(__int64 usec) 
{ 
  HANDLE timer; 
  LARGE_INTEGER ft; 

  ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

  timer = CreateWaitableTimer(NULL, TRUE, NULL); 
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
  WaitForSingleObject(timer, INFINITE); 
  CloseHandle(timer); 
}

#endif


#if (defined (__x86_64__) || defined (__i386__)) && defined (__linux__)  
#include <fpu_control.h>
#include <cstdio>
#include <cstdlib>

void ieee_round_precision(int v){  
  fpu_control_t mode = 0 ;
  fpu_control_t prev_mode ;
  const char * p = getenv("BEAST_NATIVE_FP") ;
  
  if(v)
    printf("IEEE mode setting\n") ;
  
  if (p != 0)
    return ;  
  
  prev_mode = __fpu_control ;
  
  if(v)
    printf("Intel FPU-CW was 0x%x\n", prev_mode) ;

  
  // Changed by lesta/alice, repeated by vaden/banan

  fpu_control_t tmp = 0x127f;
  _FPU_SETCW(tmp);
  mode = tmp;

  if(v)
    printf("Setting Intel FPU-CW (_FPU_DOUBLE, _FPU_RC_NEAREST) 0x%x\n", mode) ;
    
}

#elif defined (_MSC_VER) && defined (_M_IX86)
/* microsoft on i386, unsure, but guess not getenv  */
#include <cfloat>
void ieee_round_precision(int v){    
  printf("IEEE mode setting\n") ;
  _control87(_PC_53, MCW_PC);   
}


#elif defined(__CYGWIN__) && defined (__i386__)
/* cygwin on i388  */


void ieee_round_precision(int v){  
  const char * p = getenv("BEAST_NATIVE_FP") ;
  printf("IEEE mode setting\n") ;
  if (p != 0)
    return ;  
  
  if(v)
    printf("IEEE mode setting\n") ;
  
 /* _control87(_PC_53, MCW_PC);   */
 /* Not found Vadim 040915 */
  
}


#else
/* we assume that everybody else does it right */
void ieee_round_precision(int){ ; } 

#endif


#ifdef ROUND_DEFINED_IN_PORTABILITY_CC
#include <cmath>
double round(const double x)
{
    // conversion to an intermediate integer used here previously is unsafe for big numbers.
    // Why cannot be inline - because it is unclear whether #include <cmath> can be added in portability.h 
    // Vadim
    return floor(x+0.5);
}
#endif


#if defined(_MSC_VER)

// BZ326
// This "C" function is used from Compac Visual Fortran subroutines
// in lib\ATLAS\WinNT_P4SSE2\lib\libf77blas.a 
// In original BLAS it is a Fortran function using I/O but since
// Compac Visual Fortran runtime library is not linked to BEAST tools
// here is our own implementation of XERBLA.  

extern "C" void cdecl XERBLA(char * func,int * msg1);
void cdecl XERBLA(char * func,int * msg1)
 {



   Error(Bstring("XERBLA reports error in LAPACK function=")+"func");
   Error("Error code="+Int2Str(*msg1));
   Error("Exits.");


   assert(false);
   
}

#endif
