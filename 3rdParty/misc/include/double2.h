#ifndef _DOUBLE2_H
#define _DOUBLE2_H


#include "lightmatConfig.h"

#include <cstdlib> // Among many things overload abs(..) for integer types.
#include <cmath>   // Among many things overload abs(..) for float types.
#include <ostream>


class double2;
class double33;
class doubleN3;
class doubleN33;
class float2;
class Bstring;

//! ////////////////////////////////////////////////////////////
//!
//! .SS Constants
//!
//! These are the defined constants
//!

//! A zero vector 
extern const double2 zerodouble2;



class double2 {

//! Friend class.
    friend class double33;
 friend class float2;
 friend class aPackBuf ;
 friend class ConverterFloat2Double2;

//! Friend functions.
friend inline const double2 operator-(const double2& v);
friend inline const double2 operator*(const double s, const double2& v);
friend inline const double2 operator*(const double2& v, const double s) ;
friend const double2 operator*(const double33& a, const double2& b);
friend const double2 operator*(const double2& a, const double33& b);
friend inline double operator*(const double2& a, const double2& b);
friend inline const double2 operator/(const double2& v, const double s);
friend inline const double2 operator/(const double2& v, const double2& w);
friend inline const double2 operator+(const double2& a, const double2& b);
friend inline const double2 operator+(const double2& a, const double s);
friend inline const double2 operator+(const double s, const double2& a);
friend inline const double2 operator-(const double2& a, const double2& b);
friend inline const double2 operator-(const double2& a, const double s);
friend inline const double2 operator-(const double s, const double2& a);
friend inline bool operator==(const double2& a, const double2& b);
friend inline bool operator==(const double2& a, const double b);
friend inline bool operator==(const double a, const double2& b);
friend inline bool operator!=(const double2& a, const double2& b);
friend std::ostream& operator<<(std::ostream& o, const double2& v);
friend Bstring ToStr(const double2& val);

friend const double2 Max(const double2& a, const double2& b);
friend const double2 Min(const double2& a, const double2& b);
friend const double2 fabs(const double2& a);

friend const double2 sin(const double2& v);


friend const double2 cos(const double2& v);



public:

    //! Empty constructor. Does NOT initialize to any special value.

    inline double2()
#ifdef DEBUG_LIGHTMAT
        :XX(1e301),YY(1e301)
#endif
    {}

    //! Constructor that initializes to X,Y to the given value.
    inline explicit double2(const double val);

    //! Constructor that initializes to X,Y to the given values
    inline double2(const double xval, const double yval);

    //! Copy constructor.
    inline double2(const double2& val);
 
    //! Modify self to be of vector length 1.
    inline double2& normalize();

    //! Modify self to be of vector length 1.
    //! If zero length, then set vector to [1, 0, 0].
    inline double2& normalizeSafe();

    //! Access element 1,2 or 3. Can be used as lvalue.
    //! Bounds checking if BOUNDS_CHECK is defined
    inline double& operator()(const int i);
 
    //! Access element 1,2 or 3. Can not be used a lvalue.
    //! Bounds checking if BOUNDS_CHECK is defined
    inline double operator()(const int i) const;
 
    //! Index to access norm2()
    static const int norm2_index_3=3;

    //! Return "length" of vector. Earlier it was function length(). //BZ297
    inline double norm2() const;
    
    //! Assignment.
    inline double2& operator=(const double a);

    //! Assignment.
    inline double2& operator=(const double2& v);
    
    //! Increment element-wise
    inline double2& operator+=(const double2& v);

    //! Decrement element-wise
    inline double2& operator-=(const double2& v);

    //! Multiply each element with s
    inline double2& operator*=(const double s);

    //! Divide every element with s
    inline double2& operator/=(const double s);
    
    //! Fills a vector with values a1, a2, a3
    inline void Set(const double a1, const double a2) ;

    //! Get the components a1, a2, a3 from a vector
    inline void Get(double& a1, double& a2) const;

    //! Return true if all components are zeroes
    inline bool isZero() const;

    //!  Return true if at least one components is non zero
    inline bool isNonZero() const;

#ifdef USE_IMPLICIT_CONV
    //! Conversion from float2
    // cppcheck-suppress noExplicitConstructor
    inline double2 (const float2& conv_in);
#endif


private: // protected: We try private instead.
    //! They are made protected purely for efficency reasons.
    //! PLEASE DO NOT ACCESS!!!!
    //! Will be accessed by friend inline functions later on.
    double XX; 
    double YY; 


    //! This still has to be written
    int check_bound(const int i) ;


};


////////////////////////////////////////////////////////////////////////
//
// .SS Declaration of functions
//
// Here we declare global functions related to double2
// For the description we use the notation: "V" denotes a double2,
// "s" scalar, and "M" a double33
//
//! -V
inline const double2 operator-(const double2& v);

//! s * V
inline const double2 operator*(const double s, const double2& v);

//! V * s
inline const double2 operator*(const double2& v, const double s);

//! Dot product: M * V
const double2 operator*(const double33& a, const double2& b);

//! Dot product: V * M
const double2 operator*(const double2& a, const double33& b);

//! Dot product: V * V
inline double operator*(const double2& a, const double2& b);

//! V / s
inline const double2 operator/(const double2& v, const double s);

//! V / V
inline const double2 operator/(const double2& v, const double2& w);

//! V + V
inline const double2 operator+(const double2& a, const double2& b);

//! V + s
inline const double2 operator+(const double2& a, const double s);

//! s + V
inline const double2 operator+(const double s, const double2& a);

//! V - V
inline const double2 operator-(const double2& a, const double2& b);

//! V - s
inline const double2 operator-(const double2& a, const double s);

//! s - V
inline const double2 operator-(const double s, const double2& a);

//! V == V (no fuzz for comparison)
inline bool operator==(const double2& a, const double2& b);

//! V == s (no fuzz for comparison)
inline bool operator==(const double2& a, const double b);

//! s == V (no fuzz for comparison)
inline bool operator==(const double a, const double2& b);

//! V != V (no fuzz for comparison)
inline bool operator!=(const double2& a, const double2& b);

//! Cross product, V = Cross(V1, V2), normally written V = V1 x V2
inline const double2 Cross(const double2& V1, const double2& V2);

//! Sinus of each component.
inline const double2 sin(const double2& v);

//! Cosinus of each component.
inline const double2 cos(const double2& v);

//! Maximum defined as maximum for each component separately.
const double2 Max(const double2& a, const double2& b);

//! Minimum defined as minimum for each component separately.
const double2 Min(const double2& a, const double2& b);

//! fabs on each component separately.
const double2 fabs(const double2& a);

//! Output on ostream.
std::ostream& operator<<(std::ostream& o, const double2& v);

//! Conversion to string.
Bstring ToStr(const double2& val);


//! returns the size of a double2
inline int sizeOf(const double2 &) ;



#include "float2.h"
#include "double2.icc"
#include "float2.icc"
#endif // _DOUBLE2_H
