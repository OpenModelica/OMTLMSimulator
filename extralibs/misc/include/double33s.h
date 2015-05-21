#ifndef _DOUBLE33S_H_
#define _DOUBLE33S_H_

//! //////////////////////////////////////////////////////////////////////
//!
//! .SS Functionality
//!
//! The double33s class is a variant of double33 that aims at symmetric
//! 3x3 matrices.
//!
//! @author
//! I. Nakhimovski 2008-01-17

#include "lightmatConfig.h"
#include <ostream>
#include "Bstring.h"

#include "double33.h"
// For conversion

//! ////////////////////////////////////////////////////////////
//!
//! .SS Constants
//!
//! These are the defined constants
//!
class double33s;
class float33s;
class double33;
class double3;

//! A zero matrix 
extern const double33s zerodouble33s;

//! A identity matrix 
extern const double33s Idouble33s;




class double33s  {
    friend class float33s;
     friend class ConverterFloat33sDouble33s;
// Friend Inline functions.
    friend inline const double33s operator-(const double33s& m);
    friend inline const double33s operator*(const double s, const double33s& m);
    friend inline const double33s operator*(const double33s& m, const double s);
    friend inline const double33 operator*(const double33s& a, const double33s& b);
    friend inline const double3 operator*(const double33s& a, const double3& b);
    friend inline const double3 operator*(const double3& a, const double33s& b);
    friend inline const double33s operator/(const double33s& m, const double s);
    friend inline const double33s operator/(const double33s& a, const double33s& b);
    friend inline const double33s operator+(const double33s& a, const double33s& b);
    friend inline const double33s operator+(const double33s& m, const double s);
    friend inline const double33s operator+(const double s, const double33s& m);
    friend inline const double33s operator-(const double33s& a, const double33s& b);
    friend inline const double33s operator-(const double33s& m, const double s);
    friend inline const double33s operator-(const double s, const double33s& m);
    friend inline bool operator==(const double33s& a, const double33s& b);
    friend inline bool operator==(const double33s& a, const double b);
    friend inline bool operator==(const double a, const double33s& b);
    friend inline bool operator!=(const double33s& a, const double33s& b);
    friend inline const double33s elementwise_Div(const double33s& a, const double33s& b);
    friend inline std::ostream& operator<<(std::ostream& o, const double33s& M);
    friend inline const double33s Max(const double33s& a,const double33s& b);//***************************
    friend inline const double33s Min(const double33s& a,const double33s& b);//***************************
    friend inline const double33s fabs(const double33s& a);//**************************
    friend Bstring ToStr(const double33s& val);

    friend double MaxRelAbsError(const double33s& a, const double33s& b, const double tol);


    friend class aPackBuf ;

public:

    //! Empty constructor. Does NOT initialize to any special value.
    inline double33s()
#ifdef DEBUG_LIGHTMAT
        : x11(1e301), x22(1e301), x33(1e301), x12(1e301), x23(1e301), x13(1e301)
#endif
        {};

    //! Constructor that initializes the components to the given value.
    inline explicit double33s(const double val);

    //! Constructor that initializes the components to the given values
    inline double33s(const double a11, const double a22, const double a33,
	     const double a12, const double a23, const double a13);

    //! Copy constructor.
    inline double33s(const double33s& val);

#ifdef USE_IMPLICIT_CONV
    //! Conversion constructor from  float33s.
    inline double33s(const float33s& val);
#endif

    //! Access elements. Can be used as lvalue.
    inline double& operator()(const int row, const int col) ;

    //! Access elements. Can not be used as lvalue.
    inline double operator()(const int row, const int col) const;

    //     //! Access column exists for double33. Can not be used as lvalue. 
    //     //! Not so efficient implemented rigth now. 
    //     //! I skip implementation for now as this has never been used.
    //     inline const double3 operator()(const int row) const; 

    //! Assignment.
    inline double33s& operator=(const double a);
    
    //! Assignment.
    inline double33s& operator=(const double33s& m);

    //! M += M
    inline double33s& operator+=(const double33s& m);

    //! M += s
    inline double33s& operator+=(const double a);

    //! M -= M
    inline double33s& operator-=(const double33s& m);

    //! M -= s
    inline double33s& operator-=(const double a);

    //! M *= s
    inline double33s& operator*=(const double as);

    //    //! M *= M
    //    double33s& operator*=(const double33s& m);

    //! M /= s
    inline double33s& operator/=(const double a);

    //! Fills a matrix of type double33s from two vectors diag=[11, 22, 33]  overdiag[12, 23, 13]
    inline void Set(const double3& diag, const double3& overdiag);

    //! Get matrix data into two vectors diag=[11, 22, 33]  overdiag[12, 23, 13]
    inline void Get(double3& diag, double3& overdiag) const;

    //! Fills a matrix of type double33s from a C-array (6 elements)
    //! in the order [11, 22, 33, 12, 23, 13]
    inline void Set(const double data[]);

    //! Get matrix data into a C-array (6 elements)
    //! in the order [11, 22, 33, 12, 23, 13]
    inline void Get( double data[]) const;

    //! Fills a matrix of type double33s from the elements or components.
    inline void Set(const double a11, const double a22, const double a33,
		    const double a12, const double a23, const double a13);

    //! Get the components a11, a12, ... a33 from a matrix of type double33s.
    inline void Get(double& a11,  double& a22,  double& a33,
		    double& a12,  double& a23,  double& a13) const;

    //! Convert to the general double33
    inline const double33 get_as_double33() const;

    //! Convert to the general double33
    inline operator const double33() const { 
     	return get_as_double33(); 
    } 

    //! Return the transpose of the matrix.
    inline const double33s T() const { return *this; }

    //! Return the determinant of matrix.
    inline double det() const;

    //! Compute the eigenvalues (fast analytical algorithm, may be inacurate for some special cases)
    void calc_eigenvalues(double3& ev) const;


    //! Compute the eigenvalues & eigensystem (iterative QL algorithm, high accuracy).
    //! A * V[i] = ev(i+1) * V[i],  for i={0,1,2} and A == *this
    //! Note that right now the order of eigenvalues returned from  calc_eigenvalues
    //! may be different from the order returned from this call.    
    inline void calc_eigensystem( double3& ev, double3* V) const;

    //! Index to access calc_vonMises()
    static const int calc_vonMises_index_7=7;

     //! Compute effective (von Misses) stress
    inline double calc_vonMises() const;

    //! Compute transformation: B = Transpose(A) * M * A
    //! For the cases where M is a tensor in coordinate system c1
    //! and A is an orthogonal transformation matrix c1_A_c2
    //! this results in coordinate transformation to system c2.
    inline void calc_Transform1(const double33& A, double33s& B) const;
    inline double33s calc_Transform1(const double33& A) const {
	double33s B;
	calc_Transform1(A, B);
	return B;
    };    

    //! Compute transformation: B = A * M * Transpose(A)
    //! For the cases where M is a tensor in coordinate system c2
    //! and A is an orthogonal transformation matrix c1_A_c2
    //! this results in coordinate transformation to system c1.
    inline void calc_Transform2(const double33& A, double33s& B) const;
    inline double33s calc_Transform2(const double33& A) const {
	double33s B;
	calc_Transform2(A, B);
	return B;
    };    

    //! limitElements reduces the elements of the tensor to be within [-lim, lim].
    //! lim is assumed to be positive.
    //! Returns true if any of the elements was updated.
    inline bool limitElements(const double lim) {
	bool ret;
	ret = limitX(x11, lim);
	ret |= limitX(x22, lim);
	ret |= limitX(x33, lim);
	ret |= limitX(x12, lim);
	ret |= limitX(x23, lim);
	ret |= limitX(x13, lim);
	return ret;
    };

    //! Test functionality is used to test correctness of the implementation/compilation
    static int test_functionality();


    //! Return true if all components are zeroes
    inline bool isZero() const;

    //!  Return true if at least one components is non zero
    inline bool isNonZero() const;
   

protected: 

    //! limitX is an utility function that updates x to be within [-lim, lim]
    //! lim is assumed to be positive.
    //! Returnes true if x was updated.
    inline static bool limitX(double&x, const double lim) {
	if( x > lim) { x = lim; return true; }
	if( x < -lim) { x = -lim; return true; }
	return false;
    }

 protected:
    //! Protected data area.
    double x11, x22, x33, x12, x23, x13; 
   
    
};

#include "float33s.h"
#include "double33s.icc"
#include "float33s.inc"



#endif // _DOUBLE33S_H_
