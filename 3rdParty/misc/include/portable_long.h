#ifndef _portable_long_h
#define _portable_long_h

// The definition of long64 is moved from portability.h to a separate file.
// The reason is that including portability.h is unnecssary in 
// many cases, and it is sufficient to use just this file for
// inclusion to tostr.h
// This definition was in compress.h

#if defined (__DECCXX) || defined (_CRAYC)
#define long64 long int
// In DEC-machines "long long does not exist"
#else
#if defined (_MSC_VER)
#define long64 __int64
#else
//! In non-DEC machines 64-bit arithmetic is necessary. 
typedef  long long int long64 ;
#endif
#endif

// Generally, to get all defined symbols you can run
// this command:
//     touch foo.h; cpp -dM foo.h


#if !defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
  #if (defined(__LITTLE_ENDIAN) && __LITTLE_ENDIAN)
    #define _LITTLE_ENDIAN
  #else 
    #if (defined(__BIG_ENDIAN) && __BIG_ENDIAN)
      #define _BIG_ENDIAN
    #else
      #if defined(_X86) || defined(__x86_64) || defined(_M_IX86) || defined(__alpha) || defined(__i386) || defined(_M_X64) || defined(__amd64__) || defined(__arm__)
        #define _LITTLE_ENDIAN
      #else
        #if defined(__sparc) || defined (__mips)
          #define _BIG_ENDIAN
        #else
          #error "No ENDIAN-information can be determined. Using BIG_ENDIAN"
          #define _BIG_ENDIAN
        #endif
      #endif
    #endif
  #endif
#endif


#endif //  _portable_long_h
