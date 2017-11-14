#ifndef LIGHTMAT_H
#define LIGHTMAT_H


//! //////////////////////////////////////////////////////////////////////
//!
//! .SS Functionality
//!
//! We describe the double3 and double33 classes with support functions.
//! They are supposed to be fast vector and matrix classes. It is a special case:
//! Only vectors of length 3 and matrixes of 3 by 3 are currently supported.
//! For them the allowed index are 1, 2 and 3. The classes are designed to minimize
//! overhead for creation and destruction. An example of use is a statement like:
//! a=b+c*d;
//!
//! The member functions with the support functions gives basic functionality
//! needed in vector algebra. Special operations for coordinate transformations
//! are also defined.
//!
//! The storage of the double33 is row oriented.
//!
//! These classes can be used as replacement for doubleVec (M++) and
//! DoubleVec (Math.h++).
//!
//! All mathematical functions are pure functions and do not have any side effects.
//!
//! .SS Performance
//!
//! Internally, the double3 class carries only 3 double variables and the
//! double33 class carries a C array with 9 elements of type double.
//! Thus copying, construction and destruction should go fast. The functions
//! "new" and "delete" are not used.
//!
//! Nerly all operator-overloading functions are declared inline; in fact nearly all
//! member-functions are inline) in order to let the compiler optimise it.
//!
//! This fix-length vector is appr. 10 times faster than a general [Dd]oubleVec.
//! It minimizes the memory-usage and a lot of double3-s can be created
//! with minimal memory-requirements.
//!
//! Compared to doing an operation with a loop 0,1,2, a double3 is faster
//! for using one operator. z=a+b is faster, z=a+b+c is slower.
//! Every additional operator is three times faster with a loop.
//!
//! "Manual inlining" is appr. 5 times faster than using a loop.
//! Thus, use "z[0]=a[0]+b[0]; z[1]=a[1]+b[1]; z[2]=a[2]+b[2];"
//! It is absolutely the fastest!
//!
//! .SS Notes
//!
//!
//! We temporarely make the definition below to handle int3 if it occurs.
//! #define int3 double3
//!
//! Some of the operators and functions we could not make inline. This due
//! to bugs in the SUN compiler version 3.
//!
//! It should also be investigated if the largest functions or the seldom
//! used functions really should be inline.
//!
//! .SS Authors
//!
//! Hakan Bastedt (up to version 1.15)
//! Dag Fritzson  (version 2 or later)
//!
//! .SS History
//!
//! vec3.h,v 1.15 1993/04/29 12:34:13 edpsbb Exp $
//!
//! lightmat.h, lightmat.icc, lightmat.cc v 2.0 1994-02-17
//!
//! Removed the OUTLINE option. Problems with DEC Alpha compilors.
//!
//! New version extended with one more dimension. Also new
//! types. We have now: double3, double33, doubleN3, doubleN33, doubleN, intN.
//! D. Fritzson 98-07-16
//!
//! The intN, doubleN, doubleN3, doubleN33 now starts their data area at index 0.
//! Note the mathematical index starts still fom 1.
//! New constructor for doubleN that takes external memory.
//! D. Fritzson 2004-12-06
//!
//! Now we use "from left" rule regarding index. That means that a(i) returns
//! a row if it is a double33. New types are doubleN3 and doubleN33. Obsolete
//! ones are double3N and double33N.
//! Some not used index operators are deactivated.
//! D. Fritzson 2006-03-09

#include "lightmatConfig.h"
#include "double3.h"
#include "double33.h"
#endif // LIGHTMAT_H
