#include <cmath>
#include <cassert>
#include <float.h>
#include "coordTrans.h"
#include "mathDef.h"

namespace tlmMisc
{

// Create a transformation matrix from base system to local system.
// We use 1,2,3 rotation scheme.
const double33Mat A123(const double3Vec& phi) 
{ 
    double c1 = ::cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    return double33Mat( c2 * c3,     c1 * s3 + s1 * s2 * c3,    s1 * s3 - c1 * s2 * c3, 
		    -c2 * s3,     c1 * c3 - s1 * s2 * s3,    s1 * c3 + c1 * s2 * s3, 
 		     s2,         -s1 * c2,                   c1 * c2               );
}


// Create an inverse transformation matrix from base system to local system.
// We use 1,2,3 rotation scheme.
const double33Mat invA123(const double3Vec& phi) 
{ 
    double c1 = ::cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    return double33Mat( c2 * c3,                 -c2 * s3,                    s2, 
                     c1 * s3 + s1 * s2 * c3,   c1 * c3 - s1 * s2 * s3,    -s1 * c2, 
                     s1 * s3 - c1 * s2 * c3,   s1 * c3 + c1 * s2 * s3,     c1 * c2);
}


// Create a transformation matrix from base system to local system.
// We use 3,2,1 rotation scheme.
const double33Mat A321(const double3Vec& phi) 
{ 
    double c1 = ::cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    return double33Mat( c2 * c3, 	               c2 * s3,                    -s2, 
		    -c1 * s3 + s1 * s2 * c3,   c1 * c3 + s1 * s2 * s3, 	    s1 * c2, 
                     s1 * s3 + c1 * s2 * c3,  -s1 * c3 + c1 * s2 * s3, 	    c1 * c2);
}


// Create an inverse transformation matrix from base system to local system.
// We use 3,2,1 rotation scheme.
const double33Mat invA321(const double3Vec& phi) 
{ 
    double c1 = ::cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    return double33Mat( c2 * c3,     -c1 * s3 + s1 * s2 * c3,    s1 * s3 + c1 * s2 * c3, 
		     c2 * s3,      c1 * c3 + s1 * s2 * s3,   -s1 * c3 + c1 * s2 * s3, 
		    -s2,           s1 * c2,                   c1 * c2               );
}

// Create a transformation matrix from base system to local system.
const double33Mat A(const double3Vec& phi, const int phiSequence) 
{
    switch (phiSequence) {
    case 0: // We have 321 rotation.
	return A321(phi);
	break;
    case 1: // We have 123 rotation.
	return A123(phi);
	break;
    default:
        assert(false);
	return double33Mat(0.0); // Just dummy.
    }
}

// Create a transformation matrix from base system to local system.
// We use only rotation around the axis number 3 (z-axis).
const double33Mat A3(const double phi_3) 
{ 
    double c3 = ::cos(phi_3);
    double s3 = ::sin(phi_3);

    return double33Mat(  c3,  s3,  0.0, 
		     -s3,  c3,  0.0, 
		      0.0, 0.0, 1.0);
}


// Create a transformation matrix from base system to local system.
// We use only rotation around the axis number 2 (y-axis).
const double33Mat A2(const double phi_2) 
{ 
    double c2 = ::cos(phi_2);
    double s2 = ::sin(phi_2);

    return double33Mat( c2,  0.0, -s2, 
		     0.0, 1.0,  0.0, 
		     s2,  0.0,  c2);
}


// Create a transformation matrix from base system to local system.
// We use only rotation around the axis number 1 (x-axis).
const double33Mat A1(const double phi_1) 
{ 
    double c1 = ::cos(phi_1);
    double s1 = ::sin(phi_1);

    return double33Mat(1.0,  0.0, 0.0, 
		    0.0,  c1,  s1, 
		    0.0, -s1,  c1);
}

const double33Mat U123(const double3Vec& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    // Transformation matrix U We use 1,2,3 rotation

    return double33Mat( c2 * c3,  s3,  0.0,
		    -c2 * s3,  c3,  0.0,
		     s2,       0.0, 1.0);
}

const double33Mat U321(const double3Vec& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double c1 = ::cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);

    // Transformation matrix U We use 3,2,1 rotation

    return double33Mat(1.0,  0.0,  -s2,
		    0.0,  c1,    s1*c2,
		    0.0, -s1,    c1*c2);
}


const double33Mat H321(const double3Vec& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    // Transformation matrix H We use 3,2,1 rotation


    return double33Mat( c2 * c3, -s3,  0.0,
		     c2 * s3,  c3,  0.0,
		    -s2,       0.0, 1.0);
}


const double33Mat H123(const double3Vec& phi) 
{ 
    // Transformation matrix H We use 1,2,3 rotation

    //return (invA123(phi) * U123(phi));
    // Temp. variables for cos and sin etc.
    double c1 = ::cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);

    return double33Mat(1.0, 0.0,  s2, 
		    0.0, c1,  -c2*s1,
		    0.0, s1,   c1*c2);

}


const double33Mat invU123(const double3Vec& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double c2 = ::cos(phi.YY);
    c2 += Sign(c2)*1.0e-50;
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    // Transformation matrix U.  We use 1,2,3 rotation

    return double33Mat( c3/c2,   -s3/c2,    0.0,
		     s3,       c3,       0.0,
		    -s2*c3/c2, s2*s3/c2, 1.0);
}



const double33Mat invH321(const double3Vec& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double c2 = cos(phi.YY);
    c2 += Sign(c2)*1.0e-50;
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    // Transformation matrix invH.  We use 3,2,1 rotation

    return double33Mat( c3/c2,    s3/c2,    0.0,
		    -s3,       c3,       0.0,
		     s2*c3/c2, s2*s3/c2, 1.0);
}

const double33Mat invH123(const double3Vec& phi) 
{ 
    // Transformation matrix invH We use 1,2,3 rotation

    //return (invU123(phi) * A123(phi));

    // Temp. variables for cos and sin etc.
    double c1 = ::cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = ::cos(phi.YY);
    c2 += Sign(c2)*1.0e-50;
    double s2 = ::sin(phi.YY);

    return double33Mat(1.0,  s1*s2/c2, -c1*s2/c2, 
		    0.0,  c1,        s1, 
		    0.0, -s1/c2,     c1/c2);

}

const double33Mat invH(const double3Vec& phi, const int phiSequence)
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
	return double33Mat(0.0); // Just dummy.
    }

}


const double33Mat invU321(const double3Vec& phi) 
{ 
    // Temp. variables for cos and sin etc.
    double c1 = ::cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = ::cos(phi.YY);
    c2 += Sign(c2)*1.0e-50;
    double s2 = ::sin(phi.YY);

    // Transformation matrix invU.  We use 3,2,1 rotation

    return double33Mat(1.0, s2*s1/c2, s2*c1/c2,
		    0.0, c1,      -s1,
		    0.0, s1/c2,    c1/c2);
}



const double33Mat vU123(const double3Vec& phi, const double3Vec& vphi) 
{ 
    // Temp. variables for cos and sin etc.
    double c2 = ::cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = ::cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    // Transformation matrix vU.  We use 1,2,3 rotation
    return double33Mat(-s2*c3*vphi.YY - c2*s3*vphi.ZZ,   c3*vphi.ZZ,     0.0,
		     s2*s3*vphi.YY - c2*c3*vphi.ZZ,  -s3 * vphi.ZZ,   0.0,
 		     c2 * vphi.YY,                    0.0,            0.0);
}


const double33Mat vH123(const double3Vec& phi,
		     const double3Vec& vphi) 
{ 
    return (invA123(phi) * vU123(phi,vphi));
}



const double33Mat vH321(const double3Vec& phi,
		     const double3Vec& vphi) 
{ 
    // Temp. variables for cos and sin etc.
    double c2 = cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    return double33Mat(-c3*vphi.YY*s2 - c2*vphi.ZZ*s3,  -c3*vphi.ZZ,  0.0, 
		     c2*c3*vphi.ZZ - vphi.YY*s2*s3,  -vphi.ZZ*s3,  0.0, 
		    -c2*vphi.YY,                      0.0,         0.0);
}


const double33Mat vU321(const double3Vec& phi,
		     const double3Vec& vphi) 
{ 
    // Temp. variables for cos and sin etc.
    double c2 = cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c1 = cos(phi.XX);
    double s1 = ::sin(phi.XX);

    return double33Mat(0.0,    0.0,         -c2*vphi.YY,
		    0.0,   -vphi.XX*s1,   c1*c2*vphi.XX - vphi.YY*s1*s2, 
		    0.0,   -c1*vphi.XX,  -c2*vphi.XX*s1 - c1*vphi.YY*s2);
}


const double3Vec Omega_U123(const double3Vec& phi,
			 const double3Vec& vphi) 
{ 
    // Angular speed Omega_cBase this.  We use 1,2,3 rotation.
    // Omega[cBase][this] = U[cBase] . vphi[cBase]
    // return (U123(phi) * vphi);

    // Temp. variables for cos and sin etc.
    double c2 = cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    // Transformation matrix U We use 1,2,3 rotation

    return double3Vec(c2*c3*vphi.XX + s3*vphi.YY,
		   -c2*s3*vphi.XX + c3*vphi.YY,
		   s2*vphi.XX + vphi.ZZ);

}



const double3Vec Omega_H123(const double3Vec& phi,
			 const double3Vec& vphi) 
{ 
    // Angular speed Omega_cBase_cBase.  We use 1,2,3 rotation.
    // Omega[cBase][cBase] = H[cBase] . vphi[cBase]
    return (H123(phi) * vphi);
}


const double3Vec Omega_H321(const double3Vec& phi,
			 const double3Vec& vphi) 
{ 
    // Angular speed Omega_cBase_cBase.  We use 3,2,1 rotation.
    // Omega[cBase][cBase] = H[cBase] . vphi[cBase]

    // Temp. variables for cos and sin etc.
    double c2 = cos(phi.YY);
    double s2 = ::sin(phi.YY);
    double c3 = cos(phi.ZZ);
    double s3 = ::sin(phi.ZZ);

    return double3Vec(c2*c3*vphi.XX - s3*vphi.YY,
		   c2*s3*vphi.XX + c3*vphi.YY,
		   -s2*vphi.XX + vphi.ZZ);
}


const double3Vec Omega_U321(const double3Vec& phi,
			 const double3Vec& vphi) 
{ 
    // Angular speed Omega_cBase this.  We use 3,2,1 rotation.
    // Omega[cBase][this] = U[cBase] . vphi[cBase]
    // return (U321(phi) * vphi);

    // Temp. variables for cos and sin etc.
    double c1 = cos(phi.XX);
    double s1 = ::sin(phi.XX);
    double c2 = cos(phi.YY);
    double s2 = ::sin(phi.YY);

    // Transformation matrix U We use 3,2,1 rotation

    return double3Vec(vphi.XX - s2*vphi.ZZ,
		   c1*vphi.YY + s1*c2*vphi.ZZ,
		   -s1*vphi.YY + c1*c2*vphi.ZZ);
}

//! Transformation from cylindrical coordinates and its 1-st & 2-nd derivative to acceleration in Cartesian coordinates.
const double3Vec aRC2aR(const double3Vec& RC, 
                     const double3Vec& vRC,
                     const double3Vec& aRC)
{
    double c = cos(RC.YY);
    double s = ::sin(RC.YY);

    double x = c * RC.XX;
    double y = s * RC.XX;

    double vx = -vRC.YY * y + c * vRC.XX;
    double vy =  vRC.YY * x + s * vRC.XX;

    return double3Vec(c * aRC.XX - ( aRC.YY * y + vRC.YY * ( vy + s * vRC.XX)),
		   aRC.YY * x + vRC.YY * (vx + c * vRC.XX) + s * aRC.YY,
		   aRC.ZZ);
}


const double3Vec aR2aRC(const double3Vec& aR,
		     const double3Vec& RC, 
		     const double3Vec& vRC) 
{
    double c = cos(RC.YY);
    double s = ::sin(RC.YY);

    double aR_cmp_1 =  c*aR.XX + s*aR.YY;
    double aR_cmp_2 = -s*aR.XX + c*aR.YY;

    double aRC_1 = aR_cmp_1 + RC.XX * vRC.YY*vRC.YY;
    double aRC_2 = (aR_cmp_2 - 2 * vRC.XX * vRC.YY ) / RC.XX;

    return double3Vec(aRC_1, aRC_2, aR.ZZ);
}


const double3Vec vOmega_U123(const double3Vec& phi,
			  const double3Vec& vphi,
			  const double3Vec& aphi) 
{ 
    // Angular acc. vOmega_cBase_cBase_this.  We use 1,2,3 rotation.
    // vOmega[cBase][cBase,this] =  vOmega[cBase][this,this] =
    // vU[cBase] . vphi[cBase] + U[cBase] . aphi[cBase];
    
    return ((vU123(phi,vphi) * vphi) + (U123(phi) * aphi));
}




const double3Vec vOmega_H123(const double3Vec& phi,
			  const double3Vec& vphi,
			  const double3Vec& aphi) 
{ 
    // Angular acc. vOmega_cBase_cBase_cBase.  We use 1,2,3 rotation.
    // vOmega[cBase][cBase,cBase] =  vOmega[cBase][this,cBase] =
    // vH[cBase] . vphi[cBase] + H[cBase] . aphi[cBase];
    
    return ((vH123(phi,vphi) * vphi) + (H123(phi) * aphi));
}


const double3Vec vOmega_U321(const double3Vec& phi,
			  const double3Vec& vphi,
			  const double3Vec& aphi) 
{ 
    // Angular acc. vOmega_cBase_cBase_this.  We use 3,2,1 rotation.
    // vOmega[cBase][cBase,this] =  vOmega[cBase][this,this] =
    // vU[cBase] . vphi[cBase] + U[cBase] . aphi[cBase];
    
    return ((vU321(phi,vphi) * vphi) + (U321(phi) * aphi));
}




const double3Vec vOmega_H321(const double3Vec& phi,
			  const double3Vec& vphi,
			  const double3Vec& aphi) 
{ 
    // Angular acc. vOmega_cBase_cBase_cBase.  We use 3,2,1 rotation.
    // vOmega[cBase][cBase,cBase] =  vOmega[cBase][this,cBase] =
    // vH[cBase] . vphi[cBase] + H[cBase] . aphi[cBase];
    
    return ((vH321(phi,vphi) * vphi) + (H321(phi) * aphi));
}




int validate_euler(double q1,double q2,double q3,double q4) 
{

    double fel = fabs(sqrt(q1*q1+q2*q2+q3*q3+q4*q4) - 1);

    if (fel>1.0e-6) {
	return 0 ;
    }
    return 1;
}


int validate_euler2(double q1,double q2,double q3,double q4) 
{

    double fel = fabs(sqrt(q1*q1+q2*q2+q3*q3+q4*q4) - 1);
    if (fel>2.0e-15) {
	return 0 ;
    }
    return 1;
}


int validate_euler(double* q) {
    return validate_euler(q[0], q[1], q[2], q[3]);
} 


const double33Mat Aeuler(const double q1,
		      const double q2,
		      const double q3,
		      const double q4)
{ 
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif

    double a11 = q1*q1 - q2*q2 - q3*q3 + q4*q4; double a12 = 2.0*(q1*q2 + q3*q4);           double a13 = 2.0*(q1*q3 - q2*q4);
    double a21 = 2.0*(q1*q2 - q3*q4);           double a22 = q2*q2 - q3*q3 - q1*q1 + q4*q4; double a23 = 2.0*(q2*q3 + q1*q4) ;
    double a31 = 2.0*(q1*q3 + q2*q4);           double a32 = 2.0*(q2*q3 - q1*q4);           double a33 = q3*q3 - q1*q1 - q2*q2 + q4*q4;
            
    return double33Mat(a11, a12, a13,
		    a21, a22, a23,
		    a31, a32, a33) ;
}

const double33Mat invAeuler(const double q1,
			 const double q2,
			 const double q3,
			 const double q4)
{ 
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif

    // Note these elements are for the not transposed matrix.
    double a11 = q1*q1 - q2*q2 - q3*q3 + q4*q4; double a12 = 2.0*(q1*q2 + q3*q4);           double a13 = 2.0*(q1*q3 - q2*q4); 
    double a21 = 2.0*(q1*q2 - q3*q4);           double a22 = q2*q2 - q3*q3 - q1*q1 + q4*q4; double a23 = 2.0*(q2*q3 + q1*q4); 
    double a31 = 2.0*(q1*q3 + q2*q4);           double a32 = 2.0*(q2*q3 - q1*q4);           double a33 = q3*q3 - q1*q1 - q2*q2 + q4*q4 ;

    // Here we transpose the data before setting.
    return double33Mat(a11, a21, a31,
		    a12, a22, a32,
		    a13, a23, a33) ;
}

const double3Vec EulerTophi321(const double q1,
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
  
    // This implementation limits ph1 and phi3 to +/-pi so we use it.
    double phi1 = atan2(a23/cosphi2, a33/cosphi2);
    double phi3 = atan2(a12/cosphi2, a11/cosphi2);
    
    return double3Vec(phi1, phi2, phi3) ;
}


const double3Vec ATophi321(const double33Mat& A)
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
  
    // This implementation limits ph1 and phi3 to +/-pi so we use it.
    double phi1 = atan2(a23/cosphi2, a33/cosphi2);
    double phi3 = atan2(a12/cosphi2, a11/cosphi2);

    if(fabs(phi1) < DBL_MIN) phi1 = 0.0;
    if(fabs(phi3) < DBL_MIN) phi3 = 0.0;

    return double3Vec(phi1, phi2, phi3) ;
}

const double3Vec ATophi123(const double33Mat& A)
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
  
    // This implementation limits ph1 and phi3 to +/-pi so we use it.
    double phi1 = atan2(-a32/cosphi2, a33/cosphi2);
    double phi3 = atan2(-a21/cosphi2, a11/cosphi2);

    if(fabs(phi1) < DBL_MIN) phi1 = 0.0;
    if(fabs(phi3) < DBL_MIN) phi3 = 0.0;

    return double3Vec(phi1, phi2, phi3) ;
}

const double3Vec ATophi(const double33Mat& A, const int phiSequence)
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
	return double3Vec(0.0); // Just dummy.
    }
}


void phi321ToEuler(const double3Vec& phi,
		   double& q1,
		   double& q2,
		   double& q3,
		   double& q4)
{
    double c1 = cos(phi.XX/2.0);
    double s1 = ::sin(phi.XX/2.0);
    double c2 = cos(phi.YY/2.0);
    double s2 = ::sin(phi.YY/2.0);
    double c3 = cos(phi.ZZ/2.0);
    double s3 = ::sin(phi.ZZ/2.0);
    
    q1 =  s1*c2*c3 - c1*s2*s3 ;
    q2 =  c1*s2*c3 + s1*c2*s3 ;
    q3 = -s1*s2*c3 + c1*c2*s3 ;
    q4 =  c1*c2*c3 + s1*s2*s3 ;
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
}


void phi321ToEulerDer(const double3Vec& phi,
                      double3Vec& dq1_dphi,
                      double3Vec& dq2_dphi,
                      double3Vec& dq3_dphi,
                      double3Vec& dq4_dphi)
{
    const double c1 = cos(phi.XX/2.0);
    const double s1 = ::sin(phi.XX/2.0);
    const double c2 = cos(phi.YY/2.0);
    const double s2 = ::sin(phi.YY/2.0);
    const double c3 = cos(phi.ZZ/2.0);
    const double s3 = ::sin(phi.ZZ/2.0);

    // q1 = s1*c2*c3 - c1*s2*s3;
    dq1_dphi.XX = ( c1*c2*c3 + s1*s2*s3)/2;
    dq1_dphi.YY = (-s1*s2*c3 - c1*c2*s3)/2;
    dq1_dphi.ZZ = (-s1*c2*s3 - c1*s2*c3)/2;
    // q2 = c1*s2*c3 + s1*c2*s3;
    dq2_dphi.XX = (-s1*s2*c3 + c1*c2*s3)/2;
    dq2_dphi.YY = ( c1*c2*c3 - s1*s2*s3)/2;
    dq2_dphi.ZZ = (-c1*s2*s3 + s1*c2*c3)/2;
    // q3 = -s1*s2*c3 + c1*c2*s3;
    dq3_dphi.XX = (-c1*s2*c3 - s1*c2*s3)/2;
    dq3_dphi.YY = (-s1*c2*c3 - c1*s2*s3)/2;
    dq3_dphi.ZZ = ( s1*s2*s3 + c1*c2*c3)/2;
    // q4 = c1*c2*c3 + s1*s2*s3;
    dq4_dphi.XX = (-s1*c2*c3 + c1*s2*s3)/2;
    dq4_dphi.YY = (-c1*s2*c3 + s1*c2*s3)/2;
    dq4_dphi.ZZ = (-c1*c2*s3 + s1*s2*c3)/2;
}

void phi123ToEuler(const double3Vec& phi,
		   double& q1,
		   double& q2,
		   double& q3,
		   double& q4)
{
    
    double c1 = cos(phi.XX/2.0);
    double s1 = ::sin(phi.XX/2.0);
    double c2 = cos(phi.YY/2.0);
    double s2 = ::sin(phi.YY/2.0);
    double c3 = cos(phi.ZZ/2.0);
    double s3 = ::sin(phi.ZZ/2.0);
    
    q1 =  s1*c2*c3 + c1*s2*s3 ;
    q2 =  c1*s2*c3 - s1*c2*s3 ;
    q3 =  s1*s2*c3 + c1*c2*s3 ;
    q4 =  c1*c2*c3 - s1*s2*s3 ;
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
}


void phi123ToEulerDer(const double3Vec& phi,
                      double3Vec& dq1_dphi,
                      double3Vec& dq2_dphi,
                      double3Vec& dq3_dphi,
                      double3Vec& dq4_dphi)
{

    const double c1 = cos(phi.XX/2.0);
    const double s1 = ::sin(phi.XX/2.0);
    const double c2 = cos(phi.YY/2.0);
    const double s2 = ::sin(phi.YY/2.0);
    const double c3 = cos(phi.ZZ/2.0);
    const double s3 = ::sin(phi.ZZ/2.0);

    // q1 = s1*c2*c3 + c1*s2*s3;
    dq1_dphi.XX = (c1*c2*c3 - s1*s2*s3)/2 ;
    dq1_dphi.YY = (-s1*s2*c3 + c1*c2*s3)/2 ;
    dq1_dphi.ZZ = (-s1*c2*s3 + c1*s2*c3)/2 ;
    // q2 = c1*s2*c3 - s1*c2*s3;
    dq2_dphi.XX =  (-s1*s2*c3 - c1*c2*s3)/2 ;
    dq2_dphi.YY =  (c1*c2*c3 + s1*s2*s3)/2 ;
    dq2_dphi.ZZ =  (-c1*s2*s3 - s1*c2*c3)/2 ;
    // q3 = s1*s2*c3 + c1*c2*s3;
    dq3_dphi.XX =  (c1*s2*c3 - s1*c2*s3)/2 ;
    dq3_dphi.YY =  (s1*c2*c3 - c1*s2*s3)/2 ;
    dq3_dphi.ZZ =  (-s1*s2*s3 + c1*c2*c3)/2 ;
    // q4 = c1*c2*c3 - s1*s2*s3;
    dq4_dphi.XX =  (-s1*c2*c3 - c1*s2*s3)/2 ;
    dq4_dphi.YY =  (-c1*s2*c3 - s1*c2*s3)/2 ;
    dq4_dphi.ZZ =  (-c1*c2*s3 - s1*s2*c3)/2 ;
}


void phiToEuler(const double3Vec& phi,
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


void AToEuler(const double33Mat& A,
	      double& q1,
	      double& q2,
	      double& q3,
	      double& q4)
{
    // The old implementation was removed by Klas Modin, 2006-04-21. It transforms first to angles
    // and then to Euler parameters. This is not a good solution, since it involves singularities
    // (angles does not cover all of SO(3)).

    // The new implementation always works, i.e. it contains no singularities. However, the Euler
    // parameters q are not uniquely determined by the rotation matrix A. Indeed, -q and q
    // represents the same rotation. There is some theory documentation in header file (lightmat.h).
    const double a11 = A.x[0]; const double a12 = A.x[1]; const double a13 = A.x[2];
    const double a21 = A.x[3]; const double a22 = A.x[4]; const double a23 = A.x[5];
    const double a31 = A.x[6]; const double a32 = A.x[7]; const double a33 = A.x[8];

    q1 = 0.5*sqrt(Max(0.0, 1+a11-a22-a33));
    q2 = 0.5*sqrt(Max(0.0, 1-a11+a22-a33));
    q3 = 0.5*sqrt(Max(0.0, 1-a11-a22+a33));
    q4 = 0.5*sqrt(Max(0.0, 1+a11+a22+a33));

    if (q1 >= q2 && q1 >= q3 && q1 >= q4) {
	// Assume q1 is positive
        if (a12+a21 < 0) q2 = -q2;
        if (a13+a31 < 0) q3 = -q3;
        if (a23-a32 < 0) q4 = -q4;
    }
    else if (q2 >= q1 && q2 >= q3 && q2 >= q4) {
	// Assume q2 is positive
        if (a12+a21 < 0) q1 = -q1;
        if (a23+a32 < 0) q3 = -q3;
        if (a31-a13 < 0) q4 = -q4;
    }
    else if (q3 >= q1 && q3 >= q2 && q3 >= q4) {
	// Assume q3 is positive
        if (a13+a31 < 0) q1 = -q1;
        if (a23+a32 < 0) q2 = -q2;
        if (a12-a21 < 0) q4 = -q4;
    }
    else {
	// Assume q4 is positive
        if (a23-a32 < 0) q1 = -q1;
        if (a31-a13 < 0) q2 = -q2;
        if (a12-a21 < 0) q3 = -q3;
    }

    // We choose the solution such that q1 is always positive.
    if (q1 < 0) {
	q1 = -q1;
	q2 = -q2;
	q3 = -q3;
	q4 = -q4;
    }

#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4));
#endif
}

//#define QDOT_LONGDOUBLE

void qdot(const double q1,
	  const double q2,
	  const double q3,
	  const double q4,
	  const double c2_Omega_c1_c1_1, 
	  const double c2_Omega_c1_c1_2, 
	  const double c2_Omega_c1_c1_3,
	  double& q1dot,
	  double& q2dot,
	  double& q3dot,
	  double& q4dot) 
{
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
#ifdef QDOT_LONGDOUBLE
    long double q1l = q1;
    long double q2l = q2;
    long double q3l = q3;
    long double q4l = q4;

    long double c2_Omega_c1_c1_1l = c2_Omega_c1_c1_1;
    long double c2_Omega_c1_c1_2l = c2_Omega_c1_c1_2;
    long double c2_Omega_c1_c1_3l = c2_Omega_c1_c1_3;

    long double q1dotl = (-c2_Omega_c1_c1_3l*q2l + c2_Omega_c1_c1_2l*q3l + c2_Omega_c1_c1_1l*q4l)/2.0L ;
    long double q2dotl = ( c2_Omega_c1_c1_3l*q1l - c2_Omega_c1_c1_1l*q3l + c2_Omega_c1_c1_2l*q4l)/2.0L ;
    long double q3dotl = (-c2_Omega_c1_c1_2l*q1l + c2_Omega_c1_c1_1l*q2l + c2_Omega_c1_c1_3l*q4l)/2.0L ;
    long double q4dotl = (-c2_Omega_c1_c1_1l*q1l - c2_Omega_c1_c1_2l*q2l - c2_Omega_c1_c1_3l*q3l)/2.0L ;

    q1dot = q1dotl;
    q2dot = q2dotl;
    q3dot = q3dotl;
    q4dot = q4dotl;
#else
    q1dot = (-c2_Omega_c1_c1_3*q2 + c2_Omega_c1_c1_2*q3 + c2_Omega_c1_c1_1*q4)/2.0 ;
    q2dot = ( c2_Omega_c1_c1_3*q1 - c2_Omega_c1_c1_1*q3 + c2_Omega_c1_c1_2*q4)/2.0 ;
    q3dot = (-c2_Omega_c1_c1_2*q1 + c2_Omega_c1_c1_1*q2 + c2_Omega_c1_c1_3*q4)/2.0 ;
    q4dot = (-c2_Omega_c1_c1_1*q1 - c2_Omega_c1_c1_2*q2 - c2_Omega_c1_c1_3*q3)/2.0 ;
#endif
}


void qdot_unsafe(const double q1,
                 const double q2,
                 const double q3,
                 const double q4,
                 const double c2_Omega_c1_c1_1, 
                 const double c2_Omega_c1_c1_2, 
                 const double c2_Omega_c1_c1_3,
                 double& q1dot,
                 double& q2dot,
                 double& q3dot,
                 double& q4dot) 
{
#ifdef QDOT_LONGDOUBLE
    long double q1l = q1;
    long double q2l = q2;
    long double q3l = q3;
    long double q4l = q4;

    long double c2_Omega_c1_c1_1l = c2_Omega_c1_c1_1;
    long double c2_Omega_c1_c1_2l = c2_Omega_c1_c1_2;
    long double c2_Omega_c1_c1_3l = c2_Omega_c1_c1_3;

    long double q1dotl = (-c2_Omega_c1_c1_3l*q2l + c2_Omega_c1_c1_2l*q3l + c2_Omega_c1_c1_1l*q4l)/2.0L ;
    long double q2dotl = ( c2_Omega_c1_c1_3l*q1l - c2_Omega_c1_c1_1l*q3l + c2_Omega_c1_c1_2l*q4l)/2.0L ;
    long double q3dotl = (-c2_Omega_c1_c1_2l*q1l + c2_Omega_c1_c1_1l*q2l + c2_Omega_c1_c1_3l*q4l)/2.0L ;
    long double q4dotl = (-c2_Omega_c1_c1_1l*q1l - c2_Omega_c1_c1_2l*q2l - c2_Omega_c1_c1_3l*q3l)/2.0L ;

    q1dot = q1dotl;
    q2dot = q2dotl;
    q3dot = q3dotl;
    q4dot = q4dotl;
#else
    q1dot = (-c2_Omega_c1_c1_3*q2 + c2_Omega_c1_c1_2*q3 + c2_Omega_c1_c1_1*q4)/2.0 ;
    q2dot = ( c2_Omega_c1_c1_3*q1 - c2_Omega_c1_c1_1*q3 + c2_Omega_c1_c1_2*q4)/2.0 ;
    q3dot = (-c2_Omega_c1_c1_2*q1 + c2_Omega_c1_c1_1*q2 + c2_Omega_c1_c1_3*q4)/2.0 ;
    q4dot = (-c2_Omega_c1_c1_1*q1 - c2_Omega_c1_c1_2*q2 - c2_Omega_c1_c1_3*q3)/2.0 ;
#endif
}




// Calculates the time derivate of the Euler parameters q1 ... q4.
void qdotL(const double q1,
           const double q2,
           const double q3,
           const double q4,
           const double c2_Omega_c1_c2_1,
           const double c2_Omega_c1_c2_2,
           const double c2_Omega_c1_c2_3,
           double& q1dot,
           double& q2dot,
           double& q3dot,
           double& q4dot) 
{
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif
    // validate_euler checks so the square sum isn't to far from 1.0

#ifdef QDOT_LONGDOUBLE
    long double q1l = q1;
    long double q2l = q2;
    long double q3l = q3;
    long double q4l = q4;

    long double c2_Omega_c1_c2_1l = c2_Omega_c1_c2_1;
    long double c2_Omega_c1_c2_2l = c2_Omega_c1_c2_2;
    long double c2_Omega_c1_c2_3l = c2_Omega_c1_c2_3;

    long double q1dotl =(  q4l*c2_Omega_c1_c2_1l - q3l*c2_Omega_c1_c2_2l + q2l*c2_Omega_c1_c2_3l )/2.0L ;
    long double q2dotl =(  q3l*c2_Omega_c1_c2_1l + q4l*c2_Omega_c1_c2_2l - q1l*c2_Omega_c1_c2_3l )/2.0L ;
    long double q3dotl =( -q2l*c2_Omega_c1_c2_1l + q1l*c2_Omega_c1_c2_2l + q4l*c2_Omega_c1_c2_3l )/2.0L ;
    long double q4dotl =( -q1l*c2_Omega_c1_c2_1l - q2l*c2_Omega_c1_c2_2l - q3l*c2_Omega_c1_c2_3l )/2.0L ;

    q1dot = q1dotl;
    q2dot = q2dotl;
    q3dot = q3dotl;
    q4dot = q4dotl;
#else
    q1dot =(  q4*c2_Omega_c1_c2_1 - q3*c2_Omega_c1_c2_2 + q2*c2_Omega_c1_c2_3 )/2.0 ;
    q2dot =(  q3*c2_Omega_c1_c2_1 + q4*c2_Omega_c1_c2_2 - q1*c2_Omega_c1_c2_3 )/2.0 ;
    q3dot =( -q2*c2_Omega_c1_c2_1 + q1*c2_Omega_c1_c2_2 + q4*c2_Omega_c1_c2_3 )/2.0 ;
    q4dot =( -q1*c2_Omega_c1_c2_1 - q2*c2_Omega_c1_c2_2 - q3*c2_Omega_c1_c2_3 )/2.0 ;
#endif
}


void qdotdot(const double q1,
             const double q2,
             const double q3,
             const double q4,
             const double q1dot,
             const double q2dot,
             const double q3dot,
             const double q4dot,
             const double c2_Omega_c1_c1_1,
             const double c2_Omega_c1_c1_2,
             const double c2_Omega_c1_c1_3,
             const double c2_vOmega_c1_c1_1,
             const double c2_vOmega_c1_c1_2,
             const double c2_vOmega_c1_c1_3,
             double& q1dotdot,
             double& q2dotdot,
             double& q3dotdot,
             double& q4dotdot)
{
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif

    // QDOT_LONGDOUBLE not yet implemented here.
    double r1a, r2a, r3a, r4a;
    double r1b, r2b, r3b, r4b;
    qdot(q1dot, q2dot, q3dot, q4dot,  c2_Omega_c1_c1_1,  c2_Omega_c1_c1_2,  c2_Omega_c1_c1_3, r1a, r2a, r3a, r4a);
    qdot(q1,    q2,    q3,    q4,    c2_vOmega_c1_c1_1, c2_vOmega_c1_c1_2, c2_vOmega_c1_c1_3, r1b, r2b, r3b, r4b);
    q1dotdot = r1a + r1b;
    q2dotdot = r2a + r2b;
    q3dotdot = r3a + r3b;
    q4dotdot = r4a + r4b;
}


void qdotdotL(const double q1,
              const double q2,
              const double q3,
              const double q4,
              const double q1dot,
              const double q2dot,
              const double q3dot,
              const double q4dot,
              const double c2_Omega_c1_c2_1,
              const double c2_Omega_c1_c2_2,
              const double c2_Omega_c1_c2_3,
              const double c2_vOmega_c1_c2_1,
              const double c2_vOmega_c1_c2_2,
              const double c2_vOmega_c1_c2_3,
              double& q1dotdot,
              double& q2dotdot,
              double& q3dotdot,
              double& q4dotdot)
{
#ifdef DEBUGFLG
    assert(validate_euler2(q1,q2,q3,q4)) ;
#endif

    // QDOT_LONGDOUBLE not yet implemented here.
    double r1a, r2a, r3a, r4a;
    double r1b, r2b, r3b, r4b;
    qdotL(q1dot, q2dot, q3dot, q4dot,  c2_Omega_c1_c2_1,  c2_Omega_c1_c2_2,  c2_Omega_c1_c2_3, r1a, r2a, r3a, r4a);
    qdotL(q1,    q2,    q3,    q4,    c2_vOmega_c1_c2_1, c2_vOmega_c1_c2_2, c2_vOmega_c1_c2_3, r1b, r2b, r3b, r4b);
    q1dotdot = r1a + r1b;
    q2dotdot = r2a + r2b;
    q3dotdot = r3a + r3b;
    q4dotdot = r4a + r4b;
}


#if 0
// Not used.
//! omega_q_qdot computes the inverse to the qdot routine.
//! Calculates Omega_{c2/c1}^{(c1)} from q_{c2/c1} and q_dot_{c2/c1}. 
//! Omega = (T^{-1}(q) q_dot
void omega_q_qdot(const double q[4], const double qdot[4], double c2_Omega_c1_c1[3])
{
  double q1 = 2.0*q[0];
  double q2 = 2.0*q[1];
  double q3 = 2.0*q[2];
  double q4 = 2.0*q[3];

  c2_Omega_c1_c1[0] =   q4*qdot[0] - q3*qdot[1] + q2*qdot[2] - q1*qdot[3]; 
  c2_Omega_c1_c1[1] =   q3*qdot[0] + q4*qdot[1] - q1*qdot[2] - q2*qdot[3]; 
  c2_Omega_c1_c1[2] =  -q2*qdot[0] + q1*qdot[1] + q4*qdot[2] - q3*qdot[3];
 
}
#endif

// Calculates the transpose of the omega_q_qdot ( T^{-T} [a;b;c] )
// Used in SubJac.cc
void omega_q_qdot_T(const double q1, const double q2, const double q3, const double q4,
		    const double a,  const double b, const double c, 
		    double& out1, double& out2, double& out3, double& out4)
{
    // Compute the transpose of omega_q_dot

    out1 =  (          c * q2 +  b * q3 +  a * q4)*2.0;
    out2 =  (c * q1 -            a * q3 +  b * q4)*2.0;
    out3 =  (b * q1 +  a * q2 +            c * q4)*2.0;
    out4 =  (a * q1 -  b * q2 -  c * q3          )*2.0;
}


#if 0 
// Not used.
void FormT(double T[4][4], const double q[4])
{
    // BUG Wrong dimension on T. It should be 4x3 matrix if the comment below is correct. DF 2015-02-24
    assert(false);

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

    // BUG must be wrong first index. DF 2014-02-24
    assert(false);
    T[1][0]= -q2[1]; 
    T[1][1]=  q2[0];
    T[1][2]=  q2[3];
    T[1][3]= -q2[2];

}


void FormS(double S[4][4], const double3& omega)
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

void FormSS(double SS[4][4], const double q[4], const double qdot[4])
{
    // BUG What is "double const"??  DF 2015-02-24
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
#endif




// Create the inverse to a symmetric matrix.
const double33Mat invAsym(const double xx, 
		       const double xy, 
		       const double xz,
		       const double yy, 
		       const double yz, 
		       const double zz)
{
    
    double tmp = xz*xz*yy - 2*xy*xz*yz + xx*yz*yz + xy*xy*zz - xx*yy*zz ;
    assert(tmp != 0.0) ;
    double t1 = (-xz*yz + xy*zz)/tmp ;
    double t2 = ( xz*yy - xy*yz)/tmp ;
    double t3 = (-xy*xz + xx*yz)/tmp ;
    
    return double33Mat((yz*yz-yy*zz)/tmp, t1, t2, 
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
             const double33Mat& A,
             // Output parameters 
             double& bxx,
             double& bxy,
             double& bxz,
             double& byy,
             double& byz,
             double& bzz)
{   // res = A . Asym . invA
    
    double33Mat J, invA;
    
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
            const double33Mat& A,
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





//! Return the maximum absolute value of the relative errors of each component.
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double a, const double b, const double tol)
{
    assert(tol>0.0);

    double rel = fabs(a-b)*2.0/(fabs(a)+fabs(b)+tol);

    return rel;
}

//! Return the maximum absolute value of the relative errors of each component.
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double3Vec& a, const double3Vec& b, const double tol)
{
    double rel = MaxRelAbsError(a.XX, b.XX, tol);
    rel = Max(rel, MaxRelAbsError(a.YY, b.YY, tol));
    rel = Max(rel, MaxRelAbsError(a.ZZ, b.ZZ, tol));

    return rel;
}

//! Return the maximum absolute value of the relative errors of each component.
//! Use a tolerance to avoid division by zero.
double MaxRelAbsError(const double33Mat& a, const double33Mat& b, const double tol)
{
    double rel = 0.0;
    for (int i=0; i<9; i++) {
	rel = Max(rel, MaxRelAbsError(a.x[i], b.x[i], tol));
    } 

    return rel;
}

}
