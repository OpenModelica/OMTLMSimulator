#ifndef PORTABLE_ISNAN_H
#define PORTABLE_ISNAN_H

#if defined(_MSC_VER)
#include <cfloat>
// The isnan appears in header files sometimes, e.g. for inlined functions.
// It is better therefore to have a separate header for such portaing.
#define portable_isnan(x) _isnan(x)
#define portable_isfinite(x) _finite(x)
#else
#include <cmath>
#define portable_isnan(x) std::isnan(x)
#define portable_isfinite(x) std::isfinite(x)
#endif

#endif
