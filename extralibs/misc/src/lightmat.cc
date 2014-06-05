#include <cmath>
#include <cassert>
#include "lightmat.h"
#include "double33s.h"
#include "mathdefs.h"
#include "ErrorLog.h"
#include "tostr.h"
#include <ostream>
#include <cfloat>


//////////////////////////////////////////////////////////////
//
// .SS Constants
//
// These are the defined constants
//

const double3 zerodouble3 = double3(0.0);

const double33 zerodouble33 = double33(0.0);

const double33 Idouble33 = double33(1.0, 0.0, 0.0,
				    0.0, 1.0, 0.0,
				    0.0, 0.0, 1.0);
// A identity matrix 




// Non-inlined functions.
double33::double33(const double a11, const double a12, const double a13,
		   const double a21, const double a22, const double a23,
		   const double a31, const double a32, const double a33)
{
    x[0] = a11; x[1] = a12; x[2] = a13;
    x[3] = a21; x[4] = a22; x[5] = a23;
    x[6] = a31; x[7] = a32; x[8] = a33;
}

double33::double33(const double33& val)
{
    x[0] = val.x[0]; x[1] = val.x[1]; x[2] = val.x[2];
    x[3] = val.x[3]; x[4] = val.x[4]; x[5] = val.x[5];
    x[6] = val.x[6]; x[7] = val.x[7]; x[8] = val.x[8];
}

double33& double33::operator=(const double33& m) 
{
    if (this != &m) {
	x[0] = m.x[0]; x[1] = m.x[1]; x[2] = m.x[2];
	x[3] = m.x[3]; x[4] = m.x[4]; x[5] = m.x[5];
	x[6] = m.x[6]; x[7] = m.x[7]; x[8] = m.x[8];
    }
    return *this;
}

#if 0
// Deactivated since not used and not most efficient. DF 2006-03-09
double33& double33::operator=(const doubleN3& m)
{
    // Assumes column orienting. But since the x data is
    // on row storage format we need to convert.
    assert(m.length()==3);
    x[0] = m(1,1);
    x[1] = m(1,2);
    x[2] = m(1,3);
    x[3] = m(2,1);
    x[4] = m(2,2);
    x[5] = m(2,3);
    x[6] = m(3,1);
    x[7] = m(3,2);
    x[8] = m(3,3);

    return *this;
}
#endif


double33& double33::operator*=(const double33& b)
{
    double a11 = x[0]*b.x[0];
    a11 += x[1]*b.x[3];
    a11 += x[2]*b.x[6];

    double a12 = x[0]*b.x[1];
    a12 += x[1]*b.x[4];
    a12 += x[2]*b.x[7];

    double a13 = x[0]*b.x[2];
    a13 += x[1]*b.x[5];
    a13 += x[2]*b.x[8];

    double a21 = x[3]*b.x[0];
    a21 += x[4]*b.x[3];
    a21 += x[5]*b.x[6];

    double a22 = x[3]*b.x[1];
    a22 += x[4]*b.x[4];
    a22 += x[5]*b.x[7];

    double a23 = x[3]*b.x[2];
    a23 += x[4]*b.x[5];
    a23 += x[5]*b.x[8];

    double a31 = x[6]*b.x[0];
    a31 += x[7]*b.x[3];
    a31 += x[8]*b.x[6];

    double a32 = x[6]*b.x[1];
    a32 += x[7]*b.x[4];
    a32 += x[8]*b.x[7];

    double a33 = x[6]*b.x[2];
    a33 += x[7]*b.x[5];
    a33 += x[8]*b.x[8];

    x[0] = a11; x[1] = a12;  x[2] = a13;
    x[3] = a21; x[4] = a22;  x[5] = a23;
    x[6] = a31; x[7] = a32;  x[8] = a33;

    return *this;
}	

void double33::Set(const double a11, const double a12, const double a13,
		   const double a21, const double a22, const double a23,
		   const double a31, const double a32, const double a33)
{
    x[0] = a11; x[1] = a12; x[2] = a13;
    x[3] = a21; x[4] = a22; x[5] = a23;
    x[6] = a31; x[7] = a32; x[8] = a33;
}


void double33::Get(double& a11,  double& a12,  double& a13,
		   double& a21,  double& a22,  double& a23,
		   double& a31,  double& a32,  double& a33) const
{
    a11 = x[0]; a12 = x[1]; a13 = x[2];
    a21 = x[3]; a22 = x[4]; a23 = x[5];
    a31 = x[6]; a32 = x[7]; a33 = x[8];
}


// Compute SVD of the 3x3 matrix: A = U S Transpose(V), where U & V are unary (rotation) matrices
// and S is diagonal (singular values)
void double33::calc_svd(double3 U[3], double3& S, double3 V[3])
{
    double ata11 = x[0]*x[0] + x[3]*x[3] + x[6]*x[6];
    double ata12 = x[0]*x[1] + x[3]*x[4] + x[6]*x[7];
    double ata13 = x[0]*x[2] + x[3]*x[5] + x[6]*x[8];

    double ata22 = x[1]*x[1] + x[4]*x[4] + x[7]*x[7];
    double ata23 = x[1]*x[2] + x[4]*x[5] + x[7]*x[8];

    double ata33 = x[2]*x[2] + x[5]*x[5] + x[8]*x[8];

    double33s AtA(ata11,ata22,ata33,ata12,ata23,ata13);

    double3 evAtA;
    AtA.calc_eigensystem(evAtA, V);

    for(int i = 1; i <= 3; i++) {
        double ev = evAtA(i);
        if(ev < 0)
            ev = 0; // this is pure numerics. Negative is not possible
        else
            ev = sqrt(ev);
        S(i) = ev;
        if(ev > 1e-30 || ev < -1e-30) {
            U[i-1] = (*this) * V[i-1];
            U[i-1] /= ev;
        }
        else {
            U[i-1] = V[i-1];
        }
    }
}

// Compute rotation matrix U from polar decomposition of the 3x3 matrix:
// A = P U = U P'
void double33::calc_polar_rotation(double33& U)
{
    double3 W[3], V[3];
    double3 S;
    calc_svd(W,S,V);
    U = OuterProduct(W[0],V[0]);
    U += OuterProduct(W[1],V[1]);
    U += OuterProduct(W[2],V[2]);
}

// Compute left polar decomposition of the 3x3 matrix: A = P U,
// P i positive semidefinite and U is unary (rotation) matrix
void double33::calc_left_polar(double33& P, double33& U)
{
    double3 W[3], V[3];
    double3 S;
    calc_svd(W,S,V);
    U = OuterProduct(W[0],V[0]);
    U += OuterProduct(W[1],V[1]);
    U += OuterProduct(W[2],V[2]);

    P = OuterProduct(W[0],W[0]) * S(1);
    P += OuterProduct(W[1],W[1]) * S(2);
    P += OuterProduct(W[2],W[2]) * S(3);
}

// Compute right polar decomposition of the 3x3 matrix: A = U P,
// P i positive semidefinite and U is unary (rotation) matrix
void double33::calc_right_polar(double33& U, double33& P)
{
    double3 W[3], V[3];
    double3 S;
    calc_svd(W,S,V);
    U = OuterProduct(W[0],V[0]);
    U += OuterProduct(W[1],V[1]);
    U += OuterProduct(W[2],V[2]);

    P = OuterProduct(V[0],V[0]) * S(1);
    P += OuterProduct(V[1],V[1]) * S(2);
    P += OuterProduct(V[2],V[2]) * S(3);
}

// M * V
const double3 operator*(const double33& a, const double3& b)
{
    double XX = a.x[0]*b.XX;
    XX += a.x[1]*b.YY;
    XX += a.x[2]*b.ZZ;
		       
    double YY = a.x[3]*b.XX;
    YY += a.x[4]*b.YY;
    YY += a.x[5]*b.ZZ;

    double ZZ = a.x[6]*b.XX;
    ZZ += a.x[7]*b.YY;
    ZZ += a.x[8]*b.ZZ;

    return double3(XX, YY, ZZ);

#if 0
    // This old implementation has problem with inlining for SUN C++.
    return double3(a.x[0]*b.XX + a.x[1]*b.YY + a.x[2]*b.ZZ,
		   a.x[3]*b.XX + a.x[4]*b.YY + a.x[5]*b.ZZ,
		   a.x[6]*b.XX + a.x[7]*b.YY + a.x[8]*b.ZZ);
#endif
}	

// V * M
const double3 operator*(const double3& a, const double33& b)
{
    double XX = a.XX*b.x[0];
    XX += a.YY*b.x[3];
    XX += a.ZZ*b.x[6];

    double YY = a.XX*b.x[1];
    YY += a.YY*b.x[4];
    YY += a.ZZ*b.x[7];
    
    double ZZ = a.XX*b.x[2];
    ZZ += a.YY*b.x[5];
    ZZ += a.ZZ*b.x[8];

    return double3(XX, YY, ZZ);

#if 0
    // This old implementation has problem with inlining for SUN C++.
    return double3(a.XX*b.x[0] + a.YY*b.x[3] + a.ZZ*b.x[6],
		   a.XX*b.x[1] + a.YY*b.x[4] + a.ZZ*b.x[7],
		   a.XX*b.x[2] + a.YY*b.x[5] + a.ZZ*b.x[8]);
#endif
}	




// Create a transformation matrix from base system to local system.
// We use 1,2,3 rotation scheme.
const double33 A123(const double3& phi) 
{ 
    double tmpc1 = cos(phi.XX);
    double tmps1 = sin(phi.XX);
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    return double33(tmpc2 * tmpc3, 
		    tmpc1 * tmps3 + tmps1 * tmps2 * tmpc3, 
		    tmps1 * tmps3 - tmpc1 * tmps2 * tmpc3, 
		    -tmpc2 * tmps3, 
		    tmpc1 * tmpc3 - tmps1 * tmps2 * tmps3, 
		    tmps1 * tmpc3 + tmpc1 * tmps2 * tmps3, 
		    tmps2, 
		    -tmps1 * tmpc2, 
		    tmpc1 * tmpc2);
}


// Create an inverse transformation matrix from base system to local system.
// We use 1,2,3 rotation scheme.
const double33 invA123(const double3& phi) 
{ 
    double tmpc1 = cos(phi.XX);
    double tmps1 = sin(phi.XX);
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    return double33(tmpc2 * tmpc3, 
		    -tmpc2 * tmps3, 
		    tmps2, 
		    tmpc1 * tmps3 + tmps1 * tmps2 * tmpc3, 
		    tmpc1 * tmpc3 - tmps1 * tmps2 * tmps3, 
		    -tmps1 * tmpc2, 
		    tmps1 * tmps3 - tmpc1 * tmps2 * tmpc3, 
		    tmps1 * tmpc3 + tmpc1 * tmps2 * tmps3, 
		    tmpc1 * tmpc2);
}


// Create a transformation matrix from base system to local system.
// We use 3,2,1 rotation scheme.
const double33 A321(const double3& phi) 
{ 
    double tmpc1 = cos(phi.XX);
    double tmps1 = sin(phi.XX);
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    return double33(tmpc2 * tmpc3, 
		    tmpc2 * tmps3, 
		    -tmps2, 
		    -tmpc1 * tmps3 + tmps1 * tmps2 * tmpc3, 
		    tmpc1 * tmpc3 + tmps1 * tmps2 * tmps3, 
		    tmps1 * tmpc2, 
		    tmps1 * tmps3 + tmpc1 * tmps2 * tmpc3, 
		    -tmps1 * tmpc3 + tmpc1 * tmps2 * tmps3, 
		    tmpc1 * tmpc2);
}


// Create an inverse transformation matrix from base system to local system.
// We use 3,2,1 rotation scheme.
const double33 invA321(const double3& phi) 
{ 
    double tmpc1 = cos(phi.XX);
    double tmps1 = sin(phi.XX);
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    return double33(tmpc2 * tmpc3,             
		    -tmpc1 * tmps3 + tmps1 * tmps2 * tmpc3, 
		    tmps1 * tmps3 + tmpc1 * tmps2 * tmpc3, 
		    tmpc2 * tmps3,  
		    tmpc1 * tmpc3 + tmps1 * tmps2 * tmps3, 
		    -tmps1 * tmpc3 + tmpc1 * tmps2 * tmps3, 
		    -tmps2, 
		    tmps1 * tmpc2, 
		    tmpc1 * tmpc2);
}

// Create a transformation matrix from base system to local system.
const double33 A(const double3& phi, const int phiSequence) 
{
    switch (phiSequence) {
    case 0: // We have 321 rotation.
	return A321(phi);
	break;
    case 1: // We have 123 rotation.
	return A123(phi);
	break;
    default:
	assert(False);
	return double33(0.0); // Just dummy.
    }
}

// Create a transformation matrix from base system to local system.
// We use only rotation around the axis number 3 (z-axis).
const double33 A3(const double phi_3) 
{ 
    double tmpc3 = cos(phi_3);
    double tmps3 = sin(phi_3);

    return double33( tmpc3, tmps3, 0.0, 
		     -tmps3, tmpc3, 0.0, 
		     0.0,   0.0, 1.0);
}


// Create a transformation matrix from base system to local system.
// We use only rotation around the axis number 2 (y-axis).
const double33 A2(const double phi_2) 
{ 
    double tmpc2 = cos(phi_2);
    double tmps2 = sin(phi_2);

    return double33( tmpc2, 0.0, -tmps2, 
		     0.0, 1.0,    0.0, 
		     tmps2, 0.0,  tmpc2);
}


// Create a transformation matrix from base system to local system.
// We use only rotation around the axis number 1 (x-axis).
const double33 A1(const double phi_1) 
{ 
    double tmpc1 = cos(phi_1);
    double tmps1 = sin(phi_1);

    return double33(1.0,    0.0,   0.0, 
		    0.0,  tmpc1, tmps1, 
		    0.0, -tmps1, tmpc1);
}

const double33 U123(const double3& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    // Transformation matrix U We use 1,2,3 rotation

    return double33(tmpc2 * tmpc3, tmps3, 0.0,
		    -tmpc2 * tmps3, tmpc3, 0.0,
		    tmps2, 0.0 , 1.0);
}

const double33 U321(const double3& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc1 = cos(phi.XX);
    double tmps1 = sin(phi.XX);
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);

    // Transformation matrix U We use 3,2,1 rotation

    return double33(1.0,    0.0,      -tmps2,
		    0.0,  tmpc1, tmps1*tmpc2,
		    0.0, -tmps1, tmpc1*tmpc2);
}


const double33 H321(const double3& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    // Transformation matrix H We use 3,2,1 rotation


    return double33(tmpc2 * tmpc3, -tmps3, 0.0,
		    tmpc2 * tmps3,  tmpc3, 0.0,
		    -tmps2, 0.0 , 1.0);
}


const double33 H123(const double3& phi) 
{ 
    // Transformation matrix H We use 1,2,3 rotation

    //return (invA123(phi) * U123(phi));
    // Temp. variables for cos and sin etc.
    double c1 = cos(phi.XX);
    double s1 = sin(phi.XX);
    double c2 = cos(phi.YY);
    double s2 = sin(phi.YY);

    return double33(1.0, 0.0, s2, 
		    0.0, c1, -c2*s1,
		    0.0, s1, c1*c2);

}


const double33 invU123(const double3& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    tmpc2 += Sign(tmpc2)*1.0e-50;
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    // Transformation matrix U.  We use 1,2,3 rotation

    return double33(tmpc3/tmpc2, -tmps3/tmpc2, 0.0,
		    tmps3, tmpc3, 0.0,
		    -tmps2 * tmpc3/tmpc2, tmps2 * tmps3/tmpc2, 1.0);
}



const double33 invH321(const double3& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    tmpc2 += Sign(tmpc2)*1.0e-50;
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    // Transformation matrix invH.  We use 3,2,1 rotation

    return double33(tmpc3/tmpc2, tmps3/tmpc2, 0.0,
		    -tmps3, tmpc3, 0.0,
		    tmps2 * tmpc3/tmpc2, tmps2 * tmps3/tmpc2, 1.0);
}

const double33 invH123(const double3& phi) 
{ 
    // Transformation matrix invH We use 1,2,3 rotation

    //return (invU123(phi) * A123(phi));

    // Temp. variables for cos and sin etc.
    double c1 = cos(phi.XX);
    double s1 = sin(phi.XX);
    double c2 = cos(phi.YY);
    c2 += Sign(c2)*1.0e-50;
    double s2 = sin(phi.YY);

    return double33(1.0, s1*s2/c2, -c1*s2/c2, 
		    0.0, c1, s1, 
		    0.0, -s1/c2, c1/c2);

}

const double33 invH(const double3& phi, const int phiSequence)
{
    switch (phiSequence) {
    case 0: // We have 321 rotation.
	return invH321(phi);
	break;
    case 1: // We have 123 rotation.
	return invH123(phi);
	break;
    default:
	assert(False);
	return double33(0.0); // Just dummy.
    }

}


const double33 invU321(const double3& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc1 = cos(phi.XX);
    double tmps1 = sin(phi.XX);
    double tmpc2 = cos(phi.YY);
    tmpc2 += Sign(tmpc2)*1.0e-50;
    double tmps2 = sin(phi.YY);

    // Transformation matrix invU.  We use 3,2,1 rotation

    return double33(1.0, tmps2*tmps1/tmpc2, tmps2*tmpc1/tmpc2,
		    0.0, tmpc1, -tmps1,
		    0.0, tmps1/tmpc2, tmpc1/tmpc2);
}



const double33 vU123(const double3& phi, const double3& vphi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    // Transformation matrix vU.  We use 1,2,3 rotation
    return double33(-tmps2 * tmpc3 * vphi.YY - tmpc2 * tmps3 * vphi.ZZ,
		    tmpc3 * vphi.ZZ,
		    0.0,
		    tmps2 * tmps3 * vphi.YY - tmpc2 * tmpc3 * vphi.ZZ,
		    -tmps3 * vphi.ZZ,
		    0.0,
		    tmpc2 * vphi.YY,
		    0.0,
		    0.0);
}


const double33 vH123(const double3& phi,
		     const double3& vphi) 
{ 
    return (invA123(phi) * vU123(phi,vphi));
}



const double33 vH321(const double3& phi,
		     const double3& vphi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    return double33(-tmpc3*vphi.YY*tmps2 - tmpc2*vphi.ZZ*tmps3, 
		    -tmpc3*vphi.ZZ, 
		    0.0, 
		    tmpc2*tmpc3*vphi.ZZ - vphi.YY*tmps2*tmps3, 
		    -vphi.ZZ*tmps3, 
		    0.0, 
		    -tmpc2*vphi.YY, 
		    0.0,
		    0.0);
}


const double33 vU321(const double3& phi,
		     const double3& vphi) 
{ 
    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc1 = cos(phi.XX);
    double tmps1 = sin(phi.XX);

    return double33(0.0, 
		    0.0,
		    -tmpc2*vphi.YY,
		    0.0,
		    -vphi.XX*tmps1, 
		    tmpc1*tmpc2*vphi.XX - vphi.YY*tmps1*tmps2, 
		    0.0,
		    -tmpc1*vphi.XX,
		    -tmpc2*vphi.XX*tmps1 - tmpc1*vphi.YY*tmps2);
}


const double3 Omega_U123(const double3& phi,
			 const double3& vphi) 
{ 
    // Angular speed Omega_cBase this.  We use 1,2,3 rotation.
    // Omega[cBase][this] = U[cBase] . vphi[cBase]
    // return (U123(phi) * vphi);

    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    // Transformation matrix U We use 1,2,3 rotation

    return double3(tmpc2*tmpc3*vphi.XX + tmps3*vphi.YY,
		   -tmpc2*tmps3*vphi.XX + tmpc3*vphi.YY,
		   tmps2*vphi.XX + vphi.ZZ);

}



const double3 Omega_H123(const double3& phi,
			 const double3& vphi) 
{ 
    // Angular speed Omega_cBase_cBase.  We use 1,2,3 rotation.
    // Omega[cBase][cBase] = H[cBase] . vphi[cBase]
    return (H123(phi) * vphi);
}


const double3 Omega_H321(const double3& phi,
			 const double3& vphi) 
{ 
    // Angular speed Omega_cBase_cBase.  We use 3,2,1 rotation.
    // Omega[cBase][cBase] = H[cBase] . vphi[cBase]

    // Temp. variables for cos and sin etc.
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);
    double tmpc3 = cos(phi.ZZ);
    double tmps3 = sin(phi.ZZ);

    return double3(tmpc2*tmpc3*vphi.XX - tmps3*vphi.YY,
		   tmpc2*tmps3*vphi.XX + tmpc3*vphi.YY,
		   -tmps2*vphi.XX + vphi.ZZ);
}


const double3 Omega_U321(const double3& phi,
			 const double3& vphi) 
{ 
    // Angular speed Omega_cBase this.  We use 3,2,1 rotation.
    // Omega[cBase][this] = U[cBase] . vphi[cBase]
    // return (U321(phi) * vphi);

    // Temp. variables for cos and sin etc.
    double tmpc1 = cos(phi.XX);
    double tmps1 = sin(phi.XX);
    double tmpc2 = cos(phi.YY);
    double tmps2 = sin(phi.YY);

    // Transformation matrix U We use 3,2,1 rotation

    return double3(vphi.XX - tmps2*vphi.ZZ,
		   tmpc1*vphi.YY + tmps1*tmpc2*vphi.ZZ,
		   -tmps1*vphi.YY + tmpc1*tmpc2*vphi.ZZ);
}

//! Transformation from cylindrical coordinates and its 1-st & 2-nd derivative to 
//! acceleration in Cartesian coordinates.
const double3 aRC2aR(    const double3& RC, 
			 const double3& vRC,
			 const double3& aRC){
    double tmpcos = cos(RC.YY);
    double tmpsin = sin(RC.YY);

    double x = tmpcos * RC.XX;
    double y = tmpsin * RC.XX;

    double vx = - vRC.YY * y + tmpcos * vRC.XX;
    double vy = vRC.YY * x + tmpsin * vRC.XX;

    return double3(tmpcos * aRC.XX - ( aRC.YY * y + vRC.YY * ( vy + tmpsin * vRC.XX)),
		   aRC.YY * x + vRC.YY * (vx + tmpcos * vRC.XX) + tmpsin * aRC.YY,
		   aRC.ZZ);
}


const double3 aR2aRC(const double3& aR,
		     const double3& RC, 
		     const double3& vRC) 
{
    double tmpc = cos(RC.YY);
    double tmps = sin(RC.YY);

    double aR_cmp_1 =  tmpc*aR.XX + tmps*aR.YY;
    double aR_cmp_2 = -tmps*aR.XX + tmpc*aR.YY;

    double aRC_1 = aR_cmp_1 + RC.XX * vRC.YY*vRC.YY;
    double aRC_2 = (aR_cmp_2 - 2 * vRC.XX * vRC.YY ) / RC.XX;

    return double3(aRC_1, aRC_2, aR.ZZ);
}


const double3 vOmega_U123(const double3& phi,
			  const double3& vphi,
			  const double3& aphi) 
{ 
    // Angular acc. vOmega_cBase_cBase_this.  We use 1,2,3 rotation.
    // vOmega[cBase][cBase,this] =  vOmega[cBase][this,this] =
    // vU[cBase] . vphi[cBase] + U[cBase] . aphi[cBase];
    
    return ((vU123(phi,vphi) * vphi) + (U123(phi) * aphi));
}




const double3 vOmega_H123(const double3& phi,
			  const double3& vphi,
			  const double3& aphi) 
{ 
    // Angular acc. vOmega_cBase_cBase_cBase.  We use 1,2,3 rotation.
    // vOmega[cBase][cBase,cBase] =  vOmega[cBase][this,cBase] =
    // vH[cBase] . vphi[cBase] + H[cBase] . aphi[cBase];
    
    return ((vH123(phi,vphi) * vphi) + (H123(phi) * aphi));
}


const double3 vOmega_U321(const double3& phi,
			  const double3& vphi,
			  const double3& aphi) 
{ 
    // Angular acc. vOmega_cBase_cBase_this.  We use 3,2,1 rotation.
    // vOmega[cBase][cBase,this] =  vOmega[cBase][this,this] =
    // vU[cBase] . vphi[cBase] + U[cBase] . aphi[cBase];
    
    return ((vU321(phi,vphi) * vphi) + (U321(phi) * aphi));
}




const double3 vOmega_H321(const double3& phi,
			  const double3& vphi,
			  const double3& aphi) 
{ 
    // Angular acc. vOmega_cBase_cBase_cBase.  We use 3,2,1 rotation.
    // vOmega[cBase][cBase,cBase] =  vOmega[cBase][this,cBase] =
    // vH[cBase] . vphi[cBase] + H[cBase] . aphi[cBase];
    
    return ((vH321(phi,vphi) * vphi) + (H321(phi) * aphi));
}


const double3 Min(const double3& a, const double3& b)
{
    return double3(a.XX < b.XX ? a.XX : b.XX,
		   a.YY < b.YY ? a.YY : b.YY,
		   a.ZZ < b.ZZ ? a.ZZ : b.ZZ);
}	

const double3 Max(const double3& a, const double3& b)
{
    return double3(a.XX > b.XX ? a.XX : b.XX,
		   a.YY > b.YY ? a.YY : b.YY,
		   a.ZZ > b.ZZ ? a.ZZ : b.ZZ);
}	
const double3 fabs(const double3&a) //************************************************
{
    return double3(fabs(a.XX),fabs(a.YY),fabs(a.ZZ));
}
ostream& operator<<(ostream& o, const double3& v) 
{
    o << "( ";
    o << v.XX << ' ' << v.YY << ' ' << v.ZZ << " )";
    return o;
}


ostream& operator<<(ostream& o, const double33& M) 
{
    return o << "( " << M.x[0] << ' ' << M.x[1] << ' ' << M.x[2] << "\n"
	     << M.x[3] << ' ' << M.x[4] << ' ' << M.x[5] << "\n"
	     << M.x[6] << ' ' << M.x[7] << ' ' << M.x[8] << " )";
}

Bstring ToStr(const double3& val) 
{
    return Bstring("( " + ToStr(val.XX) + " " + ToStr(val.YY) + " " + ToStr(val.ZZ) + " )");
}

Bstring ToStr(const float3& val) 
{
    return Bstring("( " + ToStr(val.XX) + " " + ToStr(val.YY) + " " + ToStr(val.ZZ) + " )");
}



Bstring ToStr(const double33& val) 
{
    Bstring res;
    res = "( " + ToStr(val.x[0]) + " " + ToStr(val.x[1]) + " " + ToStr(val.x[2]) + "  \n";
    res+= "  " + ToStr(val.x[3]) + " " + ToStr(val.x[4]) + " " + ToStr(val.x[5]) + "  \n";
    res+= "  " + ToStr(val.x[6]) + " " + ToStr(val.x[7]) + " " + ToStr(val.x[8]) + " )";
    return res;
}

const double33 Max(const double33& a,const double33& b)
{
    return double33(a.x[0]>b.x[0]?a.x[0]:b.x[0],
                    a.x[1]>b.x[1]?a.x[1]:b.x[1],
                    a.x[2]>b.x[2]?a.x[2]:b.x[2],
                    a.x[3]>b.x[3]?a.x[3]:b.x[3],
                    a.x[4]>b.x[4]?a.x[4]:b.x[4],
	            a.x[5]>b.x[5]?a.x[5]:b.x[5],
                    a.x[6]>b.x[6]?a.x[6]:b.x[6],
                    a.x[7]>b.x[7]?a.x[7]:b.x[7],
                    a.x[8]>b.x[8]?a.x[8]:b.x[8]);
}

const double33 Min(const double33& a,const double33& b)
{
    return double33(a.x[0]<b.x[0]?a.x[0]:b.x[0],
                    a.x[1]<b.x[1]?a.x[1]:b.x[1],
                    a.x[2]<b.x[2]?a.x[2]:b.x[2],
                    a.x[3]<b.x[3]?a.x[3]:b.x[3],
                    a.x[4]<b.x[4]?a.x[4]:b.x[4],
	            a.x[5]<b.x[5]?a.x[5]:b.x[5],
                    a.x[6]<b.x[6]?a.x[6]:b.x[6],
                    a.x[7]<b.x[7]?a.x[7]:b.x[7],
                    a.x[8]<b.x[8]?a.x[8]:b.x[8]);
}

const double33 fabs(const double33& a)
{
    return double33(fabs(a.x[0]),
                    fabs(a.x[1]),
                    fabs(a.x[2]),
                    fabs(a.x[3]),
                    fabs(a.x[4]),
                    fabs(a.x[5]),
                    fabs(a.x[6]),
                    fabs(a.x[7]),
                    fabs(a.x[8]));
}

int validate_euler(double q1,double q2,double q3,double q4) 
{

    double fel = fabs(sqrt(q1*q1+q2*q2+q3*q3+q4*q4) - 1);

    if (fel>1.0e-6) {
	Error("validate_euler error=" + ToStr(fel));
	return 0 ;
    }
    return 1;
}


int validate_euler2(double q1,double q2,double q3,double q4) 
{

    double fel = fabs(sqrt(q1*q1+q2*q2+q3*q3+q4*q4) - 1);
    if (fel>2.0e-15) {
        Error("validate_euler error=" + ToStr(fel));
	return 0 ;
    }
    return 1;
}


int validate_euler(double* q) {
    return validate_euler(q[0], q[1], q[2], q[3]);
} 


const double33 Aeuler(const double q1,
		      const double q2,
		      const double q3,
		      const double q4)
{ 
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif

    double a11 = q1*q1 - q2*q2 - q3*q3 + q4*q4 ;
    double a12 = 2.0*(q1*q2 + q3*q4) ;
    double a13 = 2.0*(q1*q3 - q2*q4) ;
    double a21 = 2.0*(q1*q2 - q3*q4) ;
    double a22 = q2*q2 - q3*q3 - q1*q1 + q4*q4 ;
    double a23 = 2.0*(q2*q3 + q1*q4) ;
    double a31 = 2.0*(q1*q3 + q2*q4) ;
    double a32 = 2.0*(q2*q3 - q1*q4) ;
    double a33 = q3*q3 - q1*q1 - q2*q2 + q4*q4 ;
            
    return double33(a11, a12, a13,
		    a21, a22, a23,
		    a31, a32, a33) ;
}












const double33 invAeuler(const double q1,
			 const double q2,
			 const double q3,
			 const double q4)
{ 
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif

    double a11 = q1*q1 - q2*q2 - q3*q3 + q4*q4 ;
    double a12 = 2.0*(q1*q2 + q3*q4) ;
    double a13 = 2.0*(q1*q3 - q2*q4) ;
    double a21 = 2.0*(q1*q2 - q3*q4) ;
    double a22 = q2*q2 - q3*q3 - q1*q1 + q4*q4 ;
    double a23 = 2.0*(q2*q3 + q1*q4) ;
    double a31 = 2.0*(q1*q3 + q2*q4) ;
    double a32 = 2.0*(q2*q3 - q1*q4) ;
    double a33 = q3*q3 - q1*q1 - q2*q2 + q4*q4 ;
            
    return double33(a11, a21, a31,
		    a12, a22, a32,
		    a13, a23, a33) ;
}

const double3 EulerTophi321(const double q1,
			    const double q2,
			    const double q3,
			    const double q4)
{
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
    double a11 = q1*q1 - q2*q2 - q3*q3 + q4*q4 ;
    double a12 = 2.0*(q1*q2 + q3*q4) ;
    double a13 = 2.0*(q1*q3 - q2*q4) ;
    double a23 = 2.0*(q2*q3 + q1*q4) ;
    double a33 = q3*q3 - q1*q1 - q2*q2 + q4*q4 ;

#ifdef DEBUGFLG
    // If we should be able to make asin() then the value should be in [-1, 1]
    assert(a13>-1.0-1.0e-15);
    assert(a13< 1.0+1.0e-15);
#endif
    double phi2 = (a13 == 0.0)? 0.0 : asin((a13<-1.0) ? 1.0 : ((a13>1.0) ? -1.0 : -a13 )); // Safe
    //double phi2 = asin(-a13); 
    
    double tmp = cos(phi2) ;
    double cosphi2 = tmp+Sign(tmp)*1.0e-50 ;
  
    // This implementation limits ph1 and phi3 to +/-pi
    // so we use it.
    double phi1 = atan2(a23/cosphi2, a33/cosphi2);
    double phi3 = atan2(a12/cosphi2, a11/cosphi2);
    
    return double3(phi1, phi2, phi3) ;
}


const double3 ATophi321(const double33& A)
{
    double a11 = A.x[0]; // A(1,1);
    double a12 = A.x[1]; // A(1,2);
    double a13 = A.x[2]; // A(1,3);
    double a23 = A.x[5]; // A(2,3);
    double a33 = A.x[8]; // A(3,3);

#ifdef DEBUGFLG
    // If we should be able to make asin() then the value should be in [-1, 1]
    assert(a13>-1.0-1.0e-15);
    assert(a13< 1.0+1.0e-15);
#endif
    //double phi2 =  asin(-a13) ;
    double phi2 = (fabs(a13) < DBL_MIN)? 0.0 : asin((a13<-1.0) ? 1.0 : ((a13>1.0) ? -1.0 : -a13 )); // Safe

    double tmp = cos(phi2) ;
    double cosphi2 = tmp+Sign(tmp)*1.0e-50 ;
  
    // This implementation limits ph1 and phi3 to +/-pi
    // so we use it.
    double phi1 = atan2(a23/cosphi2, a33/cosphi2);
    double phi3 = atan2(a12/cosphi2, a11/cosphi2);

    if(fabs(phi1) < DBL_MIN) phi1 = 0.0;
    if(fabs(phi3) < DBL_MIN) phi3 = 0.0;

    return double3(phi1, phi2, phi3) ;
}

const double3 ATophi123(const double33& A)
{
    double a11 = A.x[0]; // A(1,1);
    double a21 = A.x[3]; // A(2,1);
    double a31 = A.x[6]; // A(3,1);
    double a32 = A.x[7]; // A(3,2);
    double a33 = A.x[8]; // A(3,3);

#ifdef DEBUGFLG
    // If we should be able to make asin() then the value should be in [-1, 1]
    assert(a31>-1.0-1.0e-15);
    assert(a31< 1.0+1.0e-15);
#endif
    double phi2 = (fabs(a31)<DBL_MIN)? 0.0 : asin((a31<-1.0) ? -1.0 : ((a31>1.0) ? 1.0 : a31 )); // Safe
    //double phi2 =  asin(a31) ;
    
    double tmp = cos(phi2) ;
    double cosphi2 = tmp+Sign(tmp)*1.0e-50 ;
  
    // This implementation limits ph1 and phi3 to +/-pi
    // so we use it.
    double phi1 = atan2(-a32/cosphi2, a33/cosphi2);
    double phi3 = atan2(-a21/cosphi2, a11/cosphi2);

    if(fabs(phi1) < DBL_MIN) phi1 = 0.0;
    if(fabs(phi3) < DBL_MIN) phi3 = 0.0;

    return double3(phi1, phi2, phi3) ;
}

const double3 ATophi(const double33& A, const int phiSequence)
{
    switch (phiSequence) {
    case 0: // We have 321 rotation.
	return ATophi321(A);
	break;
    case 1: // We have 123 rotation.
	return ATophi123(A);
	break;
    default:
	assert(False);
	return double3(0.0); // Just dummy.
    }
}



void phi321ToEuler(const double3& phi,
		   double& q1,
		   double& q2,
		   double& q3,
		   double& q4)
{
    
    double c1 = cos(phi.XX/2.0);
    double s1 = sin(phi.XX/2.0);
    double c2 = cos(phi.YY/2.0);
    double s2 = sin(phi.YY/2.0);
    double c3 = cos(phi.ZZ/2.0);
    double s3 = sin(phi.ZZ/2.0);
    
    q1 =  s1*c2*c3 - c1*s2*s3 ;
    q2 =  c1*s2*c3 + s1*c2*s3 ;
    q3 = -s1*s2*c3 + c1*c2*s3 ;
    q4 =  c1*c2*c3 + s1*s2*s3 ;
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
}


void phi123ToEuler(const double3& phi,
		   double& q1,
		   double& q2,
		   double& q3,
		   double& q4)
{
    
    double c1 = cos(phi.XX/2.0);
    double s1 = sin(phi.XX/2.0);
    double c2 = cos(phi.YY/2.0);
    double s2 = sin(phi.YY/2.0);
    double c3 = cos(phi.ZZ/2.0);
    double s3 = sin(phi.ZZ/2.0);
    
    q1 =  s1*c2*c3 + c1*s2*s3 ;
    q2 =  c1*s2*c3 - s1*c2*s3 ;
    q3 =  s1*s2*c3 + c1*c2*s3 ;
    q4 =  c1*c2*c3 - s1*s2*s3 ;
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
}

void phi123ToEulerDer(const double3& phi,
                      double3& dq1_dphi,
                      double3& dq2_dphi,
                      double3& dq3_dphi,
                      double3& dq4_dphi)
{

    const double c1 = cos(phi.XX/2.0);
    const double s1 = sin(phi.XX/2.0);
    const double c2 = cos(phi.YY/2.0);
    const double s2 = sin(phi.YY/2.0);
    const double c3 = cos(phi.ZZ/2.0);
    const double s3 = sin(phi.ZZ/2.0);

    // q1 =  s1*c2*c3 + c1*s2*s3 ;
    dq1_dphi.XX = (c1*c2*c3 - s1*s2*s3)/2 ;
    dq1_dphi.YY = (-s1*s2*c3 + c1*c2*s3)/2 ;
    dq1_dphi.ZZ = (-s1*c2*s3 + c1*s2*c3)/2 ;

    // q2.XX =  c1*s2*c3 - s1*c2*s3 ;
    dq2_dphi.XX =  (-s1*s2*c3 - c1*c2*s3)/2 ;
    dq2_dphi.YY =  (c1*c2*c3 + s1*s2*s3)/2 ;
    dq2_dphi.ZZ =  (-c1*s2*s3 - s1*c2*c3)/2 ;

    // q3 =  s1*s2*c3 + c1*c2*s3 ;
    dq3_dphi.XX =  (c1*s2*c3 - s1*c2*s3)/2 ;
    dq3_dphi.YY =  (s1*c2*c3 - c1*s2*s3)/2 ;
    dq3_dphi.ZZ =  (-s1*s2*s3 + c1*c2*c3)/2 ;


    // q4 =  c1*c2*c3 - s1*s2*s3 ;
    dq4_dphi.XX =  (-s1*c2*c3 - c1*s2*s3)/2 ;
    dq4_dphi.YY =  (-c1*s2*c3 - s1*c2*s3)/2 ;
    dq4_dphi.ZZ =  (-c1*c2*s3 - s1*s2*c3)/2 ;
}


void phiToEuler(const double3& phi,
                const int phiSequence,
                double& q1,
                double& q2,
                double& q3,
                double& q4)
{
    switch (phiSequence) {
    case 0: // We have 321 rotation.
	phi321ToEuler(phi, q1, q2, q3,q4);
	break;
    case 1: // We have 123 rotation.
	phi123ToEuler(phi, q1, q2, q3,q4);
	break;
    default:
	assert(False);
    }
}


void AToEuler(const double33& A,
	      double& q1,
	      double& q2,
	      double& q3,
	      double& q4)
{
    //
    // THEORY: see documentation in header file (lightmat.h). 
    //

    // --------------------------------------------------------
    // OLD IMPLEMENTATION

    // This is the old implementation. It transforms
    // first to angles and then to Euler parameters.
    // This is not a good solution, since it involves
    // singularities (angles does not cover all of SO(3)).

    // -- removed by Klas 2006-0421 --
    //phi321ToEuler(ATophi321(A), q1, q2, q3, q4);
    //return;
    // -------------------------------


    // -------------------------------------------------------
    // NEW IMPLEMENTATION

    // The new implementation always works, i.e. it 
    // contains no singularities. However, the Euler
    // parameters q are not uniqly determined by the
    // rotation matrix A. Indeed, -q and q represents
    // the same rotation (see theory in header file).

    const double a11 = A.x[0];
    const double a12 = A.x[1];
    const double a13 = A.x[2];
    const double a21 = A.x[3];
    const double a22 = A.x[4];
    const double a23 = A.x[5];
    const double a31 = A.x[6];
    const double a32 = A.x[7];
    const double a33 = A.x[8];

    q1 = 0.5*sqrt(Max(0.0,1+a11-a22-a33));
    q2 = 0.5*sqrt(Max(0.0,1-a11+a22-a33));
    q3 = 0.5*sqrt(Max(0.0,1-a11-a22+a33));
    q4 = 0.5*sqrt(Max(0.0,1+a11+a22+a33));
    
    if (q1>=q2 && q1>=q3 && q1>=q4) {
	// Assume q1 is positive
	if (a12+a21<0) q2 = -q2;
	if (a13+a31<0) q3 = -q3;
	if (a23-a32<0) q4 = -q4;
    }
    else if (q2>=q1 && q2>=q3 && q2>=q4) {
	// Assume q2 is positive
	if (a12+a21<0) q1 = -q1;
	if (a23+a32<0) q3 = -q3;
	if (a31-a13<0) q4 = -q4;
    }
    else if (q3>=q1 && q3>=q2 && q3>=q4) {
	// Assume q3 is positive
	if (a13+a31<0) q1 = -q1;
	if (a23+a32<0) q2 = -q2;
	if (a12-a21<0) q4 = -q4;
    }
    else {
	// Assume q4 is positive
	if (a23-a32<0) q1 = -q1;
	if (a31-a13<0) q2 = -q2;
	if (a12-a21<0) q3 = -q3;
    }

    // We choose the solution such that 
    // q1 is always positive
    if (q1 < 0) {
	q1 = -q1;
	q2 = -q2;
	q3 = -q3;
	q4 = -q4;
    }

#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4));
    // Iakov, do not change this. If there is a problem then we should fix the
    // the underlying problem. DF 2002-03-28.
#endif
}



// Calculates the time derivate of the Euler parameters q1 ... q4.
void qdot(const double q1,
	  const double q2,
	  const double q3,
	  const double q4,
	  const double3& cg_Omega_cB_cB,
	  double& q1dot,
	  double& q2dot,
	  double& q3dot,
	  double& q4dot) {
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
    // validate_euler checks so the square sum isn't to far from 1.0
 

    double3 cg_Omega_cB_cg_tmp = Aeuler(q1, q2, q3, q4) * cg_Omega_cB_cB;
  
    double Omega1, Omega2, Omega3 ;
    cg_Omega_cB_cg_tmp.Get(Omega1, Omega2, Omega3);

    q1dot =( q4*Omega1 - q3*Omega2 + q2*Omega3 )/2.0 ;
    q2dot =( q3*Omega1 + q4*Omega2 - q1*Omega3 )/2.0 ;
    q3dot =( -q2*Omega1 + q1*Omega2 + q4*Omega3)/2.0 ;
    q4dot =( -q1*Omega1 - q2*Omega2 - q3*Omega3)/2.0 ;
}



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
	  double& q4dot) {
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif


    q1dot = (-(cg_Omega_cB_cB_3*q2) + cg_Omega_cB_cB_2*q3 + cg_Omega_cB_cB_1*q4)/2 ;
    q2dot =    (cg_Omega_cB_cB_3*q1 - cg_Omega_cB_cB_1*q3 + cg_Omega_cB_cB_2*q4)/2 ;
    q3dot = (-(cg_Omega_cB_cB_2*q1) + cg_Omega_cB_cB_1*q2 + cg_Omega_cB_cB_3*q4)/2 ;
    q4dot = (-(cg_Omega_cB_cB_1*q1) - cg_Omega_cB_cB_2*q2 - cg_Omega_cB_cB_3*q3)/2 ;

}
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
	  double& q4dot) {

    q1dot = (-(cg_Omega_cB_cB_3*q2) + cg_Omega_cB_cB_2*q3 + cg_Omega_cB_cB_1*q4)/2 ;
    q2dot =    (cg_Omega_cB_cB_3*q1 - cg_Omega_cB_cB_1*q3 + cg_Omega_cB_cB_2*q4)/2 ;
    q3dot = (-(cg_Omega_cB_cB_2*q1) + cg_Omega_cB_cB_1*q2 + cg_Omega_cB_cB_3*q4)/2 ;
    q4dot = (-(cg_Omega_cB_cB_1*q1) - cg_Omega_cB_cB_2*q2 - cg_Omega_cB_cB_3*q3)/2 ;

}

// omega_q_qdot computes the inverse to the qdot routine
void omega_q_qdot(const double q[4], const double qdot[4], double omega[3])
{
  double q1 = 2.0*q[0];
  double q2 = 2.0*q[1];
  double q3 = 2.0*q[2];
  double q4 = 2.0*q[3];

  omega[0] =   q4*qdot[0] - q3*qdot[1] + q2*qdot[2] - q1*qdot[3]; 
  omega[1] =   q3*qdot[0] + q4*qdot[1] - q1*qdot[2] - q2*qdot[3]; 
  omega[2] =  -q2*qdot[0] + q1*qdot[1] + q4*qdot[2] - q3*qdot[3];
 
}

// Calculates the time derivate of the Euler parameters q1 ... q4.
void qdot_om_cg(const double q1,
          const double q2,
          const double q3,
          const double q4,
          const double3& Omega_cg,
          double& q1dot,
          double& q2dot,
          double& q3dot,
          double& q4dot) {
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
    // validate_euler checks so the square sum isn't to far from 1.0


    double Omega1, Omega2, Omega3 ;
    Omega_cg.Get(Omega1, Omega2, Omega3);

    q1dot =( q4*Omega1 - q3*Omega2 + q2*Omega3 )/2.0 ;
    q2dot =( q3*Omega1 + q4*Omega2 - q1*Omega3 )/2.0 ;
    q3dot =( -q2*Omega1 + q1*Omega2 + q4*Omega3)/2.0 ;
    q4dot =( -q1*Omega1 - q2*Omega2 - q3*Omega3)/2.0 ;
}

// Calculates the time derivate of the Euler parameters q1 ... q4.
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
          double& q4dot) {
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
    // validate_euler checks so the square sum isn't to far from 1.0
    q1dot =( q4*omega_cg_1 - q3*omega_cg_2 + q2*omega_cg_3 )/2.0 ;
    q2dot =( q3*omega_cg_1 + q4*omega_cg_2 - q1*omega_cg_3 )/2.0 ;
    q3dot =( -q2*omega_cg_1 + q1*omega_cg_2 + q4*omega_cg_3)/2.0 ;
    q4dot =( -q1*omega_cg_1 - q2*omega_cg_2 - q3*omega_cg_3)/2.0 ;
}

//#if 0
void omega_q_qdot_T(const double q1 ,const double q2,const double q3,const double q4,
		    const double a,  const double b, const double c, 
		    double &out1, double &out2, double &out3, double &out4)
{
  // Compute the transpose of omega_q_dot

  out1 =  (          c * q2 +  b * q3 +  a * q4)*2.0;
  out2 =  (c * q1 -            a * q3 +  b * q4)*2.0;
  out3 =  (b * q1 +  a * q2 +            c * q4)*2.0;
  out4 =  (a * q1 -  b * q2 -  c * q3          )*2.0;
}
//#endif

void FormT(double T[4][4], const double q[4])
{
  double q2[4];
  q2[0]=q[0]/2;
  q2[1]=q[1]/2;
  q2[2]=q[2]/2;
  q2[3]=q[3]/2;
  // Transformation matrix T(q) in qdot = T(q) omega
  // The matrix is collapsed in routine qdot
  // This matrix can be found in SubJac::SetAnalytical_dXdot_dvX

  T[0][0]=  q2[3];
  T[0][1]= -q2[2];
  T[0][2]=  q2[1];
  T[0][3]= -q2[0];

  T[1][0]=  q2[2];
  T[1][1]=  q2[3];
  T[1][2]= -q2[0];
  T[1][3]= -q2[1];

  T[1][0]= -q2[1];
  T[1][1]=  q2[0];
  T[1][2]=  q2[3];
  T[1][3]= -q2[2];

}



void FormS(double S[4][4],const double3& omega)
{
  // 
  //          (  0   w3 -w2  -w1 )
  // S(w)=1/2 ( -w3   0  w1  -w2 )
  //          (  w1 -w1   0  -w3 )
  //          (  w1  w2  w3   0  )
  // Fortran style layout S(i,j)=S[i][j]
  // This matric can also be found in SubJac::SetAnalytcal_dXdot_dX

  double od[3];
  od[0]=omega(1)/2;
  od[1]=omega(2)/2;
  od[2]=omega(3)/2;

  S[0][0]= 0.0;
  S[0][1]= od[2];
  S[0][2]=-od[1];
  S[0][3]=-od[0];

  S[1][0]=-od[2];
  S[1][1]= 0.0;
  S[1][2]= od[0];
  S[1][3]=-od[1];

  S[2][0]= od[1];
  S[2][1]=-od[0];
  S[2][2]= 0.0;
  S[2][3]=-od[2];

  S[3][0]= od[0];
  S[3][1]= od[1];
  S[3][2]= od[2];
  S[3][3]= 0.0;

}

void FormSS(double SS[4][4],const double q[4],const double qdot[4])
{
  double const &q1=q[0];
  double const &q2=q[1];
  double const &q3=q[2];
  double const &q4=q[3];
  double const &q1p=qdot[0];
  double const &q2p=qdot[1];
  double const &q3p=qdot[2];
  double const &q4p=qdot[3];

  // d(T(q_dot)/dq_dot T^{-1} q_dot
  SS[0][0]=   0.0;
  SS[0][1]= - q2*q1p + q1*q2p + q4*q3p - q3*q4p;
  SS[0][2]= - q3*q1p - q4*q2p + q1*q3p + q2*q4p;
  SS[0][3]= - q4*q1p + q3*q2p - q2*q3p + q1*q4p;
		
  SS[1][0]=   q2*q1p - q1*q2p - q4*q3p + q3*q4p;
  SS[1][1]=   0.0;
  SS[1][2]=   q4*q1p - q3*q2p + q2*q3p - q1*q4p;
  SS[1][3]= - q3*q1p - q4*q2p + q1*q3p + q2*q4p;

  SS[2][0]=   q3*q1p + q4*q2p - q1*q3p - q2*q4p;
  SS[2][1]= - q4*q1p + q3*q2p - q2*q3p + q1*q4p;
  SS[2][2]=   0.0;
  SS[2][3]=   q2*q1p - q1*q2p - q4*q3p + q3*q4p;

  SS[3][0]=   q4*q1p - q3*q2p + q2*q3p - q1*q4p;
  SS[3][1]=   q3*q1p + q4*q2p - q1*q3p - q2*q4p;
  SS[3][2]= - q2*q1p + q1*q2p + q4*q3p - q3*q4p;
  SS[3][3]=   0.0;

  // T(Q_dot) T ^{-1}
  SS[0][0]+=  -q1*q1p;
  SS[0][1]+=  -q1*q2p + q4*q3p - q3*q4p;
  SS[0][2]+=  -q4*q2p - q1*q3p + q2*q4p;
  SS[0][3]+=   q3*q2p - q2*q3p - q1*q4p;
  
  SS[1][0]+=  -q2*q1p - q4*q3p + q3*q4p;
  SS[1][1]+=  -q2*q2p;
  SS[1][2]+=   q4*q1p - q2*q3p - q1*q4p;
  SS[1][3]+=  -q3*q1p + q1*q3p - q2*q4p;
  
  
  SS[2][0]+=  -q3*q1p + q4*q2p - q2*q4p;
  SS[2][1]+=  -q4*q1p - q3*q2p + q1*q4p;
  SS[2][2]+=  -q3*q3p;
  SS[2][3]+=   q2*q1p - q1*q2p - q3*q4p;
  
  
  SS[3][0]+=  -q4*q1p - q3*q2p + q2*q3p;
  SS[3][1]+=   q3*q1p - q4*q2p - q1*q3p;
  SS[3][2]+=  -q2*q1p + q1*q2p - q4*q3p;
  SS[3][3]+=  -q4*q4p;

}



// Create the inverse to a symmetric matrix.
const double33 invAsym(const double xx, 
		       const double xy, 
		       const double xz,
		       const double yy, 
		       const double yz, 
		       const double zz){
    
    double tmp = xz*xz*yy - 2*xy*xz*yz + xx*yz*yz + xy*xy*zz - xx*yy*zz ;
    assert(tmp != 0.0) ;
    double t1 = (-xz*yz + xy*zz)/tmp ;
    double t2 = ( xz*yy - xy*yz)/tmp ;
    double t3 = (-xy*xz + xx*yz)/tmp ;
    
    return double33((yz*yz-yy*zz)/tmp, t1, t2, 
		    t1, (xz*xz-xx*zz)/tmp, t3,
		    t2, t3, (xy*xy-xx*yy)/tmp) ;
}



// Not currently used. The RotSym is an optimized version of this.
void RotSym2(// Input parameters 
    const double axx,
    const double axy,
    const double axz,
    const double ayy,
    const double ayz,
    const double azz,
    const double33& A,
    // Output parameters 
    double& bxx,
    double& bxy,
    double& bxz,
    double& byy,
    double& byz,
    double& bzz)
{   // res = A . Asym . invA
    
    double33 J, invA;
    
    J = Asym(axx,axy,axz,ayy,ayz,azz);
    J = A * J;
    invA = A.T();
    J = J * invA;

    bxx = J(1,1);
    bxy = J(1,2);
    bxz = J(1,3);
    byy = J(2,2);
    byz = J(2,3);
    bzz = J(3,3);
}


void RotSym(// Input parameters 
    const double axx,
    const double axy,
    const double axz,
    const double ayy,
    const double ayz,
    const double azz,
    const double33& A,
    // Output parameters 
    double& bxx,
    double& bxy,
    double& bxz,
    double& byy,
    double& byz,
    double& bzz)
{
    // This is an optimized version of RotSym2
    double Tmp_17995;
    double Tmp_17996;
    double Tmp_17997;
    double Tmp_17998;
    double Tmp_17999;
    double Tmp_18000;
    double Tmp_18001;
    double Tmp_18002;
    double Tmp_18003;
    double Tmp_18004;
    double Tmp_18005;
    double Tmp_18006;
    double Tmp_18007;
    double Tmp_18008;
    double Tmp_18009;
    double Tmp_18010;
    double Tmp_18011;
    double Tmp_18012;
    double Tmp_18013;
    double Tmp_18014;
    double Tmp_18015;
    double Tmp_18016;
    double Tmp_18017;
    double Tmp_18018;
    double Tmp_18019;
    double Tmp_18020;
    double Tmp_18021;
    double Tmp_18022;
    double Tmp_18023;
    double Tmp_18024;
    double Tmp_18025;
    double Tmp_18026;
    double Tmp_18027;
    double Tmp_18028;
    double Tmp_18029;
    double Tmp_18030;
    double Tmp_18031;
    double Tmp_18032;
    double Tmp_18033;
    double Tmp_18034;
    double Tmp_18035;
    double Tmp_18036;
    double Tmp_18037;
    double Tmp_18038;
    double Tmp_18039;
    double Tmp_18040;
    double Tmp_18041;
    double Tmp_18042;
    double Tmp_18043;
    double Tmp_18044;
    double Tmp_18045;
    double Tmp_18046;
    double Tmp_18047;
    double Tmp_18048;
    double Tmp_18049;
    double Tmp_18050;
    double Tmp_18051;
    double Tmp_18052;
    double Tmp_18053;
    double Tmp_18054;
    double Tmp_18055;
    double Tmp_18056;
    double Tmp_18057;
    double Tmp_18058;
    double Tmp_18059;
    double Tmp_18060;
    double Tmp_18061;
    double Tmp_18062;
    double Tmp_18063;
    double Tmp_18064;
    double Tmp_18065;
    double Tmp_18066;
    double Tmp_18067;
    double Tmp_18068;
    double Tmp_18069;

    // Body of RotSym 
    Tmp_17995 = A(1,1);
    Tmp_17996 = Tmp_17995*Tmp_17995;
    Tmp_17997 = axx*Tmp_17996;
    Tmp_17998 = A(1,2);
    Tmp_17999 = 2*axy*Tmp_17995*Tmp_17998;
    Tmp_18000 = Tmp_17998*Tmp_17998;
    Tmp_18001 = ayy*Tmp_18000;
    Tmp_18002 = A(1,3);
    Tmp_18003 = 2*axz*Tmp_17995*Tmp_18002;
    Tmp_18004 = 2*ayz*Tmp_17998*Tmp_18002;
    Tmp_18005 = Tmp_18002*Tmp_18002;
    Tmp_18006 = azz*Tmp_18005;
    Tmp_18007 = Tmp_17997 + Tmp_17999 + Tmp_18001 + Tmp_18003 + Tmp_18004 + Tmp_18006;
    Tmp_18008 = axx*Tmp_17995;
    Tmp_18009 = axy*Tmp_17998;
    Tmp_18010 = axz*Tmp_18002;
    Tmp_18011 = Tmp_18008 + Tmp_18009 + Tmp_18010;
    Tmp_18012 = A(2,1);
    Tmp_18013 = Tmp_18011*Tmp_18012;
    Tmp_18014 = axy*Tmp_17995;
    Tmp_18015 = ayy*Tmp_17998;
    Tmp_18016 = ayz*Tmp_18002;
    Tmp_18017 = Tmp_18014 + Tmp_18015 + Tmp_18016;
    Tmp_18018 = A(2,2);
    Tmp_18019 = Tmp_18017*Tmp_18018;
    Tmp_18020 = axz*Tmp_17995;
    Tmp_18021 = ayz*Tmp_17998;
    Tmp_18022 = azz*Tmp_18002;
    Tmp_18023 = Tmp_18020 + Tmp_18021 + Tmp_18022;
    Tmp_18024 = A(2,3);
    Tmp_18025 = Tmp_18023*Tmp_18024;
    Tmp_18026 = Tmp_18013 + Tmp_18019 + Tmp_18025;
    Tmp_18027 = A(3,1);
    Tmp_18028 = Tmp_18011*Tmp_18027;
    Tmp_18029 = A(3,2);
    Tmp_18030 = Tmp_18017*Tmp_18029;
    Tmp_18031 = A(3,3);
    Tmp_18032 = Tmp_18023*Tmp_18031;
    Tmp_18033 = Tmp_18028 + Tmp_18030 + Tmp_18032;
    Tmp_18034 = Tmp_18012*Tmp_18012;
    Tmp_18035 = axx*Tmp_18034;
    Tmp_18036 = 2*axy*Tmp_18012*Tmp_18018;
    Tmp_18037 = Tmp_18018*Tmp_18018;
    Tmp_18038 = ayy*Tmp_18037;
    Tmp_18039 = 2*axz*Tmp_18012*Tmp_18024;
    Tmp_18040 = 2*ayz*Tmp_18018*Tmp_18024;
    Tmp_18041 = Tmp_18024*Tmp_18024;
    Tmp_18042 = azz*Tmp_18041;
    Tmp_18043 = Tmp_18035 + Tmp_18036 + Tmp_18038 + Tmp_18039 + Tmp_18040 + Tmp_18042;
    Tmp_18044 = axx*Tmp_18012;
    Tmp_18045 = axy*Tmp_18018;
    Tmp_18046 = axz*Tmp_18024;
    Tmp_18047 = Tmp_18044 + Tmp_18045 + Tmp_18046;
    Tmp_18048 = Tmp_18047*Tmp_18027;
    Tmp_18049 = axy*Tmp_18012;
    Tmp_18050 = ayy*Tmp_18018;
    Tmp_18051 = ayz*Tmp_18024;
    Tmp_18052 = Tmp_18049 + Tmp_18050 + Tmp_18051;
    Tmp_18053 = Tmp_18052*Tmp_18029;
    Tmp_18054 = axz*Tmp_18012;
    Tmp_18055 = ayz*Tmp_18018;
    Tmp_18056 = azz*Tmp_18024;
    Tmp_18057 = Tmp_18054 + Tmp_18055 + Tmp_18056;
    Tmp_18058 = Tmp_18057*Tmp_18031;
    Tmp_18059 = Tmp_18048 + Tmp_18053 + Tmp_18058;
    Tmp_18060 = Tmp_18027*Tmp_18027;
    Tmp_18061 = axx*Tmp_18060;
    Tmp_18062 = 2*axy*Tmp_18027*Tmp_18029;
    Tmp_18063 = Tmp_18029*Tmp_18029;
    Tmp_18064 = ayy*Tmp_18063;
    Tmp_18065 = 2*axz*Tmp_18027*Tmp_18031;
    Tmp_18066 = 2*ayz*Tmp_18029*Tmp_18031;
    Tmp_18067 = Tmp_18031*Tmp_18031;
    Tmp_18068 = azz*Tmp_18067;
    Tmp_18069 = Tmp_18061 + Tmp_18062 + Tmp_18064 + Tmp_18065 + Tmp_18066 + Tmp_18068;

    bxx = Tmp_18007;
    bxy = Tmp_18026;
    bxz = Tmp_18033;
    byy = Tmp_18043;
    byz = Tmp_18059;
    bzz = Tmp_18069;
}  // end of RotSym 




// Methods for class intN

intN::intN(const int lenIn)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new int[len];
    assert(data!=0);
}


intN::intN(const int lenIn, const int var)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new int[len];
    assert(data!=0);

    if (var==0.0) {
        //should speedup construction of big arrays. // BZ516
	memset(data,0,sizeof(int)*len);
    }
    else
    {
       for (int i=0; i<len; i++) {
          data[i] = var;
       }
    }
}

intN::intN(const intN& var)
    : len(var.len), data(0)
{
    // Is expensive, try to avoid using it!
    assert(False);

    data = new int[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

intN::~intN()
{
    delete [] data;
}

intN& intN::operator=(const intN& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


intN& intN::operator=(const int var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}





// Methods for class doubleN
doubleN::doubleN()
    : len(0), data(0), XMFlg(False)
{
}

doubleN::doubleN(const int lenIn)
    : len(lenIn), data(0), XMFlg(False)
{
    assert(lenIn>0);
    data = new double[len];
    assert(data!=0);
}

doubleN::doubleN(const int lenIn, const double var)
    : len(lenIn), data(0), XMFlg(False)
{
    assert(lenIn>0);
    data = new double[len];
    assert(data!=0);

    if (var==0.0) {
        //should speedup construction of big arrays. // BZ516
	memset(data,0,sizeof(double)*len);
    }
    else {

      for (int i=0; i<len; i++) {
        data[i] = var;
      }
    }	

}


// doubleN::doubleN(const int lenIn, double* dataPtr)
// : len(lenIn), data(0), XMFlg(True)
// {
//     assert(lenIn>0);
//     assert(dataPtr!=0);
//     data = dataPtr;
// }

void doubleN::resize(const int lenIn)
{
    assert(!XMFlg);

    int oldLen = len;
    len = lenIn;
    assert(lenIn>0);
    double* oldData = data;    
    data = new double[len];
    assert(data!=0);
    if(oldData != 0) {
	if(oldLen > len) oldLen = len;
	for(int i = 0; i < oldLen; i++)
	    data[i]=oldData[i];
	delete [] oldData;
    }
}


void doubleN::resize(const int lenIn, const double var)
{
    assert(!XMFlg);

    int oldLen = len;
    len = lenIn;
    assert(lenIn>0);
    double* oldData = data;    
    data = new double[len];
    assert(data!=0);

    if(oldData != 0) {
        assert(oldLen>0);
	if(oldLen > len) oldLen = len;
	for(int i = 0; i < oldLen; i++)
	    data[i]=oldData[i];
	delete [] oldData;

        for(int ii = oldLen; ii < len; ii++)
            data[ii] = var;

    }
    else {
        for(int i = 0; i < len; i++)
            data[i] = var;
    }
}

void doubleN::resize4ExtMem(const int lenIn, double* dataPtr)
{
    assert(lenIn>0);
    assert(dataPtr!=0);

    if (XMFlg || (len==0 && data==0 && !XMFlg)) {
        // Only for default constructor or XMFlg=True
        len = lenIn;
        data = dataPtr;

        XMFlg = True;

    } else {
        assert(False);
    }
}



doubleN::doubleN(const doubleN& var)
    : len(var.len), data(0), XMFlg(False)
{
    // Is expensive, try to avoid using it!
    assert(False);

    data = new double[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

doubleN::~doubleN()
{
    if (!XMFlg) delete [] data;
}

doubleN& doubleN::operator=(const doubleN& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


doubleN& doubleN::operator=(const double var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}

// M == M
bool operator==(const doubleN& a, const doubleN& b)
{
    if (a.len!=b.len) return false;  
    int equal=true;

    for (int i=0; i<a.len; i++) {
        if (a.data[i]!=b.data[i]) {
            equal=false;
            break;
        }
    }

    return equal;
}



// Methods for class doubleN3

doubleN3::doubleN3()
    : len(0), data(0)
{
}


doubleN3::doubleN3(const int lenIn)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double3[len];
    assert(data!=0);
}


doubleN3::doubleN3(const int lenIn, const double var)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double3[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN3::doubleN3(const doubleN3& var)
    : len(var.len), data(0)
{
    // Is expensive, try to avoid using it!
    assert(False);

    data = new double3[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

doubleN3::~doubleN3()
{
    delete [] data;
}

void doubleN3::Init(const int lenIn)
{
    len = lenIn;
    assert(len>0);
    delete data;
    data = new double3[len];
    assert(data!=0);
}

void doubleN3::Init(const int lenIn, const double var)
{
    len = lenIn;
    delete data;
    data = new double3[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN3& doubleN3::operator=(const doubleN3& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


doubleN3& doubleN3::operator=(const double var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}





// Methods for class doubleN33

doubleN33::doubleN33()
    : len(0), data(0)
{
}


doubleN33::doubleN33(const int lenIn)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double33[len];
    assert(data!=0);
}


doubleN33::doubleN33(const int lenIn, const double var)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double33[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN33::doubleN33(const doubleN33& var)
    : len(var.len), data(0)
{
    // Is expensive, try to avoid using it!
    assert(False);

    data = new double33[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

doubleN33::~doubleN33()
{
    delete [] data;
}

void doubleN33::Init(const int lenIn)
{
    len = lenIn;
    assert(len>0);
    delete data;
    data = new double33[len];
    assert(data!=0);
}


void doubleN33::Init(const int lenIn, const double var)
{
    len = lenIn;
    assert(len>0);
    data = new double33[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN33& doubleN33::operator=(const doubleN33& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


doubleN33& doubleN33::operator=(const double var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}



