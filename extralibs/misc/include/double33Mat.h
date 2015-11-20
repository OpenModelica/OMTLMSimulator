#ifndef _DOUBLE33MAT_H
#define _DOUBLE33MAT_H

#include <cstdlib> // Among many things overload abs(..) for integer types.
#include <cmath>   // Among many things overload abs(..) for float types.
#include <string>
#include "strConv.h"


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


//! A zero matrix 
extern const double33Mat zerodouble33;

//! A identity matrix 
extern const double33Mat Idouble33;





class double33Mat {

// Friend Inline functions.
//!friend inline const double33 Transpose(const double33& a);
friend inline const double33Mat operator-(const double33Mat& m);
friend inline const double33Mat operator*(const double s, const double33Mat& m);
friend inline const double33Mat operator*(const double33Mat& m, const double s);
friend inline const double33Mat operator*(const double33Mat& a, const double33Mat& b);
friend const double3Vec operator*(const double33Mat& a, const double3Vec& b);
friend const double3Vec operator*(const double3Vec& a, const double33Mat& b);
friend inline const double33Mat operator/(const double33Mat& m, const double s);
friend inline const double33Mat operator+(const double33Mat& a, const double33Mat& b);
friend inline const double33Mat operator+(const double33Mat& m, const double s);
friend inline const double33Mat operator+(const double s, const double33Mat& m);
friend inline const double33Mat operator-(const double33Mat& a, const double33Mat& b);
friend inline const double33Mat operator-(const double33Mat& m, const double s);
friend inline const double33Mat operator-(const double s, const double33Mat& m);
friend inline bool operator==(const double33Mat& a, const double33Mat& b);
friend inline bool operator==(const double33Mat& a, const double b);
friend inline bool operator==(const double a, const double33Mat& b);
friend inline bool operator!=(const double33Mat& a, const double33Mat& b);
friend const double33Mat Max(const double33Mat& a,const double33Mat& b);  //*****************************************
friend const double33Mat Min(const double33Mat& a,const double33Mat& b);  //*****************************************
//friend const double33Mat fabs(const double33Mat& a); //*****************************************
friend std::ostream& operator<<(std::ostream& o, const double33Mat& M);
 friend std::string ToStr(const double33Mat& val);
friend const double3Vec ATophi321(const double33Mat& A);
friend const double3Vec ATophi123(const double33Mat& A);
friend void AToEuler(const double33Mat& A, double& q1, double& q2, double& q3, double& q4);

friend double MaxRelAbsError(const double33Mat& a, const double33Mat& b, const double tol);

friend class aPackBuf ;

public:

    //! Empty constructor. Does NOT initialize to any special value.
    inline double33Mat(){};

    //! Constructor that initializes the components to the given value.
    explicit inline double33Mat(const double val);

    //! Constructor that initializes the components to the given values
    double33Mat(const double a11, const double a12, const double a13,
	     const double a21, const double a22, const double a23,
	     const double a31, const double a32, const double a33);

    //! Copy constructor.
    double33Mat(const double33Mat& val);

    //! Access elements. Can be used as lvalue.
    inline double& operator()(const int row, const int col);

    //! Access elements. Can not be used as lvalue.
    inline double operator()(const int row, const int col) const;

    //! Access row. Can not be used as lvalue.
    //! Not so efficient implemented rigth now.
    //! Note, should be replaced by function below. This because of it is unclear what this single index operator should do.
    inline const double3Vec operator()(const int row) const;

    //! Return row as double3.
    inline const double3Vec GetRow(const int row) const;

    //! Return col as double3.
    inline const double3Vec GetCol(const int col) const;


    //! Assignment.
    inline double33Mat& operator=(const double a);
    
    //! Assignment.
    double33Mat& operator=(const double33Mat& m);

    //! Assignment. 
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //double33& operator=(const doubleN3& m);

    //! M += M
    inline double33Mat& operator+=(const double33Mat& b);

    //! M += s
    inline double33Mat& operator+=(const double s);

    //! M -= M
    inline double33Mat& operator-=(const double33Mat& b);

    //! M -= s
    inline double33Mat& operator-=(const double s);

    //! M *= s
    inline double33Mat& operator*=(const double s);

    //! M *= M
    double33Mat& operator*=(const double33Mat& b);

    //! M /= s
    inline double33Mat& operator/=(const double s);

    //! Fills a matrix of type double33 from the row vectors a1, a2, a3
    //! Same as SetRows
    inline void Set(const double3Vec& a1, 
                    const double3Vec& a2, 
                    const double3Vec& a3);

    //! Fills a matrix of type double33 from the row vectors r1, r2, r3
    inline void SetRows(const double3Vec& r1,
                        const double3Vec& r2,
                        const double3Vec& r3);

    //! Fills a matrix of type double33 from the column vectors c1, c2, c3
    inline void SetCols(const double3Vec& c1,
                        const double3Vec& c2,
                        const double3Vec& c3);

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
    inline const double33Mat T() const;
    
    //! Set matrix to be M = exp(w^t)
    void SetExpMatrix(const double3Vec& w, const double t);

    //! Return true if all components are zeroes
    inline bool isZero() const;

    //!  Return true if at least one components is non zero
    inline bool isNonZero() const;

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
inline const double33Mat operator-(const double33Mat& m);

//! s * M
inline const double33Mat operator*(const double s, const double33Mat& m);

//! M * s 
inline const double33Mat operator*(const double33Mat& m, const double s);

//! M / s 
inline const double33Mat operator/(const double33Mat& m, const double s);

//! M + M 
inline const double33Mat operator+(const double33Mat& a, const double33Mat& b);

//! M + s 
inline const double33Mat operator+(const double33Mat& m, const double s);

//! s + M
inline const double33Mat operator+(const double s, const double33Mat& m);

//! M - M 
inline const double33Mat operator-(const double33Mat& a, const double33Mat& b);

//! M - s 
inline const double33Mat operator-(const double33Mat& m, const double s);

//! s - M
inline const double33Mat operator-(const double s, const double33Mat& m);

//! Dot product: M * M
inline const double33Mat operator*(const double33Mat& a, const double33Mat& b);

//! M == M (no fuzz for comparison) 
inline bool operator==(const double33Mat& a, const double33Mat& b);

//! M == s (no fuzz for comparison) 
inline bool operator==(const double33Mat& a, const double b);

//! s == M (no fuzz for comparison) 
inline bool operator==(const double a, const double33Mat& b);

//! M != M (no fuzz for comparison) 
inline bool operator!=(const double33Mat& a, const double33Mat& b);

//! Outer product
inline const double33Mat OuterProduct(const double3Vec& a, const double3Vec& b);

//inline const double33 Transpose(const double33& a);
// Gives the transposed matrix

//! Output on ostream
std::ostream& operator<<(std::ostream& o, const double33Mat& M);

//! Conversion to string.
std::string ToStr(const double33Mat& val);



//! Return the maximum absolute value of the relative error.   MaxRelAbsError = abs(a-b)*2.0/(abs(a)+abs(b)+tol)
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double a, const double b, const double tol);

//! Return the maximum absolute value of the relative errors of each component.
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double3Vec& a, const double3Vec& b, const double tol);

//! Return the maximum absolute value of the relative errors of each component.
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double33Mat& a, const double33Mat& b, const double tol);


};


//! //////////////////////////////////////////////////////
//!
//! .SS Inline definitions
//!
//! We give the inline definitions in a separate file.
//!
#include "double33Mat.icc"

#endif // _DOUBLE33_H
