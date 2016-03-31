#ifndef _DOUBLE3VEC_H
#define _DOUBLE3VEC_H

#include <cstdlib> // Among many things overload abs(..) for integer types.
#include <cmath>   // Among many things overload abs(..) for float types.
#include <ostream>
#include <string>

//! The notation and functions are explained in the report D. Fritzson, "Notation and Vector algebra", 2015.
//!   Located at /home/Beast/IntegrationRep/doc/Reports/Theory/NotationAndVectorAlgebra/

namespace tlmMisc
{

class double3Vec;
class double33Mat;

//! ////////////////////////////////////////////////////////////
//!
//! Constants
//!
//! These are the defined constants
//!

//! A zero vector 
extern const double3Vec zerodouble3Vec;



class double3Vec {

    //! Friend class.
    friend class double33Mat;
    friend class float3;
    friend class aPackBuf ;
    friend class ConverterFloat3Double3;

    //! Friend functions.
    friend inline const double3Vec operator-(const double3Vec& v);
    friend inline const double3Vec operator*(const double s, const double3Vec& v);
    friend inline const double3Vec operator*(const double3Vec& v, const double s) ;
    friend const double3Vec operator*(const double33Mat& a, const double3Vec& b);
    friend const double3Vec operator*(const double3Vec& a, const double33Mat& b);
    friend inline double operator*(const double3Vec& a, const double3Vec& b);
    friend inline const double3Vec operator/(const double3Vec& v, const double s);
    friend inline const double3Vec operator/(const double3Vec& v, const double3Vec& w);
    friend inline const double3Vec operator+(const double3Vec& a, const double3Vec& b);
    friend inline const double3Vec operator+(const double3Vec& a, const double s);
    friend inline const double3Vec operator+(const double s, const double3Vec& a);
    friend inline const double3Vec operator-(const double3Vec& a, const double3Vec& b);
    friend inline const double3Vec operator-(const double3Vec& a, const double s);
    friend inline const double3Vec operator-(const double s, const double3Vec& a);
    friend inline bool operator==(const double3Vec& a, const double3Vec& b);
    friend inline bool operator==(const double3Vec& a, const double b);
    friend inline bool operator==(const double a, const double3Vec& b);
    friend inline bool operator!=(const double3Vec& a, const double3Vec& b);
    friend std::ostream& operator<<(std::ostream& o, const double3Vec& v);
    friend std::string ToStr(const double3Vec& val);
    friend inline const double3Vec Cross(const double3Vec& V1, const double3Vec& V2);
    friend const double3Vec Max(const double3Vec& a, const double3Vec& b);
    friend const double3Vec Min(const double3Vec& a, const double3Vec& b);
    //friend const double3Vec fabs(const double3Vec& a);//****************************************
    friend inline const double33Mat OuterProduct(const double3Vec& a, const double3Vec& b);
    friend const double33Mat A123(const double3Vec& phi);
    friend const double33Mat invA123(const double3Vec& phi);
    friend const double33Mat U123(const double3Vec& phi);
    friend const double33Mat H123(const double3Vec& phi);
    friend const double33Mat invU123(const double3Vec& phi);
    friend const double33Mat invH123(const double3Vec& phi);
    friend const double33Mat vU123(const double3Vec& phi,
                                   const double3Vec& vphi);
    friend const double33Mat vH123(const double3Vec& phi,
                                   const double3Vec& vphi) ;
    friend const double33Mat vU321(const double3Vec& phi,
                                   const double3Vec& vphi);
    friend const double33Mat vH321(const double3Vec& phi,
                                   const double3Vec& vphi) ;
    friend const double33Mat A321(const double3Vec& phi);
    friend void phi321ToEuler(const double3Vec &, double&, double&, double&, double&) ;
    friend void phi321ToEulerDer(const double3Vec& phi,
                                 double3Vec& dq1_dphi, double3Vec& dq2_dphi, double3Vec& dq3_dphi, double3Vec& dq4_dphi);
    friend void phi123ToEuler(const double3Vec &, double&, double&, double&, double&) ;
    friend void phi123ToEulerDer(const double3Vec& phi,
                                 double3Vec& dq1_dphi, double3Vec& dq2_dphi, double3Vec& dq3_dphi, double3Vec& dq4_dphi);

    friend const double33Mat invA321(const double3Vec& phi);
    friend const double33Mat U321(const double3Vec& phi);
    friend const double33Mat H321(const double3Vec& phi);
    friend const double33Mat invU321(const double3Vec& phi);
    friend const double33Mat invH321(const double3Vec& phi);
    friend const double3Vec Omega_U123(const double3Vec& phi,
                                       const double3Vec& vphi);
    friend const double3Vec Omega_H123(const double3Vec& phi,
                                       const double3Vec& vphi);
    friend const double3Vec Omega_U321(const double3Vec& phi,
                                       const double3Vec& vphi);
    friend const double3Vec Omega_H321(const double3Vec& phi,
                                       const double3Vec& vphi);
    friend inline const double33Mat Acyl(const double3Vec& RP);
    friend inline const double3Vec RC2R(const double3Vec& RC);
    friend inline const double3Vec vRC2vR(const double3Vec& RC,
                                          const double3Vec& vRC);
    friend inline const double3Vec R2RC(const double3Vec& R);
    friend const double3Vec aR2aRC(const double3Vec& aR,
                                   const double3Vec& RC,
                                   const double3Vec& vRC);
    friend const double3Vec aRC2aR(    const double3Vec& RC,
                                       const double3Vec& vRC,
                                       const double3Vec& aRC);
    friend inline const double3Vec sin(const double3Vec& v);
    friend inline const double3Vec cos(const double3Vec& v);

    friend double MaxRelAbsError(const double3Vec& a, const double3Vec& b, const double tol);


public:

    //! Empty constructor. Does NOT initialize to any special value.

    inline double3Vec()
#ifdef DEBUG_LIGHTMAT
        :XX(1e301),YY(1e301),ZZ(1e301)
    #endif
    {}

    //! Constructor that initializes to X,Y,Z to the given value.
    inline explicit double3Vec(const double val);

    //! Constructor that initializes to X,Y,Z to the given values
    inline double3Vec(const double xval, const double yval, const double zval);

    //! Copy constructor.
    inline double3Vec(const double3Vec& val);
 
    //! Modify self to be of vector length 1.
    inline double3Vec& normalize();

    //! Modify self to be of vector length 1.
    //! If zero length, then set vector to [1, 0, 0].
    inline double3Vec& normalizeSafe();

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


    //! Return true if all components are zeroes
    inline bool isZero() const;

    //!  Return true if at least one components is non zero
    inline bool isNonZero() const;

    
    //! Assignment.
    inline double3Vec& operator=(const double a);

    //! Assignment.
    inline double3Vec& operator=(const double3Vec& v);
    
    //! Increment element-wise
    inline double3Vec& operator+=(const double3Vec& v);

    //! Decrement element-wise
    inline double3Vec& operator-=(const double3Vec& v);

    //! Multiply each element with s
    inline double3Vec& operator*=(const double s);

    //! Divide every element with s
    inline double3Vec& operator/=(const double s);
    
    //! Fills a vector with values a1, a2, a3
    inline void Set(const double a1, const double a2, const double a3) ;

    //! Get the components a1, a2, a3 from a vector
    inline void Get(double& a1, double& a2, double& a3) const;
#ifdef USE_IMPLICIT_CONV
    //! Conversion from float3
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
inline const double3Vec operator-(const double3Vec& v);

//! s * V
inline const double3Vec operator*(const double s, const double3Vec& v);

//! V * s
inline const double3Vec operator*(const double3Vec& v, const double s);

//! Dot product: M * V
const double3Vec operator*(const double33Mat& a, const double3Vec& b);

//! Dot product: V * M
const double3Vec operator*(const double3Vec& a, const double33Mat& b);

//! Dot product: V * V
inline double operator*(const double3Vec& a, const double3Vec& b);

//! V / s
inline const double3Vec operator/(const double3Vec& v, const double s);

//! V / V
inline const double3Vec operator/(const double3Vec& v, const double3Vec& w);

//! V + V
inline const double3Vec operator+(const double3Vec& a, const double3Vec& b);

//! V + s
inline const double3Vec operator+(const double3Vec& a, const double s);

//! s + V
inline const double3Vec operator+(const double s, const double3Vec& a);

//! V - V
inline const double3Vec operator-(const double3Vec& a, const double3Vec& b);

//! V - s
inline const double3Vec operator-(const double3Vec& a, const double s);

//! s - V
inline const double3Vec operator-(const double s, const double3Vec& a);

//! V == V (no fuzz for comparison)
inline bool operator==(const double3Vec& a, const double3Vec& b);

//! V == s (no fuzz for comparison)
inline bool operator==(const double3Vec& a, const double b);

//! s == V (no fuzz for comparison)
inline bool operator==(const double a, const double3Vec& b);

//! V != V (no fuzz for comparison)
inline bool operator!=(const double3Vec& a, const double3Vec& b);

//! Cross product, V = Cross(V1, V2), normally written V = V1 x V2
inline const double3Vec Cross(const double3Vec& V1, const double3Vec& V2);

//! Sinus of each component.
//inline const double3Vec sin(const double3Vec& v);

//! Cosinus of each component.
//inline const double3Vec cos(const double3Vec& v);

//! Maximum defined as maximum for each component separately.
const double3Vec Max(const double3Vec& a, const double3Vec& b);

//! Minimum defined as minimum for each component separately.
const double3Vec Min(const double3Vec& a, const double3Vec& b);

//! Output on ostream.
std::ostream& operator<<(std::ostream& o, const double3Vec& v);

//! Conversion to string.
std::string ToStr(const double3Vec& val);

//! returns the size of a double3
inline int sizeOf(const double3Vec &) ;

}

#include "double3Vec.icc"


#endif // _DOUBLE3_H
