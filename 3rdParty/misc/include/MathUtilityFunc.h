#ifndef _MATHUTILITYFUNC_H
#define _MATHUTILITYFUNC_H

#include "newmat.h"
#include "lightmat.h"


//! Functions that calculate scalar products without memory management.

//! Return scalar product of these two vectors. res = row*col
double ScalarMult(const RowVector& row, const ColumnVector& col);


//! Return scalar product res = (row1+row2)*col
double ScalarMult(const RowVector& row1, const RowVector& row2, const ColumnVector& col);


//! Return scalar res = col.t()*m1*col
double ScalarMult3(const Matrix& m1, const ColumnVector& col);


//! Return scalar res = col.t()*m1*col, where m1 is symmetric.
double ScalarMult3S(const SymmetricMatrix& m1, const ColumnVector& col);


//! Return scalar res = col.t()*(m1+m1)*col
double ScalarMult3(const Matrix& m1, const Matrix& m2, const ColumnVector& col);


//! Return scalar res = col.t()*(m1+m1)*col, where m1 and m2 are symmetric.
double ScalarMult3S(const SymmetricMatrix& m1, const SymmetricMatrix& m2, const ColumnVector& col);


//! Return scalar res = col1.t()*m1*col2
double ScalarMult4(const ColumnVector& col1, const Matrix& m1, const ColumnVector& col2);


//! Return scalar res = col1.t()*m1*col2, where m1 is symmetric.
double ScalarMult4S(const ColumnVector& col1, const SymmetricMatrix& m1, const ColumnVector& col2);


//! Return scalar res = col1.t()*(m1+m1)*col2
double ScalarMult4(const ColumnVector& col1, const Matrix& m1, const Matrix& m2, const ColumnVector& col2);


//! Return scalar res = col1.t()*(m1+m1)*col2, where m1 and m2 are symmetric.
double ScalarMult4S(const ColumnVector& col1, const SymmetricMatrix& m1, const SymmetricMatrix& m2, const ColumnVector& col2);






//! Functions that calculate vectors  without memory management.

//! col += row.t()*a
void MultTransposeAdd(const double a, const RowVector& row, ColumnVector& col);






//! Functions that calculate symmetric matrixes  without memory management.


//! M += row.t()*fac*row    
//! It is assumed that a symmetric matrix is stored as lower triangular matrix,
void SymmetricByMultAdd(const RowVector& row, const double fac, SymmetricMatrix& M);


//! M += row.t()*fac*row, this for all 3 rows. Note, we assume that the array of rows are of length 3.
//! It is assumed that a symmetric matrix is stored as lower triangular matrix,
void SymmetricByMultAdd1(const RowVector* row, const double fac, SymmetricMatrix& M);


//! M += SD.t()*SD where SD is a m x n matrix and M is a n x n symmetric matrix.
//! It is assumed that a symmetric matrix is stored as lower triangular matrix,
void SymmetricByMultAdd2(const Matrix& SD, SymmetricMatrix& M);


//! M += SD.t()*CM*SD where CM is the 6 by 6 constitutive matrix for isotropic material, SD is a 6 by n matrix,
//! and M is a symmetric n by n matrix.
//!
//! Constitutive matrix for isotropic material are shown below. It is a symmetric matrix and is sparse. 
//! These properties can be utilized when computing contributions to the stiffness matrix. 
//! lambda+2*mu,  lambda,      lambda,      0,           0,          0 
//! lambda,       lambda+2*mu, lambda,      0,           0,          0 
//! lambda,       lambda       lambda+2*mu, 0,           0,          0
//! 0             0            0            mu           0           0
//! 0             0            0            0            mu          0
//! 0             0            0            0            0           mu
//!
//! It is assumed that a symmetic matrix is stored as lower triangular matrix.
void SymmetricByMultAdd4(const double lambda, const double mu, const Matrix& SD, SymmetricMatrix& M);



//! Tests function for above.
//! If failed, it return false. If OK returns true.
//! Diagnostics output on log file using Log2().
bool testMathUtilityFunc();

#endif


