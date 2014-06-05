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

#include "lightmat.h"
#include "dsyevq3.h"
#include "dsyevv3.h"
#include <iostream>

//! ////////////////////////////////////////////////////////////
//!
//! .SS Constants
//!
//! These are the defined constants
//!
class double33s;
class float33s;

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
    inline double33s& operator=(const double33s& m) ;

    //! Assignment. 
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //double33s& operator=(const doubleN3& m);

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

    //! Accepts a number from 1 to 6, returns the first component of pair in  11,22,33,12,23,13
    static int notationOrder1(int internalOrder);

    //! Accepts a number from 1 to 6, returns the second component of pair in  11,22,33,12,23,13
    static int notationOrder2(int internalOrder);


   

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

class float33s { 
    friend class ConverterFloat33sDouble33s;
    friend class aPackBuf;
    friend class double33s;
 public:   

    inline   float33s()
#ifdef DEBUG_LIGHTMAT
        : x11(1e38f), x22(1e38f), x33(1e38f), x12(1e38f), x23(1e38f), x13(1e38f)
#endif
        {};

//! Constructor that initializes the components to the given value.
 inline explicit  float33s(const float f_in);

 //! Access elements. Can not be used as lvalue.
 inline float operator()(const int row, const int col) const;


 //! Access elements. Can be used as lvalue.
    inline float& operator()(const int row, const int col) ;

#ifdef USE_IMPLICIT_CONV     
 //! Constructor to convert double33s to  float33s 
 inline float33s (const double33s & d)  {
        x11=static_cast<float>(d.x11);
        x22=static_cast<float>(d.x22);
        x33=static_cast<float>(d.x33);
        x12=static_cast<float>(d.x12);
        x23=static_cast<float>(d.x23);
        x13=static_cast<float>(d.x13);
     };
#endif


 // friend inline const float33s operator+(const float33s& a, const float33s& b); 

  //! Constructor that initializes the components to the given values
 inline float33s(const float a11, const float a22, const float a33,
	     const float a12, const float a23, const float a13);

 friend inline const float33s operator+(const  float33s  & a, const   float33s & b);
 friend inline const float33s operator-(const  float33s  & a, const   float33s & b);

 friend inline bool operator==(const float33s & a, const float33s & b);
 friend inline bool operator!=(const float33s& a, const float33s& b);
   //! M += M
 inline float33s& operator+=(const float33s& m);
 //! M -= M
 inline float33s& operator-=(const float33s& m);

 //! Assignment.
    inline float33s& operator=(const float33s& m) ;
    friend Bstring ToStr(const float33s& val);

     
 //! Protected data area.
 private:
    float x11, x22, x33, x12, x23, x13; 

};

#ifndef USE_IMPLICIT_CONV
class ConverterFloat33sDouble33s {
 public:
inline static  float33s convertToFloat33s(const double33s & d) {
    float33s f(static_cast<const float>(d.x11),
        static_cast<const float>(d.x22),
        static_cast<const float>(d.x33),
        static_cast<const float>(d.x12),
        static_cast<const float>(d.x23),
        static_cast<const float>(d.x13));
    return f;
};
inline static double33s convertToDouble33s(const float33s & f) {
    double33s d(f.x11,f.x22,f.x33,f.x12,f.x23,f.x13);
    return d;
};
};
#endif
////////////////////////////////////////////////////////////////////////
//
// .SS Method definitions
//

//! Constructor that initializes the components to the given value.
inline double33s::double33s(const double val) :
                 x11(val),
                 x22(val),
                 x33(val),
                 x12(val),
                 x23(val),
                 x13(val){}

double33s::double33s(const double a11, const double a22, const double a33,
		     const double a12, const double a23, const double a13) :
x11(a11),
    x22(a22),
    x33(a33),
    x12(a12),
    x23(a23),
    x13(a13){}

//! Copy constructor.
double33s::double33s(const double33s& val):
x11(val.x11),
    x22(val.x22),
    x33(val.x33),
    x12(val.x12),
    x23(val.x23),
    x13(val.x13){}

#ifdef USE_IMPLICIT_CONV

//! Conversion constructor from  float33s.
inline double33s::double33s(const float33s& val):
                 x11(val.x11),
                 x22(val.x22),
                 x33(val.x33),
                 x12(val.x12),
                 x23(val.x23),
                 x13(val.x13){}

#endif

inline double& double33s::operator()(const int row, const int col) {
	switch(row) {
	case 1:
	    switch(col) {
	    case 1: return x11;
	    case 2: return x12;
            case 3: return x13;
            default:assert(false);
	    }
	case 2:
	    switch(col) {
	    case 1: return x12;
	    case 2: return x22;
            case 3: return x23;
            default:assert(false);
	    }
	case 3:
	    switch(col) {
	    case 1: return x13;
	    case 2: return x23;
            case 3: return x33;
            default:assert(false);
	    }
        default:assert(false);

	}

        return x11;// Never reaches here
}

//! Access elements. Can not be used as lvalue.
inline double double33s::operator()(const int row, const int col) const	{
    switch(row) {
    case 1:
	switch(col) {
	case 1: return x11;
	case 2: return x12;
        case 3: return x13;
        default:assert(false);
	}
    case 2:
	switch(col) {
	case 1: return x12;
	case 2: return x22;
        case 3: return x23;
        default:assert(false);
	}
    case 3:
	switch(col) {
	case 1: return x13;
	case 2: return x23;
        case 3: return x33;
        default:assert(false);
        }
    default:assert(false);
    }
    assert(0);
    return x11;
}

inline double33s& double33s::operator=(const double33s& m) {
    x11 = m.x11;
    x22 = m.x22;
    x33 = m.x33;
    x12 = m.x12;
    x23 = m.x23;
    x13 = m.x13;
    return (*this);
}

inline double33s& double33s::operator=(const double a) {
    x11 = a;
    x22 = a;
    x33 = a;
    x12 = a;
    x23 = a;
    x13 = a;
    return (*this);
}

inline double33s& double33s::operator+=(const double33s& m) {
    x11 += m.x11;
    x22 += m.x22;
    x33 += m.x33;
    x12 += m.x12;
    x23 += m.x23;
    x13 += m.x13;
    return (*this);
}

inline double33s& double33s::operator+=(const double a) {
    x11 += a;
    x22 += a;
    x33 += a;
    x12 += a;
    x23 += a;
    x13 += a;
    return (*this);
}

inline double33s& double33s::operator-=(const double33s& m) {
    x11 -= m.x11;
    x22 -= m.x22;
    x33 -= m.x33;
    x12 -= m.x12;
    x23 -= m.x23;
    x13 -= m.x13;
    return (*this);
}

inline double33s& double33s::operator-=(const double a) {
    x11 -= a;
    x22 -= a;
    x33 -= a;
    x12 -= a;
    x23 -= a;
    x13 -= a;
    return (*this);
}

inline double33s& double33s::operator*=(const double a) {
    x11 *= a;
    x22 *= a;
    x33 *= a;
    x12 *= a;
    x23 *= a;
    x13 *= a;
    return (*this);
}

inline double33s& double33s::operator/=(const double a) {
    x11 /= a;
    x22 /= a;
    x33 /= a;
    x12 /= a;
    x23 /= a;
    x13 /= a;
    return (*this);
}

//! Fills a matrix of type double33s from two vectors diag=[11, 22, 33]  overdiag[12, 23, 13]
inline void double33s::Set(const double3& diag, const double3& overdiag) {
    Set(diag(1),diag(2),diag(3), overdiag(1),overdiag(2),overdiag(3));
}

inline void double33s::Get(double3& diag, double3& overdiag) const {
    Get(diag(1),diag(2),diag(3), overdiag(1),overdiag(2),overdiag(3));
}

//! Fills a matrix of type double33s from a C-array (6 elements)
//! in the order [11, 22, 33, 12, 23, 13]
inline void double33s::Set(const double data[]) {
    x11 = data[0];
    x22 = data[1];
    x33 = data[2];
    x12 = data[3];
    x23 = data[4];
    x13 = data[5];
}

//! Get matrix data into a C-array (6 elements)
//! in the order [11, 22, 33, 12, 23, 13]
inline void double33s::Get( double data[]) const {
    data[0] = x11;
    data[1] = x22;
    data[2] = x33;
    data[3] = x12;
    data[4] = x23;
    data[5] = x13;
}

//! Fills a matrix of type double33s from the elements or components.
inline void double33s::Set(const double a11, const double a22, const double a33,
			   const double a12, const double a23, const double a13) {
    x11 = a11; x22 = a22; x33=a33; x12=a12; x23=a23; x13=a13;    
}

//! Get the components a11, a12, ... a33 from a matrix of type double33s.
inline void double33s::Get(double& a11,  double& a22,  double& a33,
			   double& a12,  double& a23,  double& a13) const {
    a11 = x11; a22 = x22; a33=x33; a12=x12; a23=x23; a13=x13;
}


//! Dot product: M * V
const double3 operator*(const double33s& a, const double3& b) {
    return double3(a.x11 * b(1) + a.x12 * b(2) + a.x13 * b(3),
		   a.x12 * b(1) + a.x22 * b(2) + a.x23 * b(3),
		   a.x13 * b(1) + a.x23 * b(2) + a.x33 * b(3)
		   );
}

//! Dot product: V * M
const double3 operator*(const double3& a, const double33s& b) {
    return b * a;
}

//! returns the size of a double33s
inline int sizeOf(const double33s &) {
    return 6*sizeof(double);
}

////////////////////////////////////////////////////////////////////////
//
// .SS Definition of functions
//
// Here we declare global functions related to double33s.
// For the description we use the notation: "V" denotes a double3, 
// "s" scalar, and "M" a double33s
//

//! -M
inline const double33s operator-(const double33s& m) {
    return double33s(-m.x11, -m.x22, -m.x33, -m.x12, -m.x23, -m.x13);
}

//! s * M
inline const double33s operator*(const double s, const double33s& m) {
    return double33s(s*m.x11, s*m.x22, s*m.x33, s*m.x12, s*m.x23, s*m.x13);
}

//! M * s 
inline const double33s operator*(const double33s& m, const double s) {
    return s * m;
}

//! M / s 
inline const double33s operator/(const double33s& m, const double s){
    return double33s(m.x11/s, m.x22/s, m.x33/s, m.x12/s, m.x23/s, m.x13/s);
}


//! M + M 
inline const double33s operator+(const double33s& a, const double33s& b){
    return double33s(a.x11+b.x11,a.x22+b.x22,a.x33+b.x33,a.x12+b.x12,a.x23+b.x23,a.x13+b.x13);
}



//! M + s 
inline const double33s operator+(const double33s& m, const double s) {
    return double33s(m.x11+s, m.x22+s, m.x33+s, m.x12+s, m.x23+s, m.x13+s);
}

//! s + M
inline const double33s operator+(const double s, const double33s& m){
    return m + s;
}

//! M - M 
inline const double33s operator-(const double33s& a, const double33s& b){
    return double33s(a.x11-b.x11,a.x22-b.x22,a.x33-b.x33,a.x12-b.x12,a.x23-b.x23,a.x13-b.x13);
}

//! M - s 
inline const double33s operator-(const double33s& m, const double s){
    return double33s(m.x11-s, m.x22-s, m.x33-s, m.x12-s, m.x23-s, m.x13-s);
}

//! s - M
inline const double33s operator-(const double s, const double33s& m){
    return double33s(s-m.x11, s-m.x22, s-m.x33, s-m.x12, s-m.x23, s-m.x13);
}

//! Dot product: M * M
inline const double33 operator*(const double33s& a, const double33s& b){
    return a.get_as_double33()*b.get_as_double33();
}


//! M == M (no fuzz for comparison) 
inline bool operator==(const double33s& a, const double33s& b) {
    return ((a.x11 == b.x11) && (a.x22 == b.x22) && (a.x33 == b.x33) && 
	    (a.x12 == b.x12) && (a.x23 == b.x23) && (a.x13 == b.x13));
}

//! M == s (no fuzz for comparison) 
inline bool operator==(const double33s& a, const double b) {
    return ((a.x11 == b) && (a.x22 == b) && (a.x33 == b) && 
	    (a.x12 == b) && (a.x23 == b) && (a.x13 == b));
}

//! s == M (no fuzz for comparison) 
inline bool operator==(const double a, const double33s& b) {
    return (b == a);
}

//! M != M (no fuzz for comparison) 
inline bool operator!=(const double33s& a, const double33s& b) {
    return !(a==b);
}

double double33s::det() const {
    return  x11*(x22*x33-x23*x23)
          -x12*(x12*x33-x23*x13)
          +x13*(x12*x23-x22*x13);
}


//!  B = Transpose(A) * M * A
inline void double33s::calc_Transform1(const double33& A, double33s& B) const 
{
    double a11, a12, a13, a21, a22, a23, a31, a32, a33;
    A. Get( a11, a12, a13,
            a21, a22, a23,
            a31, a32, a33);
    double a21x12_a31x13 = (a21*x12+a31*x13);
    double a22x12_a32x13 = (a22*x12+a32*x13);
    double a23x12_a33x13 = (a23*x12+a33*x13);
    double a21x22_a31x23 = (a21*x22+a31*x23);
    double a21x23_a31x33 = (a21*x23+a31*x33);
    double a22x22_a32x23 = (a22*x22+a32*x23);
    double a22x23_a32x33 = (a22*x23+a32*x33);
    B.Set(
	  a11*a11*x11+2*a11*a21x12_a31x13+a21*a21x22_a31x23+a31*a21x23_a31x33,
	  a12*a12*x11+2*a12*a22x12_a32x13+a22*a22x22_a32x23+a32*a22x23_a32x33,
	  a13*a13*x11+2*a13*a23x12_a33x13+a23*(a23*x22+a33*x23)+a33*(a23*x23+a33*x33),
	  a12*a21x12_a31x13+a11*(a12*x11+a22x12_a32x13)+a22*a21x22_a31x23+a32*a21x23_a31x33,
	  a13*a22x12_a32x13+a12*(a13*x11+a23x12_a33x13)+a23*a22x22_a32x23+a33*a22x23_a32x33,
	  a13*a21x12_a31x13+a11*(a13*x11+a23x12_a33x13)+a23*a21x22_a31x23+a33*a21x23_a31x33
	  );
    
}

//!  B = A * M * Transpose(A)
inline void double33s::calc_Transform2(const double33& A, double33s& B) const 
{
    double a11, a12, a13, a21, a22, a23, a31, a32, a33;

    // In order to get it on the form implemented in calc_Transform1 we get the transformed
    // componets.
    A. Get( a11, a21, a31,
            a12, a22, a32,
            a13, a23, a33);

    double a21x12_a31x13 = (a21*x12+a31*x13);
    double a22x12_a32x13 = (a22*x12+a32*x13);
    double a23x12_a33x13 = (a23*x12+a33*x13);
    double a21x22_a31x23 = (a21*x22+a31*x23);
    double a21x23_a31x33 = (a21*x23+a31*x33);
    double a22x22_a32x23 = (a22*x22+a32*x23);
    double a22x23_a32x33 = (a22*x23+a32*x33);

    B.Set(
	  a11*a11*x11+2*a11*a21x12_a31x13+a21*a21x22_a31x23+a31*a21x23_a31x33,
	  a12*a12*x11+2*a12*a22x12_a32x13+a22*a22x22_a32x23+a32*a22x23_a32x33,
	  a13*a13*x11+2*a13*a23x12_a33x13+a23*(a23*x22+a33*x23)+a33*(a23*x23+a33*x33),
	  a12*a21x12_a31x13+a11*(a12*x11+a22x12_a32x13)+a22*a21x22_a31x23+a32*a21x23_a31x33,
	  a13*a22x12_a32x13+a12*(a13*x11+a23x12_a33x13)+a23*a22x22_a32x23+a33*a22x23_a32x33,
	  a13*a21x12_a31x13+a11*(a13*x11+a23x12_a33x13)+a23*a21x22_a31x23+a33*a21x23_a31x33
	  );
    
}

const double33 double33s::get_as_double33() const {
    return double33(
		    x11, x12, x13,
		    x12, x22, x23,
		    x13, x23, x33);
}

void double33s::calc_eigensystem( double3& ev, double3* V) const
{
    if(0 == V) {
	calc_eigenvalues(ev);
	return;
    }

    double A[3][3], Q[3][3], w[3];

    Get(A[0][0], A[1][1], A[2][2],A[0][1],A[1][2],A[0][2]);

    if(-1 == dsyevq3( A,  Q, w )) { // QL does not converge
	dsyevv3( A,  Q, w );
    };
    ev(1) = w[0]; ev(2) = w[1]; ev(3) = w[2];
    
    for (int i = 0; i < 3; i++) {
	double3& Vi = V[i]; Vi(1) = Q[0][i]; Vi(2) = Q[1][i]; Vi(3) = Q[2][i];
    }
}

//! M / M
//! Elementwise division, mainly used for testing stress tensors.
inline const double33s elementwise_Div(const double33s& a, const double33s& b) {
    return double33s(a.x11/b.x11,a.x22/b.x22,a.x33/b.x33,a.x12/b.x12,a.x23/b.x23,a.x13/b.x13);
}

//*****************************************************************************************
inline const double33s Max(const double33s& a,const double33s& b)
{
    return double33s(a.x11>b.x11?a.x11:b.x11,
                     a.x22>b.x22?a.x22:b.x22,
                     a.x33>b.x33?a.x33:b.x33,
                     a.x12>b.x12?a.x12:b.x12,
                     a.x23>b.x23?a.x23:b.x23,
                     a.x13>b.x13?a.x13:b.x13);
}
inline const double33s Min(const double33s& a,const double33s& b)
{
    return double33s(a.x11<b.x11?a.x11:b.x11,
                     a.x22<b.x22?a.x22:b.x22,
                     a.x33<b.x33?a.x33:b.x33,
                     a.x12<b.x12?a.x12:b.x12,
                     a.x23<b.x23?a.x23:b.x23,
                     a.x13<b.x13?a.x13:b.x13);
}

inline const double33s fabs(const double33s& a)
{
    return double33s(fabs(a.x11),
                     fabs(a.x22),
                     fabs(a.x33),
                     fabs(a.x12),
                     fabs(a.x23),
                     fabs(a.x13));
}

//*******************************************************************************************


//! Output on ostream
inline std::ostream& operator<<(std::ostream& o, const double33s& a) {    
    o << "s11=" << a.x11 << ", " 
      << "s22=" << a.x22 << ", " 
      << "s33=" << a.x33 << ", " 
      << "s12=" << a.x12 << ", " 
      << "s23=" << a.x23 << ", " 
      << "s13=" << a.x13; 
    return o;
 }

//! Conversion to string.
Bstring ToStr(const double33s& val);


//! Functions for  float33s



inline float33s::float33s(const float a11, const float a22, const float a33,
                          const float a12, const float a23, const float a13) :
                x11(a11),
                x22(a22),
                x33(a33),
                x12(a12),
                x23(a23),
                x13(a13){}


//! Constructor that initializes the components to the given value.
inline float33s::float33s(const float val) :
                x11(val),
                x22(val),
                x33(val),
                x12(val),
                x23(val),
                x13(val){}

inline const float33s operator+(const float33s& a, const float33s& b){
    return float33s(a.x11+b.x11,a.x22+b.x22,a.x33+b.x33,a.x12+b.x12,a.x23+b.x23,a.x13+b.x13);
}

inline const float33s operator-(const float33s& a, const float33s& b){
    return float33s(a.x11-b.x11,a.x22-b.x22,a.x33-b.x33,a.x12-b.x12,a.x23-b.x23,a.x13-b.x13);
}

inline bool operator==(const float33s& a, const float33s& b) {
    return ((a.x11 == b.x11) && (a.x22 == b.x22) && (a.x33 == b.x33) && 
	    (a.x12 == b.x12) && (a.x23 == b.x23) && (a.x13 == b.x13));
}

//! M != M (no fuzz for comparison) 
inline bool operator!=(const float33s& a, const float33s& b) {
    return !(a==b);
}

inline float33s& float33s::operator=(const float33s& m) {
    x11 = m.x11;
    x22 = m.x22;
    x33 = m.x33;
    x12 = m.x12;
    x23 = m.x23;
    x13 = m.x13;
    return (*this);
}

//! Conversion to string.
Bstring ToStr(const float33s& val);

inline float33s& float33s::operator+=(const float33s& m) {
    x11 += m.x11;
    x22 += m.x22;
    x33 += m.x33;
    x12 += m.x12;
    x23 += m.x23;
    x13 += m.x13;
    return (*this);
}

inline float33s& float33s::operator-=(const float33s& m) {
    x11 -= m.x11;
    x22 -= m.x22;
    x33 -= m.x33;
    x12 -= m.x12;
    x23 -= m.x23;
    x13 -= m.x13;
    return (*this);
}





inline float& float33s::operator()(const int row, const int col) {
	switch(row) {
	case 1:
	    switch(col) {
	    case 1: return x11;
	    case 2: return x12;
            case 3: return x13;
            default:assert(false);
	    }
	case 2:
	    switch(col) {
	    case 1: return x12;
	    case 2: return x22;
            case 3: return x23;
            default:assert(false);
	    }
	case 3:
	    switch(col) {
	    case 1: return x13;
	    case 2: return x23;
	    case 3: return x33;		
            default:assert(false);
	    }
        default:assert(false);
	}
	assert(0);
	return x11;
}


inline float float33s::operator()(const int row, const int col) const {
	switch(row) {
	case 1:
	    switch(col) {
	    case 1: return x11;
	    case 2: return x12;
            case 3: return x13;
            default:assert(false);
	    }
	case 2:
	    switch(col) {
	    case 1: return x12;
	    case 2: return x22;
            case 3: return x23;
            default:assert(false);
	    }
	case 3:
	    switch(col) {
	    case 1: return x13;
	    case 2: return x23;
            case 3: return x33;
            default:assert(false);
	    }
        default:assert(false);
	}
	assert(0);
	return x11;
}




////////////////////////////////////////////////////////////////////////
//
//! \class doubleN33s
//!
//!
//!  This class describes an array of double33s.
//! 
//! 
//! 
//! 
//!

class doubleN33s {

// Friend class.
friend class aPackBuf ;

public:

    //! Default constructor.
    doubleN33s();

    //! Constructor.
    doubleN33s(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN33s(const int lenIn, const double var);

    //! Copy constructor.
    doubleN33s(const doubleN33s& var);

    //! Destructor.
    ~doubleN33s();

    //! Initialization
    void Init(const int lenIn);

    //! Initialization
    void Init(const int lenIn, const double var);

    //! Assignment operator.
    doubleN33s& operator=(const doubleN33s& var);

    //! Assignment operator. All elements get the value var.
    doubleN33s& operator=(const double var);

    //! Access element i1 of type double33s.
    //! i1 in [1..N]
    inline double33s& operator()(const int i1);

    //! Access element i1 of type double33s. Cannot be used as lvalue.
    //! i1 in [1..N]
    inline const double33s& operator()(const int i1) const;

    //! Gives the length of the last dimension, i.e. i3.
    inline int length() const;

    //! Set the length of the first dimension, i.e. i1.
    inline void SetLength(const int lenIn);

private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointerto data area. Data index starts from 0.
    double33s* data;


};



// Inline definitions for doubleN33s

inline double33s& doubleN33s::operator()(const int i1)
{
#ifdef DEBUGFLG
    assert(i1>0 && i1<=len);
#endif
    return (data[i1-1]);
}

inline const double33s& doubleN33s::operator()(const int i1) const
{
#ifdef DEBUGFLG
    assert(i1>0 && i1<=len);
#endif
    return (data[i1-1]);
}

inline int doubleN33s::length() const
{
    return len;
}



#endif // _DOUBLE33S_H_
