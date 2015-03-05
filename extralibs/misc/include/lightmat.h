#ifndef _LIGHTMAT_H
#define _LIGHTMAT_H


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

#define USE_IMPLICIT_CONV
// It must be so. SurfaceDataTmpl::Set is used and not SetS/SetD. // BZ1053

#include <cstdlib> // Among many things overload abs(..) for integer types.
#include <cmath>   // Among many things overload abs(..) for float types.
#include "Bstring.h"
#include "tostr.h"

class double3;
class double33;
class doubleN3;
class doubleN33;
class float3;
class Bstring;
//! ////////////////////////////////////////////////////////////
//!
//! .SS Constants
//!
//! These are the defined constants
//!

//! A zero vector 
extern const double3 zerodouble3;

//! A zero matrix 
extern const double33 zerodouble33;

//! A identity matrix 
extern const double33 Idouble33;


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
friend const double3 operator*(const double33& a, const double3& b);
friend const double3 operator*(const double3& a, const double33& b);
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
friend ostream& operator<<(ostream& o, const double3& v);
friend Bstring ToStr(const double3& val);
friend inline const double3 Cross(const double3& V1, const double3& V2);
friend const double3 Max(const double3& a, const double3& b);
friend const double3 Min(const double3& a, const double3& b);
friend const double3 fabs(const double3& a);//****************************************
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
friend void phi123ToEuler(const double3 &, double&, double&, double&, double&) ;
friend void phi123ToEulerDer(const double3& phi,
                      double3& dq1_dphi,
                      double3& dq2_dphi,
                      double3& dq3_dphi,
                      double3& dq4_dphi);

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

public:

    //! Empty constructor. Does NOT initialize to any special value.

    inline double3()
#ifdef DEBUG_LIGHTMAT
        :XX(1e301),YY(1e301),ZZ(1e301)
#endif
    {}

    //! Constructor that initializes to X,Y,Z to the given value.
    inline double3(const double val);

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
 
    //! Return "length" of vector. Earlier it was function length(). //BZ297
    inline double norm2() const;
    
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


class float3 {
    friend class double3;
    friend class ConverterFloat3Double3;
    friend class aPackBuf;

 public:
    inline float3 ()
#ifdef DEBUG_LIGHTMAT
    // The intention is to detect errors when these
    // fields are not initialized.
        :XX(1e38f),YY(1e38f),ZZ(1e38f)
#endif
        {};

    inline explicit  float3(const float f_in):
        XX(f_in), YY(f_in), ZZ(f_in)
    {};

    inline float3 (const float3& f3):
        XX(f3.XX),
        YY(f3.YY),
        ZZ(f3.ZZ)
            {};

#ifdef USE_IMPLICIT_CONV
    inline float3 (const double3& conv_in){
        XX=static_cast<float>(conv_in.XX);
        YY=static_cast<float>(conv_in.YY);
        ZZ=static_cast<float>(conv_in.ZZ);
    };
#endif


  //! Constructor that initializes the components to the given values
 inline float3(const float a1, const float a2, const float a3);

 friend inline const float3 operator+(const  float3  & a, const   float3 & b);
 friend inline const float3 operator-(const  float3  & a, const   float3 & b);
 friend inline bool operator==(const float3 & a, const float3 & b);
 friend inline bool operator!=(const float3& a, const float3& b);
   //! M += M
 inline float3& operator+=(const float3& m);
 //! M -= M
 inline float3& operator-=(const float3& m);

 //! Assignment.
 inline float3& operator=(const float3& m) ;
 friend Bstring ToStr(const float3& val);

 //! Access element 1,2 or 3. Can not be used a lvalue.
 //! Bounds checking if BOUNDS_CHECK is defined
 inline float operator()(const int i) const;

 //! Access element 1,2 or 3. Can be used as lvalue.
 //! Bounds checking if BOUNDS_CHECK is defined
 inline float& operator()(const int i);

 private: 

    float XX; 
    float YY; 
    float ZZ;
};

#ifdef USE_IMPLICIT_CONV
// Class not needed
#else

class ConverterFloat3Double3 {
 public:
    // This class is needed in order to privide
    // access to private members of  float3,  double3 
    // without declaring a friend function.
    inline static float3 convertToFloat3(const double3 & d) {
        float3 f(static_cast<const float>(d.XX),
            static_cast<const float>(d.YY),
            static_cast<const float>(d.ZZ)
            );
        return f;
    };

    inline static double3 convertToDouble3(const float3 & f) {
        double3 d(f.XX,f.YY,f.ZZ);
        return d;
    };
};
#endif



class double33 {

// Friend Inline functions.
//!friend inline const double33 Transpose(const double33& a);
friend inline const double33 operator-(const double33& m);
friend inline const double33 operator*(const double s, const double33& m);
friend inline const double33 operator*(const double33& m, const double s);
friend inline const double33 operator*(const double33& a, const double33& b);
friend const double3 operator*(const double33& a, const double3& b);
friend const double3 operator*(const double3& a, const double33& b);
friend inline const double33 operator/(const double33& m, const double s);
friend inline const double33 operator+(const double33& a, const double33& b);
friend inline const double33 operator+(const double33& m, const double s);
friend inline const double33 operator+(const double s, const double33& m);
friend inline const double33 operator-(const double33& a, const double33& b);
friend inline const double33 operator-(const double33& m, const double s);
friend inline const double33 operator-(const double s, const double33& m);
friend inline bool operator==(const double33& a, const double33& b);
friend inline bool operator==(const double33& a, const double b);
friend inline bool operator==(const double a, const double33& b);
friend inline bool operator!=(const double33& a, const double33& b);
friend const double33 Max(const double33& a,const double33& b);  //*****************************************
friend const double33 Min(const double33& a,const double33& b);  //*****************************************
friend const double33 fabs(const double33& a); //*****************************************
friend ostream& operator<<(ostream& o, const double33& M);
friend Bstring ToStr(const double33& val);
friend const double3 ATophi321(const double33& A);
friend const double3 ATophi123(const double33& A);
friend void AToEuler(const double33& A, double& q1, double& q2, double& q3, double& q4);


friend class aPackBuf ;

public:

    //! Empty constructor. Does NOT initialize to any special value.
    inline double33(){};

    //! Constructor that initializes the components to the given value.
    inline double33(const double val);

    //! Constructor that initializes the components to the given values
    double33(const double a11, const double a12, const double a13,
	     const double a21, const double a22, const double a23,
	     const double a31, const double a32, const double a33);

    //! Copy constructor.
    double33(const double33& val);

    //! Access elements. Can be used as lvalue.
    inline double& operator()(const int row, const int col);

    //! Access elements. Can not be used as lvalue.
    inline double operator()(const int row, const int col) const;

    //! Access column. Can not be used as lvalue.
    //! Not so efficient implemented rigth now.
    inline const double3 operator()(const int row) const;

    //! Assignment.
    inline double33& operator=(const double a);
    
    //! Assignment.
    double33& operator=(const double33& m);

    //! Assignment. 
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //double33& operator=(const doubleN3& m);

    //! M += M
    inline double33& operator+=(const double33& b);

    //! M += s
    inline double33& operator+=(const double s);

    //! M -= M
    inline double33& operator-=(const double33& b);

    //! M -= s
    inline double33& operator-=(const double s);

    //! M *= s
    inline double33& operator*=(const double s);

    //! M *= M
    double33& operator*=(const double33& b);

    //! M /= s
    inline double33& operator/=(const double s);

    //! Fills a matrix of type double33 from the row vectors a1, a2, a3
    //! Same as SetRows
    inline void Set(const double3& a1, 
                    const double3& a2, 
                    const double3& a3);

    //! Fills a matrix of type double33 from the row vectors r1, r2, r3
    inline void SetRows(const double3& r1,
                        const double3& r2,
                        const double3& r3);

    //! Fills a matrix of type double33 from the column vectors c1, c2, c3
    inline void SetCols(const double3& c1,
                        const double3& c2,
                        const double3& c3);

    //! Fills a matrix of type double33 from the elements or components.
    void Set(const double a11, const double a12, const double a13,
	     const double a21, const double a22, const double a23,
	     const double a31, const double a32, const double a33);

    //! Get the components a11, a12, ... a33 from a matrix of type double33.
    void Get(double& a11,  double& a12,  double& a13,
	     double& a21,  double& a22,  double& a23,
	     double& a31,  double& a32,  double& a33) const;

    //! Return the determinant of matrix.
    inline double det() const;

    //! Return the transpose of the matrix.
    inline const double33 T() const;

 private:  // We try private intead of protected.
    //! Private data area. (1,*) = 0,1,2, (2,*)=3,4,5, (3,*)=6,7,8
    double x[9]; 

    //! Must be [1-3,1-3]
    int check_bound(const int row, const int col);

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

//! Dot product: M * V
const double3 operator*(const double33& a, const double3& b);

//! Dot product: V * M
const double3 operator*(const double3& a, const double33& b);

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

//! Output on ostream.
ostream& operator<<(ostream& o, const double3& v);

//! Conversion to string.
Bstring ToStr(const double3& val);


//! returns the size of a double3
inline int sizeOf(const double3 &) ;




////////////////////////////////////////////////////////////////////////
//
// .SS Declaration of functions
//
// Here we declare global functions related to double33.
// For the description we use the notation: "V" denotes a double3, 
// "s" scalar, and "M" a double33
//

//! -M
inline const double33 operator-(const double33& m);

//! s * M
inline const double33 operator*(const double s, const double33& m);

//! M * s 
inline const double33 operator*(const double33& m, const double s);

//! M / s 
inline const double33 operator/(const double33& m, const double s);

//! M + M 
inline const double33 operator+(const double33& a, const double33& b);

//! M + s 
inline const double33 operator+(const double33& m, const double s);

//! s + M
inline const double33 operator+(const double s, const double33& m);

//! M - M 
inline const double33 operator-(const double33& a, const double33& b);

//! M - s 
inline const double33 operator-(const double33& m, const double s);

//! s - M
inline const double33 operator-(const double s, const double33& m);

//! Dot product: M * M
inline const double33 operator*(const double33& a, const double33& b);

//! M == M (no fuzz for comparison) 
inline bool operator==(const double33& a, const double33& b);

//! M == s (no fuzz for comparison) 
inline bool operator==(const double33& a, const double b);

//! s == M (no fuzz for comparison) 
inline bool operator==(const double a, const double33& b);

//! M != M (no fuzz for comparison) 
inline bool operator!=(const double33& a, const double33& b);

//! Outer product
inline const double33 OuterProduct(const double3& a, const double3& b);

//inline const double33 Transpose(const double33& a);
// Gives the transposed matrix

//! Output on ostream
ostream& operator<<(ostream& o, const double33& M);

//! Conversion to string.
Bstring ToStr(const double33& val);



///////////////////////////////////////////////////////////////////////
//
// .SS Declarations of functions
//
// Here we declare functions used for coordinate transformations etc.
//

//! Validate the Euler parameters with a tolerance of 1.0e-6
int validate_euler(double q1,double q2,double q3,double q4) ;

//! Validate the Euler parameters with a tolerance of 1.0e-15
int validate_euler2(double q1,double q2,double q3,double q4) ;

//! Validate the Euler parameters with a tolerance of 1.0e-6
int validate_euler(double* q) ;

//! Create a transformation matrix from base system to local system.
//! We use Euler parameters.
const double33 Aeuler(const double q1,
		      const double q2,
		      const double q3,
		      const double q4) ;


//! Create an inverse transformation matrix from base system to local system.
//! We use Euler parameters.
const double33 invAeuler(const double q1,
			 const double q2,
			 const double q3,
			 const double q4) ;

//! Transformation from Euler parameters to rotation angles.
//! We use 3,2,1 rotation scheme. The angles will be cyclic
//! due to arcsin function.
const double3 EulerTophi321(const double q1,
			    const double q2,
			    const double q3,
			    const double q4) ;


//! Transformation from rotation angles to  Euler parameters.
//! We use 3,2,1 rotation scheme.
void phi321ToEuler(const double3& phi,
		   double& q1,
		   double& q2,
		   double& q3,
		   double& q4) ;

//! Transformation from rotation angles to  Euler parameters.
//! We use 1,2,3 rotation scheme.
void phi123ToEuler(const double3& phi,
		   double& q1,
		   double& q2,
		   double& q3,
		   double& q4) ;


//! Calculate Euler parameters derivatives with respect to the phi123 angles
//! for given orientation.
void phi123ToEulerDer(const double3& phi,
                      double3& dq1_dphi,
                      double3& dq2_dphi,
                      double3& dq3_dphi,
                      double3& dq4_dphi);

//! Transformation from rotation angles to  Euler parameters.
//! phiSequence 0 -> A321
//! phiSequence 1 -> A123
void phiToEuler(const double3& phi,
                const int phiSequence,
                double& q1,
                double& q2,
                double& q3,
                double& q4) ;

//! The conversion from a rotation matrix to
//! Euler parameters (quaternions of length 1) is unique
//! up to a sign \f$\{+1,-1\}\f$. That is, \f$(q_1,q_2,q_3,q_4)\f$ represents
//! the same rotation matrix as \f$(-q_1,-q_2,-q_3,-q_4)\f$.
//! This implementation is done so that \f$q_1\f$ is always non-negative.
//!
//! \par Theory
//!
//! For further information, see Chapter 2 in 
//! "Lie Groups: An Introduction Through Linear Groups" by
//! Wulf Rossmann. Especially Lemma 2A on page 34.
//!
//! The Euler parameters \f$q\f$ is an element in the 3-sphear
//! \f$S^3 = \{ q \in \mathbf{R}^4 ; ||q||=1 \} \f$. This space
//! is isomorphic to 
//! \f$SU(2) = \{ B \in GL(2,\mathbf{C}) ; B^* B = 1 \} \f$, where
//! \f$B = [\alpha,-\bar\beta;\beta,\bar\alpha]\f$. 
//! An isomorphism is given by 
//! \f$(\alpha,\beta) \mapsto 
//! (\Re(\beta),\Im(\beta),\Im(\alpha),\Re(\alpha))=(q_1,q_2,q_3,q_4)\f$.
//!
//! A surjective map \f$ \phi: S^3 \ni q \mapsto A \in SO(3)\f$ is given by \f[
//! A = \left[ \begin{array}{ccc}
//! q_1^2-q_2^2-q_3^2+q_4^2 & 2(q_1 q_2+q_3 q_4) & 2(q_1 q_3-q_2 q_4)
//! \\ 2(q_1q_2-q_3q_4) & -q_1^2+q_2^2-q_3^2+q_4^2 & 2(q_2q_3+q_1q_4)
//! \\ 2(q_1 q_3+q_2q_4) & 2(q_2q_3-q_1q_4) & -q_1^2-q_2^2+q_3^2+q_4^2
//! \end{array} \right] \f]
//! (Notice from the formula that the sign of \f$q\f$ does not matter.)
//!
//! The corresponding injective map \f$ \psi : SO(3) \ni A \mapsto q \in S^3\f$ 
//! fulfills \f[ \begin{array}{c}
//! |q_1| = \sqrt{1+A_{11}-A_{22}-A_{33}}/2
//! \\ |q_2| = \sqrt{1-A_{11}+A_{22}-A_{33}}/2
//! \\ |q_3| = \sqrt{1-A_{11}-A_{22}+A_{33}}/2
//! \\ |q_4| = \sqrt{1+A_{11}+A_{22}+A_{33}}/2
//! \end{array} \f]
//! The signs follows by combining off diagonal elements in the
//! formula for \f$q \mapsto A\f$ and the choice that 
//! \f$q_1 \geq 0 \f$ (see the actual implementation).
//!
//! Notice that \f$\phi \circ \psi = Id_{SO(3)}\f$ whereas \f$\psi \circ \phi = \pm Id_{S^3}\f$.
void AToEuler(const double33& A,
	      double& q1,
	      double& q2,
	      double& q3,
	      double& q4);


//! Calculate the rotation angles from a matrix, 3,2,1 rotation scheme.
const double3 ATophi321(const double33& A);

//! Calculate the rotation angles from a matrix, 1,2,3 rotation scheme.
const double3 ATophi123(const double33& A);

//! Calculate the rotation angles from a matrix.
//! phiSequence 0 -> A321
//! phiSequence 1 -> A123
const double3 ATophi(const double33& A, const int phiSequence);



//! Calculates the time derivate of the Euler parameters q1 ... q4.
//! If omega dimensionless, then qdot is that also.
void qdot(const double q1,
          const double q2,
          const double q3,
          const double q4,
          const double3& cg_Omega_cB_cB,
          double& q1dot,
          double& q2dot,
          double& q3dot,
          double& q4dot);

//! Calculates the time derivate of the Euler parameters q1 ... q4.
//! If omega dimensionless, then qdot is that also.
void qdot(const double q1,
          const double q2,
          const double q3,
          const double q4,
          const double cg_Omega_cB_cB_1, 
          const double cg_Omega_cB_cB_2, 
          const double cg_Omega_cB_cB_3,
          double& q1dot,
          double& q2dot,
          double& q3dot,
          double& q4dot) ;
//! Calculates the time derivate of the Euler parameters q1 ... q4.
//! If omega dimensionless, then qdot is that also.
void qdot_unsafe(const double q1,
          const double q2,
          const double q3,
          const double q4,
          const double cg_Omega_cB_cB_1, 
          const double cg_Omega_cB_cB_2, 
          const double cg_Omega_cB_cB_3,
          double& q1dot,
          double& q2dot,
          double& q3dot,
          double& q4dot) ;

// Calculates the time derivate of the Euler parameters q1 ... q4.
// As above but for omega in cg
void qdot_om_cg(const double q1,
          const double q2,
          const double q3,
          const double q4,
           const double omega_cg_1,
           const double omega_cg_2,
           const double omega_cg_3,
          double& q1dot,
          double& q2dot,
          double& q3dot,
          double& q4dot);

// Calculates the time derivate of the Euler parameters q1 ... q4.
// As above but for omega in cg
void qdot_om_cg(const double q1,
          const double q2,
          const double q3,
          const double q4,
          const double3& Omega_cg,
          double& q1dot,
          double& q2dot,
          double& q3dot,
          double& q4dot);

//! Calculates omega from q and q_dot (omega = (T^{-1}(q) q_dot)
void omega_q_qdot(const double q[4], const double qdot[4], double omega[3]);

//#if 0
//#endif
//! Calculates the transpose of the omega_q_qdot ( T^{-T} [a;b;c] )
void omega_q_qdot_T(const double q1 ,const double q2,const double q3,const double q4,
 		    const double a,  const double b, const double c, 
 		    double &out1, double &out2, double &out3, double &out4);

//! forms the transformation matrix T in q_dot = T omega
void FormT(double T[4][3], const double q[4]);

//void FormS(double S[4][4],const double omega[3]);
void FormS(double S[4][4],const double3& omega);

//!Computes  d(T(q_dot)/dq_dot T^{-1} q_dot + T(Q_dot) T ^{-1}
void FormSS(double SS[4][4],const double q[4],const double qdot[4]);

//! Create a symmetric matrix 
inline const double33 Asym(const double axx, 
			   const double axy, 
			   const double axz,
			   const double ayy, 
			   const double ayz, 
			   const double azz);

//! Create the inverse to a symmetric matrix.
const double33 invAsym(const double xx, 
		       const double xy, 
		       const double xz,
		       const double yy, 
		       const double yz, 
		       const double zz);

//! Create a transformation matrix from base system to local system.
//! We use 1,2,3 rotation scheme.
const double33 A123(const double3& phi);

//! Create an inverse transformation matrix from base system to local system.
//! We use 1,2,3 rotation scheme.
const double33 invA123(const double3& phi);

//! Create a transformation matrix from base system to local system.
//! We use 3,2,1 rotation scheme.
const double33 A321(const double3& phi);

//! Create an inverse (transposed) transformation matrix from base system to local system.
//! We use 3,2,1 rotation scheme.
const double33 invA321(const double3& phi);

//! Create a transformation matrix from base system to local system.
//! phiSequence 0 -> A321
//! phiSequence 1 -> A123
const double33 A(const double3& phi, const int phiSequence);

//! Create a transformation matrix from base system to local system.
//! We use only rotation around the axis number 3 (z-axis).
const double33 A3(const double phi_3);

//! Create a transformation matrix from base system to local system.
//! We use only rotation around the axis number 2 (y-axis).
const double33 A2(const double phi_2);

//! Create a transformation matrix from base system to local system.
//! We use only rotation around the axis number 1 (x-axis).
const double33 A1(const double phi_1);

//! Create an "U" transformation matrix.
//! We use 1,2,3 rotation scheme.
const double33 U123(const double3& phi);

//! Create an "U" transformation matrix.
//! We use 3,2,1 rotation scheme.
const double33 U321(const double3& phi);

//! Create an "H" transformation matrix.
//! We use 3,2,1 rotation scheme.
const double33 H321(const double3& phi);

//! Create an "H" transformation matrix.
//! We use 1,2,3 rotation scheme.
const double33 H123(const double3& phi);

//! Create an inverse "U" transformation matrix.
//! We use 1,2,3 rotation scheme.
const double33 invU123(const double3& phi);

//! Create an inverse "H" transformation matrix.
//! We use 1,2,3 rotation scheme.
const double33 invH123(const double3& phi);

//! Create an inverse "H" transformation matrix.
const double33 invH(const double3& phi, const int phiSequence);

//! Create an inverse "H" transformation matrix.
//! We use 3,2,1 rotation scheme.
const double33 invH321(const double3& phi);

//! Create an inverse "U" transformation matrix.
//! We use 3,2,1 rotation scheme.
const double33 invU321(const double3& phi);


//! Create a "vU" transformation matrix.
//! We use 1,2,3 rotation scheme.
const double33 vU123(const double3& phi,
			const double3& vphi);

//! Create a "vH" transformation matrix.
//! We use 1,2,3 rotation scheme.
const double33 vH123(const double3& phi,
			const double3& vphi);

//! Create a "vH" transformation matrix.
//! We use 3,2,1 rotation scheme.
const double33 vH321(const double3& phi,
			const double3& vphi);

//! Create a "vU" transformation matrix.
//! We use 3,2,1 rotation scheme.
const double33 vU321(const double3& phi,
		     const double3& vphi);

//! Angular speed of the local system relative the base system, i.e. "Omega[cBase]".
//! The vector is expressed in the local system and we use 1,2,3 rotation scheme, i.e.
//! "Omega[cBase][this] = U[cBase] . vphi[cBase]"
const double3 Omega_U123(const double3& phi,
			 const double3& vphi);

//! Angular speed of the local system relative the base system, i.e. "Omega[cBase]".
//! The vector is expressed in the base system and we use 1,2,3 rotation scheme, i.e.
//! "Omega[cBase][cBase] = H[cBase] . vphi[cBase]"
const double3 Omega_H123(const double3& phi,
			 const double3& vphi);

//! Angular acc. of the local system relative the base system. The vector 
//! is expressed in the local system and we use 1,2,3 rotation scheme. We have: 
//! "vOmega[cBase][cBase,this] =  vOmega[cBase][this,this] =
//! vU[cBase] . vphi[cBase] + U[cBase] . aphi[cBase]"
const double3 vOmega_U123(const double3& phi,
			  const double3& vphi,
			  const double3& aphi);

//! Angular acc. of the local system relative the base system. The vector 
//! is expressed in the base system and we use 1,2,3 rotation scheme. We have: 
//! "vOmega[cBase][cBase,cBase] =  vOmega[cBase][this,cBase] =
//! vH[cBase] . vphi[cBase] + H[cBase] . aphi[cBase]"
const double3 vOmega_H123(const double3& phi,
			  const double3& vphi,
			  const double3& aphi);

//! Angular speed of the local system relative the base system, i.e. "Omega[cBase]".
//! The vector is expressed in the base system and we use 3,2,1 rotation scheme, i.e.
//! "Omega[cBase][cBase] = H[cBase] . vphi[cBase]"
const double3 Omega_H321(const double3& phi,
			 const double3& vphi);

//! Angular speed of the local system relative the base system, i.e. "Omega[cBase]".
//! The vector is expressed in the local system and we use 3,2,1 rotation scheme, i.e.
//! "Omega[cBase][this] = U[cBase] . vphi[cBase]"
const double3 Omega_U321(const double3& phi,
			 const double3& vphi);


//! Angular acc. of the local system relative the base system. The vector 
//! is expressed in the local system and we use 3,2,1 rotation scheme. We have: 
//! "vOmega[cBase][cBase,this] =  vOmega[cBase][this,this] =
//! vU[cBase] . vphi[cBase] + U[cBase] . aphi[cBase]"
const double3 vOmega_U321(const double3& phi,
			  const double3& vphi,
			  const double3& aphi);

//! Angular acc. of the local system relative the base system. The vector 
//! is expressed in the base system and we use 3,2,1 rotation scheme. We have: 
//! "vOmega[cBase][cBase,cBase] =  vOmega[cBase][this,cBase] =
//! vH[cBase] . vphi[cBase] + H[cBase] . aphi[cBase]"
const double3 vOmega_H321(const double3& phi,
			  const double3& vphi,
			  const double3& aphi);

//! Transformation matrix from system cBase to local system.
//! The local system has the x-axis in radial direction (cylindrical coordinates), i.e.
//! rotation around the z-axis.
inline const double33 Acyl(const double3& RC);

//! Transformation matrix from system cBase to local system.
//! The local system has the x-axis in radial direction (cylindrical coordinates), i.e.
//! rotation around the z-axis.
inline const double33 Acyl(const double& RC_2);


//! Transformation from cylindrical coordinates to Cartesian coordinates.
inline const double3 RC2R(const double3& RC);

//! Transformation from cylindrical coordinates and its derivative to 
//! velocity in Cartesian coordinates.
inline const double3 vRC2vR(const double3& RC,
			       const double3& vRC);

//! Transformation from cylindrical coordinates and its 1-st & 2-nd derivative to 
//! acceleration in Cartesian coordinates.
const double3 aRC2aR(    const double3& RC, 
			 const double3& vRC,
			 const double3& aRC);

//! Transformation from Cartesian coordinates to cylindrical coordinates.
//! Note that the angle, i.e. component 2, will be within [-pi, pi].
//! This means the function is not an inverse function to RC2R where the angle
//! can be larger. An requirement is that both x and y components of R cannot 
//! be zero at the same time.
inline const double3 R2RC(const double3& R);

//! Transformation from acc. in Cartesian coordinates to acc. in cylindrical coordinates.
const double3 aR2aRC(const double3& aR,
		     const double3& RC, 
		     const double3& vRC);


//! Transformation of the elements of a symmetric matrix due to a rotation, i.e.
//! "A . Asym . invA". The rotation matrix A must Transpose[A] == Inverse[A], i.e.
//! orthogonal.
void RotSym(const double axx, // Input parameters 
            const double axy,
            const double axz,
            const double ayy,
            const double ayz,
            const double azz,
            const double33& A,
            double& bxx,     // Output parameters 
            double& bxy,
            double& bxz,
            double& byy,
            double& byz,
            double& bzz);



//! Transform a moment from system c1 to system c2.
//! With other words we have a force and moment couple acting at the
//! origin of system c1, then we calculate a the corresponding force and moment
//! but acting at origin of system c2.
//! This works independent of what system the vectors are expressed in.
inline void TFMc1toc2(const double3& F_In,
		      const double3& M_c1_In,
		      const double3& c2_R_c1_In,
		      // Output parameter
		      double3& M_c2_Out);


//! Transform a moment from system c1 to system c2.
//! With other words we have a force and moment couple acting at the
//! origin of system c1, then we calculate a the corresponding force and moment
//! but acting at origin of system c2.
//! The components are also transformed from system c1 to c2.
inline void TFMc1toc2(const double3& F_c1_In,
		      const double3& M_c1_c1_In,
		      const double3& c2_R_c1_c1_In,
		      const double33& c2_A_c1_In,
      		      // Output parameters
		      double3& F_c2_Out,
		      double3& M_c2_c2_Out);



//! Transform a moment from system c2 to system c1.
//! With other words we have a force and moment couple acting at the
//! origin of system c2, then we calculate a the corresponding force and moment
//! but acting at origin of system c1.
//! This works independent of what system the vectors are expressed in.
inline void TFMc2toc1(const double3& F_In,
		      const double3& M_c2_In,
		      const double3& c2_R_c1_In,
		      // Output parameter
		      double3& M_c1_Out);


//! Transform a moment from system c2 to system c2.
//! With other words we have a force and moment couple acting at the
//! origin of system c2, then we calculate a the corresponding force and moment
//! but acting at origin of system c1.
//! The components are also transformed from system c2 to c1.
inline void TFMc2toc1(const double3& F_c2_In,
		      const double3& M_c2_c2_In,
		      const double3& c2_R_c1_c1_In,
		      const double33& c2_A_c1_In,
      		      // Output parameters
		      double3& F_c1_Out,
		      double3& M_c1_c1_Out);


//! Create a skew symmetric matrix. That is Cross(v, x) = Skew(v)*x;
//! This function logically belongs to lightmat.
inline void MakeSkewMatrix(const double3& v, double33& m);


////////////////////////////////////////////////////////////////////////
//
//! \class intN
//!
//!
//!  This class describes an array of int.
//! 
//! 
//! 
//! 
//!

class intN {

// Friend class.
friend class aPackBuf ;


public:

    //! Constructor.
    intN(const int lenIn);

    //! Constructor. Initialize with var.
    intN(const int lenIn, const int var);

    //! Copy constructor.
    intN(const intN& var);

    //! Destructor.
    ~intN();

    //! Assignment operator.
    intN& operator=(const intN& var);

    //! Assignment operator. All elements get the value var.
    intN& operator=(const int var);

    //! Access element i of type int.
    int& operator()(const int i);

    //! Access element i of type int. Cannot be used as lvalue.
    inline int operator()(const int i) const;

    //! Gives the length of the dimension.
    inline int length() const;

private:

    //! Number of components. Data index starts from 1.
    const int len;

    //! Pointer to data area. Data starts from index 0.
    int* data;


};



////////////////////////////////////////////////////////////////////////
//
//! \class doubleN
//!
//!
//!  This class describes an array of double.
//! 
//! 
//! 
//! 
//!

class doubleN {

// Friend class.
friend class aPackBuf ;
//! M == M 
friend bool operator==(const doubleN& a, const doubleN& b);

public:

    //! Empty constructor. Len is set to zero. Resize is expected to be called later.
    doubleN( );

    //! Constructor. No initialization of elements.
    doubleN(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN(const int lenIn, const double var);

    //doubleN(const int lenIn, double* dataPtr); Overloading problem.
    // Constructor. Initialize with external data storage.

    //! Copy constructor.
    doubleN(const doubleN& var);

    //! Destructor.
    ~doubleN();

    //! Assignment operator.
    doubleN& operator=(const doubleN& var);

    //! Assignment operator. All elements get the value var.
    doubleN& operator=(const double var);

    //! Access element i of type double.
    inline double& operator()(const int i);

    //! Access element i of type double. Cannot be used as lvalue.
    inline double operator()(const int i) const;

   

    //! Gives the length of the dimension.
    inline int length() const;

    //! Resize the vector. No initialization of elements.
    void resize(const int lenIn);

    //! Resize the vector.
    //! If the new length is larger than the old one then var is used as init value for new elements.
    void resize(const int lenIn, const double var);

    //! Resize the vector. Use external data storage.
    //! Note, it can onlybe done when created with constructor external data storage,
    //! or default constructor has been used.
    void resize4ExtMem(const int lenIn, double* dataPtr);

private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointer to data area. Data index starts from 0.
    double* data;

    //! If True then the data pointer is pointing to external memory
    //! and no local memory is allocated.
    int XMFlg;


};


////////////////////////////////////////////////////////////////////////
//
// .SS Declaration of functions
//
// Here we declare global functions related to doubleN.
// For the description we use the notation:  "M" a doubleN
//

 //! M == M 
 bool operator==(const doubleN& a, const doubleN& b);

////////////////////////////////////////////////////////////////////////
//
//! \class doubleN3
//!
//!
//!  This class describes an array of double3.
//! 
//! 
//! 
//! 
//!

class doubleN3 {

// Friend class.
friend class aPackBuf ;

public:

    //! Default constructor.
    doubleN3();

    //! Constructor.
    doubleN3(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN3(const int lenIn, const double var);

    //! Copy constructor.
    doubleN3(const doubleN3& var);

    //! Destructor.
    ~doubleN3();

    //! Initialization.
    void Init(const int lenIn);

    //! Initialization with var.
    void Init(const int lenIn, const double var);

    //! Assignment operator.
    doubleN3& operator=(const doubleN3& var);

    //! Assignment operator. All elements get the value var.
    doubleN3& operator=(const double var);

    //! Access element i1 of type double3.
    inline double3& operator()(const int i1);

    //! Access element i1 of type double3. Cannot be used as lvalue.
    inline const double3& operator()(const int i1) const;

    //! Access element [i1,i2].
    //! i1 in [1..N] and i2 in [1,2,3]
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline double& operator()(const int i1, const int i2);

    //! Access element [i1,i2]. Cannot be used as lvalue.
    //! i1 in [1..N] and i2 in [1,2,3]
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline const double operator()(const int i1, const int i2) const;

    //! Gives the length of the first dimension, i.e. i1.
    inline int length() const;

    //! Set the length of the first dimension, i.e. i1.
    inline void SetLength(const int lenIn);

private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointerto data area. Data index starts from 0.
    double3* data;


};





////////////////////////////////////////////////////////////////////////
//
//! \class doubleN33
//!
//!
//!  This class describes an array of double33.
//! 
//! 
//! 
//! 
//!

class doubleN33 {

// Friend class.
friend class aPackBuf ;

public:

    //! Default constructor.
    doubleN33();

    //! Constructor.
    doubleN33(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN33(const int lenIn, const double var);

    //! Copy constructor.
    doubleN33(const doubleN33& var);

    //! Destructor.
    ~doubleN33();

    //! Initialization
    void Init(const int lenIn);

    //! Initialization
    void Init(const int lenIn, const double var);

    //! Assignment operator.
    doubleN33& operator=(const doubleN33& var);

    //! Assignment operator. All elements get the value var.
    doubleN33& operator=(const double var);

    //! Access element i1 of type double33.
    //! i1 in [1..N]
    inline double33& operator()(const int i1);

    //! Access element i1 of type double33. Cannot be used as lvalue.
    //! i1 in [1..N]
    inline const double33& operator()(const int i1) const;

    //! Access element [i1,i2]. Cannot be used as lvalue.
    //! Not so efficient implemented right now.
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline const double3 operator()(const int i1, const int i2) const; 

    //! Access element [i1,i2,i3].
    //! i1 in [1..N] ,  i2 in [1,2,3] , and i3 in [1,2,3]
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline double& operator()(const int i1, const int i2, const int i3);

    //! Access element [i1,i2,i3]. Cannot be used as lvalue.
    //! i1 in [1..N] ,  i2 in [1,2,3] , and i3 in [1,2,3]
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline const double operator()(const int i1, const int i2, const int i3) const;

    //! Gives the length of the last dimension, i.e. i3.
    inline int length() const;

    //! Set the length of the first dimension, i.e. i1.
    inline void SetLength(const int lenIn);

private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointerto data area. Data index starts from 0.
    double33* data;


};




//! //////////////////////////////////////////////////////
//!
//! .SS Inline definitions
//!
//! We give the inline definitions in a separate file.
//!
#include "lightmat.icc"


//! ObjectMath definitions for code generation

//! Used in dynamic.cc and testContactArea.cc
//#define SetArray3(v,x,y,z) v.Set(x,y,z)

//#define Det(v) v.det()

//#define AbsVector(v) v.length()

#endif // _LIGHTMAT_H 
