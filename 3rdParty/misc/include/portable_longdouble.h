#ifndef PORTABLE_LONGDOUBLE_H
#define PORTABLE_LONGDOUBLE_H

typedef long double longdouble;


// set precision type to double that used in NewMat arrays.
// Use -DUSING_LONG_DOUBLE to affect the type of "double"
// Used in 2 places:
// in lib/misc/include/portable_longdouble.h
// in lib/NewMat/include.h
#ifdef USING_LONG_DOUBLE
typedef long double Real;
#else
typedef double Real;
#endif

#endif // PORTABLE_LONGDOUBLE_H

