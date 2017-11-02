#include "MathUtilityFunc.h"
#include <cassert>
#include "ErrorLog.h"
#include "newmatap.h"
#ifdef DEBUGFLG
#include<iostream>
#endif

const double tol = 1.0e-12;

bool failedFlg = false;

//! Return res = row*col
double ScalarMult(const RowVector& row, const ColumnVector& col)
{


#ifdef DEBUGFLG
    int nn = row.Ncols();
    assert(nn==col.Nrows());
    assert(nn==col.Storage());
    assert(nn==row.Storage());
#endif
    Real* prow = row.Store();
    Real* pcol = col.Store();
    double res = 0;
    int i = row.Ncols();
    while(i--) 
	res += prow[i] * pcol[i]; 

    return res;

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (row*col).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);

    if (fabs(rel)>tol) {
        Log2("ScalarMult:("+ToStr(row.Ncols())+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif

}


//! Return res = (row1+row2)*col
double ScalarMult(const RowVector& row1, const RowVector& row2, const ColumnVector& col)
{


#ifdef DEBUGFLG
    int nn = row1.Ncols();
    assert(nn==col.Nrows());
    assert(nn==row1.Storage());
    assert(nn==row2.Storage());
    assert(nn==col.Storage());
    assert(row2.Ncols()==nn);
#endif

    Real* prow1 = row1.Store();
    Real* prow2 = row2.Store();
    Real* pcol = col.Store();
    double res = 0;
    int i = row1.Ncols();
    while(i--) 
	res += (prow1[i]+prow2[i])*pcol[i]; 


    return res;

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = ((row1+row2)*col).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult 2 arg:("+ToStr(row1.Ncols())+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif


}



//! Return scalar res = col.t()*m1*col
double ScalarMult3(const Matrix& m1, const ColumnVector& col)
{

    const int nn=col.Nrows();

#ifdef DEBUGFLG
    assert(nn==m1.Ncols());
    assert(nn==m1.Nrows());
#endif

    Real* pm1 = m1.Store();
    Real* pcol = col.Store();
    double res = 0.0;

    int k = 0;
    for (int i=0; i<nn; i++) {
        double rowsum = 0.0;
        for (int j=0; j<nn; j++) {
            rowsum += pm1[k]*pcol[j]; 
            k++;
        }
        res += rowsum*pcol[i];
    }

#ifdef DEBUGFLG
    assert(k==m1.Storage());
#endif

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (col.t()*(m1*col)).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult3:("+ToStr(nn)+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif


    return res;
}


//! Return scalar res = col.t()*m1*col, where m1 is symmetric.
double ScalarMult3S(const SymmetricMatrix& m1, const ColumnVector& col)
{

    const int nn=col.Nrows();

#ifdef DEBUGFLG
    assert(nn==m1.Ncols());
    assert(nn==m1.Nrows());
#endif

    Real* pm1 = m1.Store();
    Real* pcol = col.Store();
    double res = 0.0;

    int k = 0;
    for (int i=0; i<nn; i++) {
        
        double rowsum = 0.0;
        // Off-diagonal elements
        for (int j=0; j<i; j++) {
            rowsum += pm1[k]*pcol[j]; 
            k++;
        }
        rowsum *= 2.0;

        // Diagonal element
        rowsum += pm1[k]*pcol[i];
        k++;

        res += rowsum*pcol[i];
    }

#ifdef DEBUGFLG
    assert(k==m1.Storage());
#endif

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (col.t()*(m1*col)).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult3S:("+ToStr(nn)+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
    }
#endif

    return res;
}


//! Return scalar res = col.t()*(m1+m1)*col
double ScalarMult3(const Matrix& m1, const Matrix& m2, const ColumnVector& col)
{

    const int nn=col.Nrows();

#ifdef DEBUGFLG
    assert(nn==m1.Ncols());
    assert(nn==m1.Nrows());
    assert(nn==m2.Ncols());
    assert(nn==m2.Nrows());
#endif

    Real* pm1 = m1.Store();
    Real* pm2 = m2.Store();
    Real* pcol = col.Store();
    double res = 0.0;

    int k = 0;
    for (int i=0; i<nn; i++) {
        double rowsum = 0.0;
        for (int j=0; j<nn; j++) {
            rowsum += (pm1[k]+pm2[k])*pcol[j]; 
            k++;
        }
        res += rowsum*pcol[i];
    }

#ifdef DEBUGFLG
    assert(k==m1.Storage());
    assert(k==m2.Storage());
#endif

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (col.t()*((m1+m2)*col)).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult3 2 arg:("+ToStr(nn)+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;

    }
    
#endif


    return res;
}


//! Return scalar res = col.t()*(m1+m2)*col, where m1 and m2 are symmetric.
double ScalarMult3S(const SymmetricMatrix& m1, const SymmetricMatrix& m2, const ColumnVector& col)
{

    const int nn=col.Nrows();

#ifdef DEBUGFLG
    assert(nn==m1.Ncols());
    assert(nn==m1.Nrows());
    assert(nn==m2.Ncols());
    assert(nn==m2.Nrows());
#endif

    Real* pm1 = m1.Store();
    Real* pm2 = m2.Store();
    Real* pcol = col.Store();
    double res = 0.0;

    int k = 0;
    for (int i=0; i<nn; i++) {

        double rowsum = 0.0;
        // Off-diagonal elements
        for (int j=0; j<i; j++) {
            rowsum += (pm1[k]+pm2[k])*pcol[j]; 
            k++;
        }
        rowsum *= 2.0;

        // Diagonal element
        rowsum += (pm1[k]+pm2[k])*pcol[i];
        k++;

        res += rowsum*pcol[i];
    }

#ifdef DEBUGFLG
    assert(k==m1.Storage());
    assert(k==m2.Storage());
#endif

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (col.t()*((m1+m2)*col)).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult3S 2 arg:("+ToStr(nn)+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
    }
#endif


    return res;
}







//! Return scalar res = col1.t()*m1*col2
double ScalarMult4(const ColumnVector& col1, const Matrix& m1, const ColumnVector& col2)
{

    const int nn=col1.Nrows();

#ifdef DEBUGFLG
    assert(nn==m1.Ncols());
    assert(nn==m1.Nrows());
    assert(nn==col2.Nrows());
#endif

    Real* pm1 = m1.Store();
    Real* pcol1 = col1.Store();
    Real* pcol2 = col2.Store();
    double res = 0.0;

    int k = 0;
    for (int i=0; i<nn; i++) {
        double rowsum = 0.0;
        for (int j=0; j<nn; j++) {
            rowsum += pm1[k]*pcol2[j]; 
            k++;
        }
        res += rowsum*pcol1[i];
    }

#ifdef DEBUGFLG
    assert(k==m1.Storage());
#endif

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (col1.t()*(m1*col2)).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult4:("+ToStr(nn)+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
    }
#endif

    return res;
}


//! Return scalar res = col1.t()*m1*col2, where m1 is symmetric.
double ScalarMult4S(const ColumnVector& col1, const SymmetricMatrix& m1, const ColumnVector& col2)
{

    const int nn=col1.Nrows();

#ifdef DEBUGFLG
    assert(nn==m1.Ncols());
    assert(nn==m1.Nrows());
    assert(nn==col2.Nrows());
#endif

    Real* pm1 = m1.Store();
    Real* pcol1 = col1.Store();
    Real* pcol2 = col2.Store();
    double res = 0.0;

    int k = 0;
    for (int i=0; i<nn; i++) {

        double rowsum = 0.0;
        // Off-diagonal elements
        double pcol1_i = pcol1[i];
        double pcol2_i = pcol2[i];
        for (int j=0; j<i; j++) {
            rowsum += pm1[k]*(pcol1_i*pcol2[j]+pcol1[j]*pcol2_i);
            k++;
        }

        // Diagonal element
        rowsum += pm1[k]*pcol1_i*pcol2_i;
        k++;

        res += rowsum;
    }

#ifdef DEBUGFLG
    assert(k==m1.Storage());
#endif

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (col1.t()*(m1*col2)).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult4S:("+ToStr(nn)+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
        cerr << "ScalarMult4S m1 = " << m1 <<endl;
        cerr << "ScalarMult4S col1 = " << col1 <<endl;
        cerr << "ScalarMult4S col2 = " << col2 <<endl;

    }
#endif


    return res;
}


//! Return scalar res = col1.t()*(m1+m1)*col2
double ScalarMult4(const ColumnVector& col1, const Matrix& m1, const Matrix& m2, const ColumnVector& col2)
{

    const int nn=col1.Nrows();

#ifdef DEBUGFLG
    assert(nn==m1.Ncols());
    assert(nn==m1.Nrows());
    assert(nn==m2.Ncols());
    assert(nn==m2.Nrows());
    assert(nn==col2.Nrows());
#endif

    Real* pm1 = m1.Store();
    Real* pm2 = m2.Store();
    Real* pcol1 = col1.Store();
    Real* pcol2 = col2.Store();
    double res = 0.0;

    int k = 0;
    for (int i=0; i<nn; i++) {
        double rowsum = 0.0;
        for (int j=0; j<nn; j++) {
            rowsum += (pm1[k]+pm2[k])*pcol2[j]; 
            k++;
        }
        res += rowsum*pcol1[i];
    }

#ifdef DEBUGFLG
    assert(k==m1.Storage());
    assert(k==m2.Storage());
#endif


#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (col1.t()*((m1+m2)*col2)).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult4 2 arg:("+ToStr(nn)+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif


    return res;
}


//! Return scalar res = col1.t()*(m1+m1)*col2, where m1 and m2 are symmetric.
double ScalarMult4S(const ColumnVector& col1, const SymmetricMatrix& m1, const SymmetricMatrix& m2, const ColumnVector& col2)
{

    const int nn=col1.Nrows();

#ifdef DEBUGFLG
    assert(nn==m1.Ncols());
    assert(nn==m1.Nrows());
    assert(nn==m2.Ncols());
    assert(nn==m2.Nrows());
    assert(nn==col2.Nrows());
#endif

    Real* pm1 = m1.Store();
    Real* pm2 = m2.Store();
    Real* pcol1 = col1.Store();
    Real* pcol2 = col2.Store();
    double res = 0.0;

    int k = 0;
    for (int i=0; i<nn; i++) {

        double rowsum = 0.0;
        // Off-diagonal elements
        double pcol1_i = pcol1[i];
        double pcol2_i = pcol2[i];
        for (int j=0; j<i; j++) {
            rowsum += (pm1[k]+pm2[k])*(pcol1_i*pcol2[j]+pcol1[j]*pcol2_i);
            k++;
        }

        // Diagonal element
        rowsum += (pm1[k]+pm2[k])*pcol1_i*pcol2_i;
        k++;

        res += rowsum;
    }

#ifdef DEBUGFLG
    assert(k==m1.Storage());
    assert(k==m2.Storage());
#endif

#ifdef IMPLEMENTTEST
    // Old expression
    double res2 = (col1.t()*((m1+m2)*col2)).AsScalar();
    double rel = (res2-res)/Max(0.5*fabs(res2+res), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("ScalarMult4S 2 arg:("+ToStr(nn)+")  "+ToStr(res2)+", "+ToStr(res)+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif

    return res;
}





//! col += row.t()*a
void MultTransposeAdd(const double a, const RowVector& row, ColumnVector& col)
{
#ifdef DEBUGFLG
    assert(row.Ncols() == col.Nrows());
#endif
#ifdef IMPLEMENTTEST
    ColumnVector res2 = col;
#endif


    Real* prow = row.Store();
    Real* pcol = col.Store();
    int i = row.Ncols();
    while(i--) 
	pcol[i] += prow[i] * a; 


#ifdef IMPLEMENTTEST
    ColumnVector res = col;
    // Orginal expression
    res2 += row.t()*a;
    double rel = (res2-res).NormFrobenius()/Max(0.5*(res2+res).NormFrobenius(), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("MultTransposeAdd: "+ToStr(res2.NormFrobenius())+", "+ToStr(res.NormFrobenius())+" , rel="+ToStr(rel));
        failedFlg = true;
    }
#endif


}



//! M += row.t()*fac*row    
//! It is assumed that a symmetric matrix is stored as lower triangular matrix,
void SymmetricByMultAdd(const RowVector& row, const double fac, SymmetricMatrix& M)
{
#ifdef DEBUGFLG
    assert(row.Ncols() == M.Nrows());
    assert(row.Ncols() == M.Ncols());
#endif

#ifdef IMPLEMENTTEST
    Matrix res2 = M;
#endif

    Real* prow = row.Store();
    Real* pM = M.Store();
    const int nn = row.Ncols();
    int k=0;
    for (int i=0; i<nn; i++) {
        for (int j=0; j<=i; j++) {
            pM[k] += prow[i]*prow[j]*fac; 
            k++;
        }
    }

#ifdef DEBUGFLG
    assert(k==M.Storage());
#endif



#ifdef IMPLEMENTTEST
    Matrix res = M;
    // Orginal expression
    res2 += row.t()*(fac*row);
    double rel = (res2-res).NormFrobenius()/Max(0.5*(res2+res).NormFrobenius(), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("SymmetricByMultAdd: "+ToStr(res2.NormFrobenius())+", "+ToStr(res.NormFrobenius())+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif

}

//! M += row.t()*fac*row, this for all 3 rows. Note, we assume that the array rows are of length 3.
//! It is assumed that a symmetric matrix is stored as lower triangular matrix,
void SymmetricByMultAdd1(const RowVector* row, const double fac, SymmetricMatrix& M)
{
#ifdef DEBUGFLG
    int Mrows = M.Nrows();
    int Mcols = M.Ncols();
    for (int hh=0; hh<3; hh++) {
        assert(row[hh].Ncols() == Mrows);
        assert(row[hh].Ncols() == Mcols);
    }
#endif

#ifdef IMPLEMENTTEST
    Matrix res2 = M;
#endif

    Real* prow0 = row[0].Store();
    Real* prow1 = row[1].Store();
    Real* prow2 = row[2].Store();
    Real* pM = M.Store();
    const int nn = row[0].Ncols();
    int k=0;
    for (int i=0; i<nn; i++) {
        for (int j=0; j<=i; j++) {
            double mel = prow0[i]*prow0[j]; 
            mel += prow1[i]*prow1[j]; 
            mel += prow2[i]*prow2[j]; 
            mel *= fac;

            pM[k] += mel;
            k++;
        }
    }

#ifdef DEBUGFLG
    assert(k==M.Storage());
#endif


#ifdef IMPLEMENTTEST
    Matrix res = M;
    // Orginal expression
    res2 += row[0].t()*(fac*row[0]);
    res2 += row[1].t()*(fac*row[1]);
    res2 += row[2].t()*(fac*row[2]);
    double rel = (res2-res).NormFrobenius()/Max(0.5*(res2+res).NormFrobenius(), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("SymmetricByMultAdd1: "+ToStr(res2.NormFrobenius())+", "+ToStr(res.NormFrobenius())+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif

}



//! M += SD.t()*SD where SD is a m x n matrix and M is a n x n symmetric matrix.
//! It is assumed that a symmetric matrix is stored as lower triangular matrix,
void SymmetricByMultAdd2(const Matrix& SD, SymmetricMatrix& M)
{
#ifdef DEBUGFLG
    assert(SD.Ncols() == M.Nrows());
    assert(SD.Ncols() == M.Ncols());
#endif
#ifdef IMPLEMENTTEST
    Matrix res2 = M;
#endif

    Real* pSD = SD.Store();
    Real* pSDT = pSD;
    Real* pM = M.Store();
    const int nn = SD.Ncols();
    const int mm = SD.Nrows();
    int kk=0;
    for (int i=0; i<nn; i++) {
        for (int j=0; j<=i; j++) {
            double me = 0.0;
            for (int k=0; k<mm; k++) {
                int off = k*nn;
                me += pSDT[off+i]*pSD[off+j];
            }
            pM[kk] += me;
            kk++;
        }
    }

#ifdef DEBUGFLG
    assert(kk==M.Storage());
#endif

#ifdef IMPLEMENTTEST
    Matrix res = M;
    // Orginal expression
    res2 += SD.t()*SD;
    double rel = (res2-res).NormFrobenius()/Max(0.5*(res2+res).NormFrobenius(), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("SymmetricByMultAdd2: "+ToStr(res2.NormFrobenius())+", "+ToStr(res.NormFrobenius())+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif

}


#if 0
// BUG this is not true, the result is not symmetric

//! M += SD2.t()*SD1 where SD1 and SD2 are m x n matrix, and M is a n x n symmetric matrix.
//! It is assumed that a symmetric matrix is stored as lower triangular matrix,
void SymmetricByMultAdd3(const Matrix& SD1, const Matrix& SD2, SymmetricMatrix& M)
{
    assert(0); // Not correct theory or implementation.

#ifdef DEBUGFLG
    assert(SD1.Ncols() == M.Nrows());
    assert(SD1.Ncols() == M.Ncols());
    assert(SD2.Ncols() == M.Nrows());
    assert(SD2.Ncols() == M.Ncols());
    assert(SD1.Nrows() == SD2.Nrows());
    assert(SD1.Ncols() == SD2.Ncols());
#endif

#ifdef IMPLEMENTTEST
    Matrix res2 = M;
#endif

    double* pSD1 = SD1.Store();
    double* pSD2 = SD2.Store();
    double* pM = M.Store();
    const int nn = SD1.Ncols();
    const int nk = SD1.Nrows();
    int kk=0;
    for (int i=0; i<nn; i++) {
        for (int j=0; j<=i; j++) {
            double sumel = 0.0;
            for (int k=0; k<nk; k++) {
                int off = k*nn;
                sumel += pSD2[off+i]*pSD1[off+j];
            }
            pM[kk] += sumel;
            kk++;
        }
    }

#ifdef DEBUGFLG
    assert(kk==M.Storage());
#endif


#ifdef IMPLEMENTTEST
    Matrix res = M;
    // Orginal expression
    res2 += SD2.t()*SD1;
    double rel = (res2-res).NormFrobenius()/Max(0.5*(res2+res).NormFrobenius(), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("SymmetricByMultAdd3: "+ToStr(res2.NormFrobenius())+", "+ToStr(res.NormFrobenius())+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif

}
#endif




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
void SymmetricByMultAdd4(const double lambda, const double mu, const Matrix& SD, SymmetricMatrix& M)
{
    // CM*SD gives for n = 3, where m -> mu, and a -> lambda.
    // 2*m*s(1,1)+a*(s(1,1)+s(2,1)+s(3,1)),  2*m*s(1,2)+a*(s(1,2)+s(2,2)+s(3,2)),  2*m*s(1,3)+a*(s(1,3)+s(2,3)+s(3,3))
    // 2*m*s(2,1)+a*(s(1,1)+s(2,1)+s(3,1)),  2*m*s(2,2)+a*(s(1,2)+s(2,2)+s(3,2)),  2*m*s(2,3)+a*(s(1,3)+s(2,3)+s(3,3))
    // 2*m*s(3,1)+a*(s(1,1)+s(2,1)+s(3,1)),  2*m*s(3,2)+a*(s(1,2)+s(2,2)+s(3,2)),  2*m*s(3,3)+a*(s(1,3)+s(2,3)+s(3,3))
    // m*s(4,1),                             m*s(4,2),                             m*s(4,3)
    // m*s(5,1),                             m*s(5,2),                             m*s(5,3)
    // m*s(6,1),                             m*s(6,2),                             m*s(6,3)

#ifdef DEBUGFLG
    assert(SD.Ncols() == M.Nrows());
    assert(SD.Ncols() == M.Ncols());
    assert(SD.Nrows() == 6);
#endif
#ifdef IMPLEMENTTEST
    Matrix res2 = M;
#endif

    const double mu2 = mu*2;

    Real* pSD = SD.Store();
    Real* pM = M.Store();
    const int nn = SD.Ncols();
    int kk=0;
    for (int i=0; i<nn; i++) {
        for (int j=0; j<=i; j++) {
            double sd0j = pSD[j];
            double sd1j = pSD[nn+j];
            double sd2j = pSD[2*nn+j];
            double sds = lambda*(sd0j+sd1j+sd2j);
            double me = pSD[i]*(mu2*sd0j + sds);
            me += pSD[nn+i]*(mu2*sd1j + sds);
            me += pSD[2*nn+i]*(mu2*sd2j + sds);

            me += mu*pSD[3*nn+i]*pSD[3*nn+j];
            me += mu*pSD[4*nn+i]*pSD[4*nn+j];
            me += mu*pSD[5*nn+i]*pSD[5*nn+j];

            pM[kk] += me;
            kk++;
        }
    }

#ifdef DEBUGFLG
    assert(kk==M.Storage());
#endif


#ifdef IMPLEMENTTEST
    Matrix res = M;
    // Orginal expression
    SymmetricMatrix CM(6); CM = 0.0;
    CM(1,1) = lambda+2*mu;
    CM(2,2) = lambda+2*mu;
    CM(3,3) = lambda+2*mu;
    CM(1,2) = lambda;
    CM(1,3) = lambda;
    CM(2,1) = lambda;
    CM(3,1) = lambda;
    CM(3,2) = lambda;
    CM(2,3) = lambda;
    CM(4,4) = mu;
    CM(5,5) = mu;
    CM(6,6) = mu;
//! lambda+2*mu,  lambda,      lambda,      0,           0,          0 
//! lambda,       lambda+2*mu, lambda,      0,           0,          0 
//! lambda,       lambda       lambda+2*mu, 0,           0,          0
//! 0             0            0            mu           0           0
//! 0             0            0            0            mu          0
//! 0             0            0            0            0           mu

    res2 += SD.t()*(CM*SD);
    double rel = (res2-res).NormFrobenius()/Max(0.5*(res2+res).NormFrobenius(), 1.0e-50);
    if (fabs(rel)>tol) {
        Log2("SymmetricByMultAdd4: "+ToStr(res2.NormFrobenius())+", "+ToStr(res.NormFrobenius())+" , rel="+ToStr(rel));
        failedFlg = true;
    }
    
#endif

}


double mrand() {

//     static double i=1.0;
//     return i++;

    return ((double)rand())/RAND_MAX;
}

bool testMathUtilityFunc()
{


    Log2("Start special testMathUtilityFunc() test");
#ifndef IMPLEMENTTEST
    Error("Tests testMathUtilityFunc() not activated");
#endif

    const int nn = 6;
    
    ColumnVector col1(nn); col1 = 0.0;
    ColumnVector col2(nn); col2 = 0.0;
    ColumnVector cres(nn); cres = 0.0;
    
    RowVector row1(nn); row1 = 0.0;
    RowVector row2(nn); row2 = 0.0;
    Matrix m1(nn,nn); m1 = 0.0;
    Matrix m2(nn,nn); m1 = 0.0;
    Matrix mres(nn,nn); mres = 0.0;
    SymmetricMatrix ms1(nn); ms1 = 0.0;
    SymmetricMatrix ms2(nn); ms2 = 0.0;
    SymmetricMatrix mress(nn); mress = 0.0;

    int k=1;
    for (int i=1; i<=nn; i++) {
        col1(i) = mrand();
        col2(i) = mrand();
        row1(i) = mrand();
        row2(i) = mrand();
        for (int j=1; j<=nn; j++) {
            m1(i,j) = mrand();
            m2(i,j) = mrand();
            ms1(i,j) = mrand();
            ms2(i,j) = mrand();
            k++;
        }
    }
        
    RowVector vrow[3];
    vrow[0] = row1;
    vrow[1] = row2;
    vrow[2] = row1;

    double lambda = 1.8797e30;
    double mu = 1.333377777e30;

    double resd_unused=0.0;


    //! Functions that calculate scalar products without memory management.

    //! Return scalar product of these two vectors. res = row*col
    //double ScalarMult(const RowVector& row, const ColumnVector& col);
    resd_unused += ScalarMult(row1, col1);


    //! Return scalar product res = (row1+row2)*col
    //double ScalarMult(const RowVector& row1, const RowVector& row2, const ColumnVector& col);
    resd_unused += ScalarMult(row1, row2, col1);


    //! Return scalar res = col.t()*m1*col
    //double ScalarMult3(const Matrix& m1, const ColumnVector& col);
    resd_unused += ScalarMult3(m1, col1);


    //! Return scalar res = col.t()*m1*col, where m1 is symmetric.
    //double ScalarMult3S(const SymmetricMatrix& m1, const ColumnVector& col);
    resd_unused += ScalarMult3S(ms1, col1);


    //! Return scalar res = col.t()*(m1+m1)*col
    //double ScalarMult3(const Matrix& m1, const Matrix& m2, const ColumnVector& col);
    resd_unused += ScalarMult3(m1, m2, col1);


    //! Return scalar res = col.t()*(m1+m1)*col, where m1 and m2 are symmetric.
    //double ScalarMult3S(const SymmetricMatrix& m1, const SymmetricMatrix& m2, const ColumnVector& col);
    resd_unused += ScalarMult3S(ms1, ms2, col1);


    //! Return scalar res = col1.t()*m1*col2
    //double ScalarMult4(const ColumnVector& col1, const Matrix& m1, const ColumnVector& col2);
    resd_unused += ScalarMult4(col1, m1, col2);


    //! Return scalar res = col1.t()*m1*col2, where m1 is symmetric.
    // double ScalarMult4S(const ColumnVector& col1, const SymmetricMatrix& m1, const ColumnVector& col2);
    resd_unused += ScalarMult4S(col1, ms1, col2);


    //! Return scalar res = col1.t()*(m1+m1)*col2
    //double ScalarMult4(const ColumnVector& col1, const Matrix& m1, const Matrix& m2, const ColumnVector& col2);
    resd_unused += ScalarMult4(col1, m1, m2, col2);


    //! Return scalar res = col1.t()*(m1+m1)*col2, where m1 and m2 are symmetric.
    //double ScalarMult4S(const ColumnVector& col1, 
    //                    const SymmetricMatrix& m1, const SymmetricMatrix& m2, const ColumnVector& col2);
    resd_unused += ScalarMult4S(col1, ms1, ms2, col2);






    //! Functions that calculate vectors  without memory management.

    double a = 1.567934;
    double fac = 1.567934;

    //! col += row.t()*a
    //void MultTransposeAdd(const double a, const RowVector& row, ColumnVector& col);
    MultTransposeAdd(a, row1, cres);




    //! Functions that calculate symmetric matrixes  without memory management.


    //! M += row.t()*fac*row    
    //! It is assumed that a symmetric matrix is stored as lower triangular matrix,
    //void SymmetricByMultAdd(const RowVector& row, const double fac, SymmetricMatrix& M);
    SymmetricByMultAdd(row1, fac, mress);


    //! M += row.t()*fac*row, this for all 3 rows. Note, we assume that the array of rows are of length 3.
    //! It is assumed that a symmetric matrix is stored as lower triangular matrix,
    //void SymmetricByMultAdd1(const RowVector* row, const double fac, SymmetricMatrix& M);
    SymmetricByMultAdd1(vrow, fac, mress);


    //! M += SD.t()*SD where SD is a m x n matrix and M is a n x n symmetric matrix.
    //! It is assumed that a symmetric matrix is stored as lower triangular matrix,
    //void SymmetricByMultAdd2(const Matrix& SD, SymmetricMatrix& M);
    SymmetricByMultAdd2(m1, mress);


    //! M += SD2.t()*SD1 where SD1 and SD2 are m x n matrix, and M is a n x n symmetric matrix.
    //! It is assumed that a symmetric matrix is stored as lower triangular matrix,
    //void SymmetricByMultAdd3(const Matrix& SD1, const Matrix& SD2, SymmetricMatrix& M);
    //SymmetricByMultAdd3(m1, m2, mress);


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
    //void SymmetricByMultAdd4(const double lambda, const double mu, const Matrix& SD, SymmetricMatrix& M);
    SymmetricByMultAdd4(lambda, mu, m1, mress);



    Log2("End special testMath() test");

    return !failedFlg;
}

