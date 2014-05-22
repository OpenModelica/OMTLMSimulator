#include <cstdlib>
#include <cstdio>
 
//#include <iostream>
#include <iostream>

#include "lightmat.h"
#include "double33s.h"
#include "Bstring.h"
#include "mathdefs.h"

double test_svd();
double test_polar();

int main(int argc, char *argv[])
{
    double3 phi(0.0);
    int phiSeq = 0;
    double33 AA(0.0);
    double33 B(0.0);

    phi.Set(pi*1.37, -pi/2*3.46, 11234*pi/2);
    cout << " phi=" << phi << endl;
    cout << " phiSeq=" << phiSeq << endl;

    AA = A(phi, phiSeq);

    double e1,e2,e3,e4;
    phiToEuler(phi, phiSeq, e1,e2,e3,e4);
    cout << " e1=" << e1 
         << " e2=" << e2 
         << " e3=" << e3 
         << " e4=" << e4 << endl;

    B = Aeuler(e1,e2,e3,e4);

    cout << " A=\n" << AA << endl;
    cout << " B=\n" << B << endl;
    cout << " A-B=\n" << (AA-B) << endl;
    cout << " I=\n" << (AA.T()*B) << endl;

    test_svd();
    test_polar();
    return 0 ;
}

const unsigned nTestMat = 12;

void getTestMat(double33 TestMatrix[nTestMat])
{
TestMatrix[0] = double33(1 , 2 , 3 , 4, 5, 6, 7,8,9);
TestMatrix[1] = TestMatrix[0] * 10;
TestMatrix[2] = double33(1 , 20 , 300 ,
            4000, 50000, 600000,
            700, 800, 900);
double33s Asym(1 , 2 , 3 , 4, 5 ,6);
TestMatrix[3] = Asym;

double33 Az(1e-3, 1e-1,0,
              -1e-1, 1e-3, 0,
              0, 0,0);

TestMatrix[4] = Az;
TestMatrix[5] = -Az;

double33 Ax(0, 0,0,
            0, 1e-3, 1e-1,
            0,  -1e-1, 1e-3);
TestMatrix[6] = Ax;
TestMatrix[7] = -Ax;

double33 Ay(1e-4, 0, 1e-1,
            0,0,0,
            -1e-1, 0, 1e-4);

TestMatrix[8] = Ay;
TestMatrix[9] = -Ay;
TestMatrix[10].Set(8.08687651597165892e-15, 3.16875401168427039e-14, -4.38457681230213645e-15,
        -3.32306092053699197e-14, -9.81316285839151397e-15, -2.14683469253273860e-14,
        -4.15155184821710478e-15, 1.72995773376149136e-14, -2.87010312440289044e-16 );
TestMatrix[11] = TestMatrix[10] * 1e14;
}

double ResidualMax(double33& r){
    double ret = 0;
    for(int i = 1; i <= 3; i++)
        for(int j = 1; j <= 3; j++)
            if(ret < fabs(r(i,j))) ret = fabs(r(i,j));
}

// Test svd
double test_svd() {
    double totalErr = 0;
    double33 TestMatrix[nTestMat];
    double33 Identity(1,0,0,
                      0,1,0,
                      0,0, 1);
    getTestMat(TestMatrix);

    double3 U[3], V[3], S;
    cout << endl << "===================== Testing SVD =========================" << endl;
    for(int m = 0; m < nTestMat; m++) {
        cout << "Test matrix:" << TestMatrix[m] << endl;
        TestMatrix[m].calc_svd(U,S,V);
        double33 VtV, UtU, Residual(TestMatrix[m]);
        for(int i = 0; i < 3; i++) {
            cout << "SV[" << i << "]=" << S(i+1) <<
                    " U=" << U[i] << " V=" << V[i] << endl;
            for(int j = 0; j < 3; j++) {
                VtV(i+1,j+1) = V[i] * V[j];
                UtU(i+1,j+1) = U[i] * U[j];
            }
            Residual -= OuterProduct(U[i],V[i]) * S(i+1);
        }

        cout << "VtV (should be near identity):" << VtV << endl;
        VtV -= Identity;
        cout << "UtU (should be near identity):" << UtU << endl;
        UtU -= Identity;
        double res = ResidualMax(VtV) +
                ResidualMax(UtU) +
                ResidualMax(Residual);
        totalErr += res;
        cout << "CHECK: " << res << "  residual should be near 0 " << endl;
        cout << "=============== " << endl;
    }
    cout << "CHECKSUM: " << totalErr << "  total should be near 0 " << endl;
    return totalErr;
}

// Test polar decomposition
double test_polar() {
    double totalErr = 0;
    double33 TestMatrix[nTestMat];
    double33 Identity(1,0,0,
                      0,1,0,
                      0,0, 1);
    getTestMat(TestMatrix);

    double33 Pl, Pr, U;
    cout << endl << "===================== Testing polar decomposition =========================" << endl;
    for(int m = 0; m < nTestMat; m++) {
        cout << "Test matrix:" << TestMatrix[m] << endl;
        TestMatrix[m].calc_left_polar(Pl, U);
        TestMatrix[m].calc_right_polar(U, Pr);
        cout << "P(left) =" <<  endl << Pl << endl
             << "P(right) =" <<  endl << Pr << endl
                   << "U =" <<  endl << U << endl;
        double33 PPlmTT, UtU, lResidual(TestMatrix[m]);
        PPlmTT = Pl * Pl - TestMatrix[m] * TestMatrix[m].T();
        cout << "Pl Pl - A Transpose(A):" << PPlmTT << endl;

        double33 PPrmTT, rResidual(TestMatrix[m]);
        PPrmTT = Pr * Pr - TestMatrix[m].T() * TestMatrix[m];
        cout << "Pr Pr - Transpose(A) A:" << PPrmTT << endl;

        UtU = U.T() * U;
        cout << "UtU (should be near identity):" << UtU << endl;
        UtU -= Identity;
        lResidual = TestMatrix[m] - Pl * U;
        rResidual = TestMatrix[m] - U * Pr;
        double res =
                ResidualMax(PPlmTT) +
                ResidualMax(PPrmTT) +
                ResidualMax(UtU) +
                ResidualMax(rResidual) +
                ResidualMax(lResidual);
        totalErr += res;
        cout << "CHECK: " << res << "  residual should be near 0 " << endl;
        cout << "=============== " << endl;
    }
    cout << "CHECKSUM: " << totalErr << "  total should be near 0 " << endl;
    return totalErr;
}

