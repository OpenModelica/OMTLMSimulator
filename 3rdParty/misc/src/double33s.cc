// The eigenvalue/eigensystem solvers are rewritten in C++/lightmat interface by I.Nakhimovski from:
// Joachim Kopp
// Numerical diagonalization of hermitian 3x3 matrices
// arXiv.org preprint: physics/0610206
// http://www.mpi-hd.mpg.de/~jkopp/3x3/

#include <cmath>
#include <cassert>
#include <iostream>
#include "double33s.h"
#include "mathdefs.h"

using namespace std;

static inline double sqr(const double x) { return x * x; }

static const double sqrt3 = sqrt(3.0);



Bstring ToStr(const double33s& val) 
{
    Bstring res;
    res = "( " + ToStr(val.x11) + " " + ToStr(val.x22) + " " + ToStr(val.x33) + "  \n";
    res+= "  " + ToStr(val.x12) + " " + ToStr(val.x23) + " " + ToStr(val.x13) + " )";
    return res;
}


double double33s::normForScaling() const
{
    // Smallest number for small matrices.
    // Largest number for large matrices.

    double data[6];
    Get(data);
    double theNorm;
    double minNorm=INFINITY;
    double maxNorm=0.0;

    for (int i=0; i<6; i++) {
        double data_abs = fabs(data[i]);
        if (data_abs!=0.0 && data_abs<minNorm) {
            minNorm = data_abs;
        }
        //if (data[i]!=0.0 && fabs(data[i])>maxNorm) {
        //     maxNorm = fabs(data[i]);
        // }
        maxNorm = Max(maxNorm, data_abs);
    }

    // If zero tensor, return zero.
    if (maxNorm==0.0) return 0.0;

    if (1.0/minNorm > maxNorm) {
        theNorm = minNorm;
    } 
    else {
        theNorm = maxNorm;
    }

    return theNorm;
}

void double33s::calc_eigenvaluesScaled(double3& ev) const 
{
    double theNorm = normForScaling();
    if (theNorm!=0.0) {
        // Non-zero tensor.
        double33s scaled=(*this) / theNorm;
        scaled.calc_eigenvalues(ev);
        ev *= theNorm;
    }
    else {
        // Zero tensor.
        ev = zerodouble3;
    }
}

void double33s::calc_eigenvalues(double3& ev) const 
{
    assert(isNonZero()); // Debug code, to be removed later.

    // analytical method is used (see dsyevc3.c)
  
    // Determine coefficients of characteristic poynomial. We write
    //         | a   d   f  |
    //  A =    | d*  b   e  |
    //         | f*  e*  c  |
    double de = x12*x23;                                   // d * e
    double dd = x12*x12;                                   // d^2
    double ee = x23*x23;                                   // e^2
    double ff = x13*x13;                                   // f^2

    double m  = x11 + x22 + x33;
    double c1 = (x11*x22 + x11*x33 + x22*x33) - (dd + ee + ff); // a*b + a*c + b*c - d^2 - e^2 - f^2
    double c0 = x33*dd + x11*ee + x22*ff - x11*x22*x33 - 2.0*x13*de;   // c*d^2 + a*e^2 + b*f^2 - a*b*c - 2*f*d*e
    

    double p = m*m - 3.0*c1;
    double q = m*(p - 1.5*c1) - 13.5*c0;  // m*(p - (3.0/2.0)*c1) - (27.0/2.0)*c0;
    double sqrt_p = sqrt(abs(p));

    double phi = 27.0*(0.25*c1*c1*(p-c1) + c0*(q+6.75*c0)); // 27.0*(0.25*sqr(c1)*(p-c1) + c0*(q+27.0/4.0*c0));
    phi = atan2(sqrt(abs(phi)), q)/3.0;
  
    double c = sqrt_p*cos(phi);
    double s = sqrt_p*sin(phi)/sqrt3;

    double tmp = (m-c)/3.0;
    ev.Set(tmp+c, tmp-s, tmp+s);

//     ev(2)  = (1.0/3.0)*(m - c);
//     ev(3)  = ev(2) + s;
//     ev(1)  = ev(2) + c;
//     ev(2) -= s;
}


void double33s::test_on_matrix(const double33s& A) {
    static int tstId=0;
    tstId++;
    double3 V[3], ev1;
//A=(A - 1e9)/1e16;
    cout << endl << "-- X"<<tstId<<" ------ library test for matrix: " << A <<endl;
    //if(A==0) return;
    /*
    A.calc_eigensystem(ev1, V);
    cout << " EV "<<ToStr(ev1)<<" "<<ToStr(ev1.norm2()-1);
    cout << " V "<< ToStr(V[0]) <<ToStr(V[1]) <<ToStr(V[2]);
    for(int i = 1; i <= 3; i++) {
        cout <<" " <<ToStr(V[i-1].norm2()-1)<< " "<<endl;


    }
    for(int i = 1; i <= 3; i++) {
        assert( fabs(V[i-1].norm2()-1)<1.4e-15);
    }
*/

    cout << "Determinant of A is " << A.det() << " Scale is " << A.normForScaling() << endl;
    double3 ev;
    A.calc_eigenvalues(ev);
    cout << "Eigenvalues (analytical) = " << ev  << endl;

    double3 evS;
    A.calc_eigenvaluesScaled(evS);
    cout << "Eigenvalues (scaled analytical) = " << evS  << endl;

    cout << "Eigenvalues product: " << ev(1) * ev(2)*ev(3)  << endl;

    A.calc_eigensystem(ev1, V);
    cout << "Eigenvalues (numerical) = " << ev1  << endl;
    for(int i = 1; i <= 3; i++) {
    cout << "Eigenvector " << i << ": " << V[i-1] << ", norm2: " << V[i-1].norm2() ;
	double3 vv = A * V[i-1] - ev1(i) * V[i-1];
    cout << " A * v - ev * v: " <<vv << endl;
    if(vv.norm2()/fabs(ev1(i)+1) > 1e-7) cout << "========== FAIL!!! ========" << vv.norm2() <<"/"<< fabs(ev1(i)+1)<<"="<< vv.norm2()/fabs(ev(i)+1)<< endl;
    } 


    A.calc_eigensystemScaled(ev1, V);
    cout << "Eigenvalues (scaled numerical) = " << ev1  << endl;
    for(int i = 1; i <= 3; i++) {
    cout << "Eigenvector " << i << ": " << V[i-1] << ", norm2: " << V[i-1].norm2() ;
    double3 vv = A * V[i-1] - ev1(i) * V[i-1];
    cout << "  A * v - ev * v: " <<vv << endl;
    if(vv.norm2()/fabs(ev1(i)+1) > 1e-7) cout << "========== FAIL!!! ========" << vv.norm2() <<"/"<< fabs(ev1(i)+1)<<"="<< vv.norm2()/fabs(ev(i)+1)<< endl;
    }
}


double33s double33s::toBits(int ibit){

        int i1=ibit % 2;
        int i2=ibit / 2 % 2;
        int i3=ibit / 4 % 2;
        int i4=ibit / 8 % 2;
        int i5=ibit / 16 % 2;
        int i6=ibit / 32 % 2;
        cout<<ibit<<" "<<i6<<i5<<i4<<i3<<i2<<i1<<endl;
        return double33s(i6,i5,i4,i3,i2,i1);


}

int double33s::test_functionality() {
    double33s A1(1 , 2 , 3 , 4, 5, 6);
    double33s A1x10(10 , 20 , 30 , 40, 50, 60);
    double33s A2(1 , 20 , 300 , 4000, 50000, 600000);
    double3 v(1,2,3);
    
    cerr << "Testing double33s" << endl;
    cerr << "Test matrix A1: " << A1 << endl;
    cerr << "Testing operators" << endl;
    double33s tmp = A1-A1;
    cerr << "A1 - A1: " << tmp << endl;
    if((A1-A1) != double33s(0)) {
	cerr << "Error in operators '-' or '!=' " << endl;
	assert(0);
    }
    cerr << "A1 * 10: " << A1 * 10 << endl;
    if((A1 * 10) == A1x10) {}
    else {
	cerr << "Error in operators 'm*s' or 'm==m' " << endl;
	assert(0);
    }
    cerr << "10 * A1: " << 10 * A1 << endl;
    assert(10 * A1 == A1x10 );
    double3 v1 = A1 * v;
    double3 v2 = A1.get_as_double33() * v;
    
    assert((v2-v1).norm2() < 1e-16);
    assert(A1 / 0.1 == A1*10.0);
    assert(A1 + A1 == 2 * A1);
    assert((A1 + 1.0) == ((A1 + 2.0) - 1.0));
    assert((1.0 + A1) == (A1 + 1.0));
    assert((A1 - A1) == 0.0);
    assert(0.0 == (A1 - A1));
    double33s A1c(A1);
    assert(0.0 == (A1 - A1c));
    double33s A1c2;
    A1c2(1,1) =  A1(1,1);
    A1c2(1,2) =  A1(2,1);
    A1c2(1,3) =  A1(3,1);
    A1c2(2,2) =  A1(2,2);
    A1c2(2,3) =  A1(3,2);
    A1c2(3,3) =  A1(3,3);
    assert(0.0 == (A1 - A1c2));
    // cppcheck-suppress assignmentInAssert
    assert(((A1 += A1c) -= A1c2) == A1c);
    // cppcheck-suppress assignmentInAssert
        assert( ((A1 *= 4) /= 4) == A1c);
    A1c.Set(double3(1 , 2, 3), double3(4 ,5,6));
    assert( A1c == A1); 
    double a11, a22,  a33, a12,  a23,  a13;
    A1c.Get( a11, a22, a33, a12,a23,a13);
    A1c2.Set( a11, a22, a33, a12,a23,a13);
    assert(A1c == A1c2);
    assert(a11 ==  A1(1,1));
    assert(a12 ==  A1(2,1));
    assert(a13 ==  A1(3,1));
    assert(    a22 ==  A1(2,2));
    assert(a23 ==  A1(3,2));
    assert(a33 ==  A1(3,3));
    assert(A1 == A1.T());

    test_on_matrix(A1);
    test_on_matrix(A2);
    test_on_matrix(A2*1e10);
    test_on_matrix(double33s(2,2,1,2,1,1));
    test_on_matrix(double33s(0,0,0,1,2,3)*1e6);
    test_on_matrix(double33s(0,0,0,1,0,3)*1e3);

    double33s m=double33s(
                9.8012778993872855e-78, 1.1046213408279876e-71, 4.5682402749471458e-73  ,
                1.7582934909952102e-78, -8.2068708307571835e-73 ,-6.3783443749147278e-78
                );
    double33s z;






    for(int i1=-156;i1<=4;i1+=5){
        z=m/pow(10,i1);
        double33s dd(z);


        for(int ibit=0;ibit<64;ibit++){
           double33s dd1=elementwise_Mul(toBits(ibit),dd);
           test_on_matrix(dd1);
        }


    }

    test_on_matrix(double33s(
                       9.8012778993872855e-78, 1.1046213408279876e-71, 4.5682402749471458e-73  ,
                       1.7582934909952102e-78, -8.2068708307571835e-73 ,-6.3783443749147278e-78
                       ));

    return 0;
    for(int i1=-4;i1<=4;i1++)
        for(int i2=-4;i2<=4;i2++)
            for(int i3=-4;i3<=4;i3++)
                for(int i4=-4;i4<=4;i4++)
                    for(int i5=-4;i5<=4;i5++)
                        for(int i6=-4;i6<=4;i6++){
                            test_on_matrix(double33s(i1,i2,i3,i4,i5,i6));
                        }


    for(int i1=-4;i1<=4;i1++)
        for(int i2=-4;i2<=4;i2++)
            for(int i3=-4;i3<=4;i3++)
                for(int i4=-4;i4<=4;i4++)
                    for(int i5=-4;i5<=4;i5++)
                        for(int i6=-4;i6<=4;i6++){
                            test_on_matrix(double33s(rand(),rand(),rand(),rand(),rand(),rand()));
                        }

    test_on_matrix(double33s(2,2,1,2,1,1));


    cerr << "Check that numbers below are almost zero" << endl;
    double33 ATr=A1.calc_Transform1(A1);
    cerr << ATr - A1.T() *A1*A1 << endl;
    double33s ATr2;
    A2.calc_Transform1(A2, ATr2);
    cerr << ATr2 - A2.T() *A2*A2 << endl;

    cerr << "Check that numbers below are almost zero" << endl;
    double33 ATrb=A1.calc_Transform2(A1);
    cerr << ATrb - A1*A1*A1.T() << endl;
    double33s ATr2b;
    A2.calc_Transform2(A2, ATr2b);
    cerr << ATr2b - A2*A2*A2.T() << endl;
    return 0;
}

const double33s zerodouble33s(0);
const double33s Idouble33s(1,1,1,0,0,0);

// Methods for class float33s





//! Return the maximum absolute value of the relative errors of each component.
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double33s& a, const double33s& b, const double tol)
{
    double rel = MaxRelAbsError(a.x11, b.x11, tol);
    rel = Max(rel, MaxRelAbsError(a.x22, b.x22, tol));
    rel = Max(rel, MaxRelAbsError(a.x33, b.x33, tol));
    rel = Max(rel, MaxRelAbsError(a.x12, b.x12, tol));
    rel = Max(rel, MaxRelAbsError(a.x23, b.x23, tol));
    rel = Max(rel, MaxRelAbsError(a.x13, b.x13, tol));

    return rel;
}




