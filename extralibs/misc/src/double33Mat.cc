
#include "double33Mat.h"
#include <limits>

//////////////////////////////////////////////////////////////
//
// .SS Constants
//
// These are the defined constants
//

namespace tlmMisc
{

const double33Mat zerodouble33 = double33Mat(0.0);

const double33Mat Idouble33 = double33Mat(1.0, 0.0, 0.0,
				    0.0, 1.0, 0.0,
				    0.0, 0.0, 1.0);
// A identity matrix 




// Non-inlined functions.
double33Mat::double33Mat(const double a11, const double a12, const double a13,
		   const double a21, const double a22, const double a23,
		   const double a31, const double a32, const double a33)
{
    x[0] = a11; x[1] = a12; x[2] = a13;
    x[3] = a21; x[4] = a22; x[5] = a23;
    x[6] = a31; x[7] = a32; x[8] = a33;
}

double33Mat::double33Mat(const double33Mat& val)
{
    x[0] = val.x[0]; x[1] = val.x[1]; x[2] = val.x[2];
    x[3] = val.x[3]; x[4] = val.x[4]; x[5] = val.x[5];
    x[6] = val.x[6]; x[7] = val.x[7]; x[8] = val.x[8];
}

double33Mat& double33Mat::operator=(const double33Mat& m) 
{
    if (this != &m) {
	x[0] = m.x[0]; x[1] = m.x[1]; x[2] = m.x[2];
	x[3] = m.x[3]; x[4] = m.x[4]; x[5] = m.x[5];
	x[6] = m.x[6]; x[7] = m.x[7]; x[8] = m.x[8];
    }
    return *this;
}



double33Mat& double33Mat::operator*=(const double33Mat& b)
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

void double33Mat::Set(const double a11, const double a12, const double a13,
		   const double a21, const double a22, const double a23,
		   const double a31, const double a32, const double a33)
{
    x[0] = a11; x[1] = a12; x[2] = a13;
    x[3] = a21; x[4] = a22; x[5] = a23;
    x[6] = a31; x[7] = a32; x[8] = a33;
}


void double33Mat::Get(double& a11,  double& a12,  double& a13,
		   double& a21,  double& a22,  double& a23,
		   double& a31,  double& a32,  double& a33) const
{
    a11 = x[0]; a12 = x[1]; a13 = x[2];
    a21 = x[3]; a22 = x[4]; a23 = x[5];
    a31 = x[6]; a32 = x[7]; a33 = x[8];
}


void double33Mat::SetExpMatrix(const double3Vec& w, const double t)
{
    const double norm = w.norm2();
    
    double33Mat R(Idouble33);
    if (norm > std::numeric_limits<double>::epsilon()) {
    
        // W(1,1) =   0;   W(1,2) = -w(3); W(1,3) =  w(2);
        // W(2,1) =  w(3); W(2,2) =   0;   W(2,3) = -w(1);
        // W(3,1) = -w(2); W(3,2) =  w(1); W(3,3) =   0;
        
        // This is more efficient that lot of index operations.
        double w1, w2, w3;
        w.Get(w1, w2, w3);

        double33Mat W( 0.0, -w3,   w2,
                    w3,   0.0, -w1,
                   -w2,   w1,   0.0);

        // Intermediate variables to avoid multiple matrix divisions.
        double a = norm / ::sin(norm*t);
        double b = (norm *norm) / (1.0 - ::cos(norm*t));
        R += W/a;
        R += (W*W)/b;
    }    
    
    // Set this matrix to be transpose of R, i.e.,  operator=( R.T() );
    x[0] = R(1,1); x[3] = R(1,2); x[6] = R(1,3);
    x[1] = R(2,1); x[4] = R(2,2); x[7] = R(2,3);
    x[2] = R(3,1); x[5] = R(3,2); x[8] = R(3,3);
}

// M * V
const double3Vec operator*(const double33Mat& a, const double3Vec& b)
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

    return double3Vec(XX, YY, ZZ);

#if 0
    // This old implementation has problem with inlining for SUN C++.
    return double3(a.x[0]*b.XX + a.x[1]*b.YY + a.x[2]*b.ZZ,
		   a.x[3]*b.XX + a.x[4]*b.YY + a.x[5]*b.ZZ,
		   a.x[6]*b.XX + a.x[7]*b.YY + a.x[8]*b.ZZ);
#endif
}	

// V * M
const double3Vec operator*(const double3Vec& a, const double33Mat& b)
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

    return double3Vec(XX, YY, ZZ);

#if 0
    // This old implementation has problem with inlining for SUN C++.
    return double3(a.XX*b.x[0] + a.YY*b.x[3] + a.ZZ*b.x[6],
		   a.XX*b.x[1] + a.YY*b.x[4] + a.ZZ*b.x[7],
		   a.XX*b.x[2] + a.YY*b.x[5] + a.ZZ*b.x[8]);
#endif
}	

std::ostream& operator<<(std::ostream& o, const double33Mat& M)
{
    return o << "( " << M.x[0] << ' ' << M.x[1] << ' ' << M.x[2] << "\n"
         << M.x[3] << ' ' << M.x[4] << ' ' << M.x[5] << "\n"
         << M.x[6] << ' ' << M.x[7] << ' ' << M.x[8] << " )";
}





std::string ToStr(const double33Mat& val)
{
  std::string res;
  res = "  ( " + ToStr(val.x[0]) + " " + ToStr(val.x[1]) + " " + ToStr(val.x[2]) + "  ";
  res+= "  " + ToStr(val.x[3]) + " " + ToStr(val.x[4]) + " " + ToStr(val.x[5]) + "  ";
  res+= "  " + ToStr(val.x[6]) + " " + ToStr(val.x[7]) + " " + ToStr(val.x[8]) + " )";
  return res;
}

const double33Mat Max(const double33Mat& a,const double33Mat& b)
{
    return double33Mat(a.x[0]>b.x[0]?a.x[0]:b.x[0],
                    a.x[1]>b.x[1]?a.x[1]:b.x[1],
                    a.x[2]>b.x[2]?a.x[2]:b.x[2],
                    a.x[3]>b.x[3]?a.x[3]:b.x[3],
                    a.x[4]>b.x[4]?a.x[4]:b.x[4],
                a.x[5]>b.x[5]?a.x[5]:b.x[5],
                    a.x[6]>b.x[6]?a.x[6]:b.x[6],
                    a.x[7]>b.x[7]?a.x[7]:b.x[7],
                    a.x[8]>b.x[8]?a.x[8]:b.x[8]);
}

const double33Mat Min(const double33Mat& a,const double33Mat& b)
{
    return double33Mat(a.x[0]<b.x[0]?a.x[0]:b.x[0],
                    a.x[1]<b.x[1]?a.x[1]:b.x[1],
                    a.x[2]<b.x[2]?a.x[2]:b.x[2],
                    a.x[3]<b.x[3]?a.x[3]:b.x[3],
                    a.x[4]<b.x[4]?a.x[4]:b.x[4],
                    a.x[5]<b.x[5]?a.x[5]:b.x[5],
                    a.x[6]<b.x[6]?a.x[6]:b.x[6],
                    a.x[7]<b.x[7]?a.x[7]:b.x[7],
                    a.x[8]<b.x[8]?a.x[8]:b.x[8]);
}

#if 0
const double33Mat fabs(const double33Mat& a)
{
    return double33Mat( fabs(a.x[0]),
                        fabs(a.x[1]),
                        fabs(a.x[2]),
                        fabs(a.x[3]),
                        fabs(a.x[4]),
                        fabs(a.x[5]),
                        fabs(a.x[6]),
                        fabs(a.x[7]),
                        fabs(a.x[8]));
}
#endif

}
