#ifndef _DOUBLE3_H
#define _DOUBLE3_H

#include "lightmatConfig.h"
#include "portable_longdouble.h"
#include <cstdlib> // Among many things overload abs(..) for integer types.
#include <cmath>   // Among many things overload abs(..) for float types.
#include <ostream>

//! The notation and functions are explained in the report D. Fritzson, "Notation and Vector algebra", 2015.
//!   Located at /home/Beast/IntegrationRep/doc/Reports/Theory/NotationAndVectorAlgebra/


class double3;
class double33;
class doubleN3;
class doubleN33;
class float3;
class Bstring;

//! ////////////////////////////////////////////////////////////
//!
//! Constants
//!
//! These are the defined constants
//!

//! A zero vector 
extern const double3 zerodouble3;



class double3 {							

    //! Friend class.
    friend class double33;
    friend class float3;
    friend class aPackBuf ;
    friend class ConverterFloat3Double3;

    //! Friend functions.
    friend inline const double3 operator-(const double3& v);
    friend inline const double3 operator*(const double s, const double3& v);
    friend inline const double3 operator*(const double3& v, const double s) ;
    friend inline const double3 operator*(const double33& a, const double3& b);
    friend inline const double3 operator*(const double3& a, const double33& b);
    friend inline double operator*(const double3& a, const double3& b);
    friend inline const double3 operator/(const double3& v, const double s);
    friend inline const double3 operator/(const double3& v, const double3& w);
    friend inline const double3 operator+(const double3& a, const double3& b);
    friend inline const double3 operator+(const double3& a, const double s);
    friend inline const double3 operator+(const double s, const double3& a);
    friend inline const double3 operator-(const double3& a, const double3& b);
    friend inline const double3 operator-(const double3& a, const double s);
    friend inline const double3 operator-(const double s, const double3& a);
    friend inline bool operator==(const double3& a, const double3& b);
    friend inline bool operator==(const double3& a, const double b);
    friend inline bool operator==(const double a, const double3& b);
    friend inline bool operator!=(const double3& a, const double3& b);
    friend inline bool operator!=(const double3& a, const double b);
    friend inline bool operator!=(const double a, const double3& b);
    friend std::ostream& operator<<(std::ostream& o, const double3& v);
    friend Bstring ToStr(const double3& val);
    friend inline const double3 Cross(const double3& V1, const double3& V2);
    friend const double3 Max(const double3& a, const double3& b);
    friend const double3 Min(const double3& a, const double3& b);
    friend const double3 fabs(const double3& a);
    friend inline const double33 OuterProduct(const double3& a, const double3& b);
    friend const double33 A123(const double3& phi);
    friend const double33 invA123(const double3& phi);
    friend const double33 U123(const double3& phi);
    friend const double33 H123(const double3& phi);
    friend const double33 invU123(const double3& phi);
    friend const double33 invH123(const double3& phi);
    friend const double33 vU123(const double3& phi,
                                const double3& vphi);
    friend const double33 vH123(const double3& phi,
                                const double3& vphi) ;
    friend const double33 vU321(const double3& phi,
                                const double3& vphi);
    friend const double33 vH321(const double3& phi,
                                const double3& vphi) ;
    friend const double33 A321(const double3& phi);
    friend void phi321ToEuler(const double3 &, double&, double&, double&, double&) ;
    friend void phi321ToEulerDer(const double3& phi,
                                 double3& dq1_dphi, double3& dq2_dphi, double3& dq3_dphi, double3& dq4_dphi);
    friend void phi123ToEuler(const double3 &, double&, double&, double&, double&) ;
    friend void phi123ToEulerDer(const double3& phi,
                                 double3& dq1_dphi, double3& dq2_dphi, double3& dq3_dphi, double3& dq4_dphi);

    friend const double33 invA321(const double3& phi);
    friend const double33 U321(const double3& phi);
    friend const double33 H321(const double3& phi);
    friend const double33 invU321(const double3& phi);
    friend const double33 invH321(const double3& phi);
    friend const double3 Omega_U123(const double3& phi,
                                    const double3& vphi);
    friend const double3 Omega_H123(const double3& phi,
                                    const double3& vphi); 
    friend const double3 Omega_U321(const double3& phi,
                                    const double3& vphi);
    friend const double3 Omega_H321(const double3& phi,
                                    const double3& vphi); 
    friend inline const double33 Acyl(const double3& RP);
    friend inline const double3 RC2R(const double3& RC);
    friend inline const double3 vRC2vR(const double3& RC,
                                       const double3& vRC);
    friend inline const double3 R2RC(const double3& R);
    friend const double3 aR2aRC(const double3& aR,
                                const double3& RC, 
                                const double3& vRC);
    friend const double3 aRC2aR(    const double3& RC, 
                                    const double3& vRC,
                                    const double3& aRC);
    friend inline const double3 sin(const double3& v);
    friend inline const double3 cos(const double3& v);

    friend double MaxRelAbsError(const double3& a, const double3& b, const double tol);


public:

    //! Empty constructor. Does NOT initialize to any special value.

    inline double3()
#ifdef DEBUG_LIGHTMAT
        :XX(1e301),YY(1e301),ZZ(1e301)
#endif
    {}

    //! Constructor that initializes to X,Y,Z to the given value.
    inline explicit double3(const double val);

    //! Constructor that initializes to X,Y,Z to the given values
    inline double3(const double xval, const double yval, const double zval);

    //! Copy constructor.
    inline double3(const double3& val);
 
    //! Modify self to be of vector length 1.
    inline double3& normalize();

    //! Modify self to be of vector length 1.
    //! If zero length, then set vector to [1, 0, 0].
    inline double3& normalizeSafe();

    //! Access element 1,2 or 3. Can be used as lvalue.
    //! Bounds checking if BOUNDS_CHECK is defined
    inline double& operator()(const int i);
 
    //! Access element 1,2 or 3. Can not be used a lvalue.
    //! Bounds checking if BOUNDS_CHECK is defined
    inline double operator()(const int i) const;
 
    //! Index to access norm2()
    static const int norm2_index_5=5;

    //! Index to access norm2XY()
    static const int norm2XY_index_4=4;

    //! Return "length" of vector. Earlier it was function length(). //BZ297
    inline double norm2() const;

    //! Return "length" of XY components of vector.
    inline double norm2XY() const;

    //! Return v * v
    inline double norm22() const;


    //! Return true if all components are zeroes
    inline bool isZero() const;

    //!  Return true if at least one components is non zero
    inline bool isNonZero() const;

    //! True if any of the elements is nan.
    bool isNan() const;

    //! True if any of the elements is infinite.
    bool isInfinite() const;
    
    //! Assignment.
    inline double3& operator=(const double a);

    //! Assignment.
    inline double3& operator=(const double3& v);
    
    //! Increment element-wise
    inline double3& operator+=(const double3& v);

    //! Decrement element-wise
    inline double3& operator-=(const double3& v);

    //! Multiply each element with s
    inline double3& operator*=(const double s);

    //! Divide every element with s
    inline double3& operator/=(const double s);
    
    //! Fills a vector with values a1, a2, a3
    inline void Set(const double a1, const double a2, const double a3) ;

    //! Get the components a1, a2, a3 from a vector
    inline void Get(double& a1, double& a2, double& a3) const;

    //! Get the components as an C array
    void Get(double * arr) const { arr[0]=XX; arr[1]=YY; arr[2]=ZZ; };

    //! Get the components a1, a2, a3 from a vector
    inline void GetReal(Real& a1, Real& a2, Real& a3) const;

#ifdef USE_IMPLICIT_CONV
    // cppcheck-suppress noExplicitConstructor
    //! Conversion from float3
    //! Do not use "explicit" here. It causes a lot of modifications.
    inline double3 (const float3& conv_in);
#endif


private: // protected: We try private instead.
    //! They are made protected purely for efficency reasons.
    //! PLEASE DO NOT ACCESS!!!!
    //! Will be accessed by friend inline functions later on.
    double XX; 
    double YY; 
    double ZZ;

    //! This still has to be written
    int check_bound(const int i) ;


};


////////////////////////////////////////////////////////////////////////
//
// .SS Declaration of functions
//
// Here we declare global functions related to double3
// For the description we use the notation: "V" denotes a double3,
// "s" scalar, and "M" a double33
//
//! -V
inline const double3 operator-(const double3& v);

//! s * V
inline const double3 operator*(const double s, const double3& v);

//! V * s
inline const double3 operator*(const double3& v, const double s);

//! Dot product: V * V
inline double operator*(const double3& a, const double3& b);

//! V / s
inline const double3 operator/(const double3& v, const double s);

//! V / V
inline const double3 operator/(const double3& v, const double3& w);

//! V + V
inline const double3 operator+(const double3& a, const double3& b);

//! V + s
inline const double3 operator+(const double3& a, const double s);

//! s + V
inline const double3 operator+(const double s, const double3& a);

//! V - V
inline const double3 operator-(const double3& a, const double3& b);

//! V - s
inline const double3 operator-(const double3& a, const double s);

//! s - V
inline const double3 operator-(const double s, const double3& a);

//! V == V (no fuzz for comparison)
inline bool operator==(const double3& a, const double3& b);

//! V == s (no fuzz for comparison)
inline bool operator==(const double3& a, const double b);

//! s == V (no fuzz for comparison)
inline bool operator==(const double a, const double3& b);

//! V != V (no fuzz for comparison)
inline bool operator!=(const double3& a, const double3& b);

//! V != s (no fuzz for comparison)
inline bool operator!=(const double3& a, const double b);

//! s != V (no fuzz for comparison)
inline bool operator!=(const double a, const double3& b);

//! Cross product, V = Cross(V1, V2), normally written V = V1 x V2
inline const double3 Cross(const double3& V1, const double3& V2);

//! Sinus of each component.
inline const double3 sin(const double3& v);

//! Cosinus of each component.
inline const double3 cos(const double3& v);

//! Maximum defined as maximum for each component separately.
const double3 Max(const double3& a, const double3& b);

//! Minimum defined as minimum for each component separately.
const double3 Min(const double3& a, const double3& b);

//! fabs() of each component separately.
const double3 fabs(const double3&a);

//! Output on ostream.
std::ostream& operator<<(std::ostream& o, const double3& v);

//! Conversion to string.
Bstring ToStr(const double3& val);


//! returns the size of a double3
inline int sizeOf(const double3 &) ;



#include "float3.h"
#include "double3.icc"
#include "float3.icc"
#endif // _DOUBLE3_H
