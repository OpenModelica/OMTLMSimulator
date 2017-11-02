#ifndef _DOUBLE33_H
#define _DOUBLE33_H

#include "lightmatConfig.h"

#include <cstdlib> // Among many things overload abs(..) for integer types.
#include <cmath>   // Among many things overload abs(..) for float types.
#include "Bstring.h"
#include "tostr.h"


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


//! A zero matrix 
extern const double33 zerodouble33;

//! A identity matrix 
extern const double33 Idouble33;





class double33 {

    // Friend Inline functions.
    //!friend inline const double33 Transpose(const double33& a);
    friend inline const double33 operator-(const double33& m);
    friend inline const double33 operator*(const double s, const double33& m);
    friend inline const double33 operator*(const double33& m, const double s);
    friend inline const double33 operator*(const double33& a, const double33& b);
    friend inline const double3 operator*(const double33& a, const double3& b);
    friend inline const double3 operator*(const double3& a, const double33& b);
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
    friend std::ostream& operator<<(std::ostream& o, const double33& M);
    friend Bstring ToStr(const double33& val);
    friend const double3 ATophi321(const double33& A);
    friend const double3 ATophi123(const double33& A);
    friend void AToEuler(const double33& A, double& q1, double& q2, double& q3, double& q4);

    friend double MaxRelAbsError(const double33& a, const double33& b, const double tol);

    friend class aPackBuf ;

public:

    //! Empty constructor. Does NOT initialize to any special value.
    double33(){};

    //! Constructor that initializes the components to the given value.
    explicit inline double33(const double val);

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

    //! Access row. Can not be used as lvalue.
    //! Not so efficient implemented rigth now.
    //! Note, should be replaced by function below. This because of it is unclear what this single index operator should do.
    inline const double3 operator()(const int row) const;

    //! Return row as double3.
    inline const double3 GetRow(const int row) const;

    //! Return col as double3.
    inline const double3 GetCol(const int col) const;


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

    //! Get all components as an C array.
    void Get(double * arr) const { for (int i=0; i<9; i++) arr[i]=x[i]; };

    //! Return the determinant of matrix.
    inline double det() const;

    //! Return the transpose of the matrix.
    inline const double33 T() const;
    
    //! Set matrix to be M = exp(w^t)
    void SetExpMatrix(const double3& w, const double t);

    // Compute SVD of the 3x3 matrix: A = U S Transpose(V), where U & V are unary (rotation) matrices
    // and S is diagonal (singular values)
    void calc_svd(double3 U[3], double3& S, double3 V[3]);

    // Compute rotation matrix U from polar decomposition of the 3x3 matrix:
    // A = P U = U P'
    void calc_polar_rotation(double33& U);

    // Compute left polar decomposition of the 3x3 matrix: A = P U,
    // P i positive semidefinite and U is unary (rotation) matrix
    void calc_left_polar(double33& P, double33& U);

    // Compute right polar decomposition of the 3x3 matrix: A = U P,
    // P i positive semidefinite and U is unary (rotation) matrix
    void calc_right_polar(double33& U, double33& P);

    //! Return true if all components are zeroes
    inline bool isZero() const;

    //!  Return true if at least one components is non zero
    inline bool isNonZero() const;

    //! True if any element is nan.
    bool isNan() const;


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

//! Dot product: M * V
inline const double3 operator*(const double33& a, const double3& b);

//! Dot product: V * M
inline const double3 operator*(const double3& a, const double33& b);

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

//! Max for each component separately.
const double33 Max(const double33& a,const double33& b);

//! Min for each component separately.
const double33 Min(const double33& a,const double33& b);

//! fabs on each component separately.
const double33 fabs(const double33& a);

//! Output on ostream
std::ostream& operator<<(std::ostream& o, const double33& M);

//! Conversion to string.
Bstring ToStr(const double33& val);



//! Return the maximum absolute value of the relative error.   MaxRelAbsError = abs(a-b)*2.0/(abs(a)+abs(b)+tol)
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double a, const double b, const double tol);

//! Return the maximum absolute value of the relative errors of each component.
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double3& a, const double3& b, const double tol);

//! Return the maximum absolute value of the relative errors of each component.
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double33& a, const double33& b, const double tol);






//! //////////////////////////////////////////////////////
//!
//! .SS Inline definitions
//!
//! We give the inline definitions in a separate file.
//!
#include "double33.icc"


#endif // _DOUBLE33_H
