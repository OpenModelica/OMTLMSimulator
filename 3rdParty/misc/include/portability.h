 #ifndef _portability_h
#define _portability_h

#include "portable_long.h"
// For long64 and endians

// This file should be included in any file that doesn't compile on a new
// platform. All fixes due to different interpretations of the ISO C/C++
// standard (or some really wierd compiler) should be included in this
// file. 
//                      Patrik Nordling, 2/VI/97
//
//  Things to be aware of when coding:
//
//  1/ method setbuf in the iostream class


// 1.
// The difference of the argument %1 in the setbuf method to the
// iostream class.  We introduce iostreambuffer, to be used in
// the situation:
// ofstream log ;
// iostreambuffer logBuffer[50000] ;  
// log.setbuf(logBuffer, sizeof logBuffer) ;
// rationale: The CRAY compiler has introduced the new ISO keyword "signed"

#if defined (_CRAYC)
typedef signed char iostreambuffer ;
#else
typedef char iostreambuffer ;
#endif


// This function must forced to become non-optimized in 
// the case of _MSC_VER
  //! computes a1*b1+a2*b2+a3*b3
double multiply_pairs(const double & a1,const double & b1,
          const double & a2,const double & b2,
          const double & a3,const double & b3);  

#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#if ( defined (__GNUC__) && GCC_VERSION < 30300 ) || (defined (_MSC_VER) && _MSC_VER<1800) 
// Round is missing for old compilers.
extern "C" double round(const double x);
#define ROUND_DEFINED_IN_PORTABILITY_CC
// See explanation in portability.cc
#endif

#if defined (_MSC_VER)||defined(__CYGWIN__)
//#define MAXDOUBLE DBL_MAX
// it is normally defined in float.h as ...
#define MAXDOUBLE 1.7976931348623158e+308
#endif






/* // These include files have different name due to */
/* // 8.3 DOS conventions */
/* #if defined (_MSC_VER) */
/* //#include <strstrea.h> */
/* //I dont't think this is relevant anymore when running Win2K. As such I */
/* //changed "#include <strstrea.h>" to "#include <strstream>". I had to do */
/* //this to get it to compile in W2K */
/* //Alan Treston 26/11/03 */
/* //#include <strstrea.h> */

/* // Since GCC moves to sstream from strstream, */
/* // this happens with MSC as well. In 7.1 there is sstream. */

/* #include <strstream> */

/* #else  */





/* #if defined (__GNUC__) && GCC_VERSION >= 30300 */

/* // ?? ToDo: */
/* //#include <sstream> */
/* // instead. */
/* // In g++ 3.4 strstream is called "deprected". In future  */
/* // there is no strstream, but only sstream. */
/* // All strstream uses should be removed from code. */
/* // Probably requires duplicate code in every use of strstream.  */

/* #include <strstream> */
/* typedef std::strstream   strstream; */
/* typedef std::istrstream istrstream; */
/* typedef std::ostrstream ostrstream; */

/* #else */
/* #include <strstream.h> */
/* #endif */
/* #endif */
#include <sstream> // strstream is deprecated

// This is because __alpha  DECCXX compiler  has no  ios::binary flag !
#if defined(__DECCXX)
#define ios_binary_portable 0
#else
#define  ios_binary_portable ios::binary
#endif

// This is because MSC creates a new file of zero length if cannot open
// a file for input (MSC feature)
#if defined(_MSC_VER)
#define ios_in_portable ios::in
// Earlier we had here ios::in|ios::nocreate, but this applies to OLD
// iostream library under MSVC++ 6.0, not supported for Beast anymore
#else
#define ios_in_portable  ios::in
#endif



void ieee_round_precision(int verbose) ;

#if !defined(_MSC_VER)

#include <unistd.h>

#else

/*
 * Microsoft VC++ specific portability defines
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <io.h>
#include <cstdio>
#include <cstdlib>
#include <process.h>
#include <sys/stat.h>
#include <cfloat>

#ifndef __cplusplus

#define sleep(x)  _sleep(x*1000)
#define popen(x,y) _popen(x,y)
#define pclose(x) _pclose(x)
#define unlink(x) _unlink(x)

#else

unsigned int sleep(unsigned int secs);
FILE* popen(const char *command, const char *mode);
int pclose(FILE* stream);
//int unlink(const char *filename);
void usleep(__int64 usec);
#define setenv(x,y,z) _putenv_s(x,y)
#endif 

typedef int socklen_t;
typedef int pid_t;
//#define isnan(x) _isnan(x)
// Moved to portable_isnan.h
#define finite(x) _finite(x)

#ifdef __cplusplus
}
#endif

// You must use stat64 everywhere, see BZ2881
#define stat64 _stat64

#endif // _MSC_VER


#if defined(__CYGWIN__)
#include <process.h>
#endif


/* IEEE presicion function. 
   It will set Intel to Double, and round nearest. */


/*
  SUN-Sparc specific defines, assumes CC compile on SUN
*/
#ifdef __sun
#include <ieeefp.h>
#endif

#endif // _portability_h


