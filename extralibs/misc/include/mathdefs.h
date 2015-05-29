// #ident "$Id$"

#ifndef MATHDEFS_H
#define MATHDEFS_H

#include <cassert>


// ---------- Constants -----------------

//  These are not used anymore
//  #define LEFT -1
//  #define RIGHT 1
//  #define NEAREST 0

#define False 0
#define True 1
#define Maybe -1

//! The e value.
const double Exp_of_1      = 2.71828182845904523536029;
//! The cryptic name in order to avoid mixing with local variables

//! pi
const double pi     = 3.14159265358979323846264;
//! pi*2
const double pix2   = 3.14159265358979323846264 * 2;
//! pi/2
const double pi_2   = 3.14159265358979323846264 / 2;
//! pi/3
const double pi_3   = 3.14159265358979323846264 / 3;
//! pi/4
const double pi_4   = 3.14159265358979323846264 / 4;
//! pi/180
const double Degree = 0.01745329251994329576924;
//! Euler's constant (gamma)
const double EULER = 0.577215664901532860606512090082;

//! A very small positive number.
//! It should be possible to multiply it by itself without
//! obtaining total underflow, i.e. value zero.
//! So we choose sqrt(1.0e-300)=1.0e-150.
const double smallPositiveNumber = 1.0e-150;

//! Zero degrees Celsius in degrees Kelvin
const double Kelvin_0 = 273.15;

//! New line
#define nl std::endl;
// This is in order to differ from local variable called nl

//! These constants moved here from B_Spline_ValidateQ.h

const int defaultIntMin = -999999;
const int defaultIntMax = 999999;
const double defaultMin = -1.0e300;
const double defaultMax = 1.0e300;

// Used by slicefor.cc, matrixin.cc
const double BEAST_INFINITY      = 1.0e12;
const double TOLERANCE     = 1.0e-9;
const double INFINITESIMAL = 1.0e-12;




// ------------------ Functions ----------------------

#define CL(length) ((length + 1))            
// To be able to get C arrays from 1 to length 

#define even(x) ((x)%2 == 0 ? 1 : 0)
// Even number

#define odd(x) ((x)%2 != 0 ? 1 : 0)
// Odd number

//! The sign function. Returns -1 for negative numbers, otherwise +1.
//! Note it returns +1 for zero! This behaviour is utilized.
inline int Sign(const double x) { return ((x < 0) ? -1 : 1); }
inline int Sign(const int x) { return ((x < 0) ? -1 : 1); }

#define sign(x) ((x) < 0 ? -1 : 1)
#define NRCSign(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))


// inline double ASin(const double x)
// // A safe asin that ensures that the argument is within [-1,1].
// // Note, no error messages.
// {
//     return asin((x<-1.0) ? -1.0 : ((x>1.0) ? 1.0 : x ));
// }

// inline double ACos(const double x)
// // A safe acos that ensures that the argument is within [-1,1]
// // Note, no error messages.
// {
//    return acos((x<-1.0) ? -1.0 : ((x>1.0) ? 1.0 : x ));
// }
 
// --------- Min(a, b) functions for base types -------------

inline int Min(const int x, const int y) {
	 return( (x < y) ? x : y );
}

inline unsigned int Min(const unsigned int x, const unsigned int y) {
	 return( (x < y) ? x : y );
}

inline long Min(const long x, const long y) {
	 return( (x < y) ? x : y );
}

inline long long Min(const long long x, const long long y) {
	 return( (x < y) ? x : y );
}

inline unsigned long Min(const unsigned long x, const unsigned long y) {
	 return( (x < y) ? x : y );
}

inline float Min(const float x, const float y) {
	 return( (x < y) ? x : y );
}

inline double Min(const double x, const double y) {
	 return( (x < y) ? x : y );
}

// --------- Min(a, b, c) functions for base types -------------

inline int Min(const int x, const int y, const int z) {
    return( (x < y) ? (x < z ? x : z) : (y < z ? y : z));
}

inline unsigned int Min(const unsigned int x, const unsigned int y, const unsigned int z) {
   return( (x < y) ? (x < z ? x : z) : (y < z ? y : z));
}

inline long Min(const long x, const long y, const long z) {
    return( (x < y) ? (x < z ? x : z) : (y < z ? y : z));
}

inline unsigned long Min(const unsigned long x, const unsigned long y, const unsigned long z) {
   return( (x < y) ? (x < z ? x : z) : (y < z ? y : z));
}

inline long long Min(const long long x, long long y, long long z) {
   return( (x < y) ? (x < z ? x : z) : (y < z ? y : z));
}

inline float Min(const float x, const float y, const float z) {
    return( (x < y) ? (x < z ? x : z) : (y < z ? y : z));
}

inline double Min(const double x, const double y, const double z) {
    return( (x < y) ? (x < z ? x : z) : (y < z ? y : z));
}

// --------- Min(a, b, c) functions for base types -------------

inline int Min(const int w, const int x, const int y, const int z) {
    return Min(Min(w, x), Min(y, z));
}

inline unsigned int Min(const unsigned int w, const unsigned int x, 
			const unsigned int y, const unsigned int z) {
    return Min(Min(w, x), Min(y, z));
}

inline long Min(const long w, const long x, const long y, const long z) {
    return Min(Min(w, x), Min(y, z));
}

inline unsigned long Min(const unsigned long w, const unsigned long x, 
			 const unsigned long y, const unsigned long z) {
   return Min(Min(w, x), Min(y, z));
}

inline float Min(const float w, const float x, const float y, const float z) {
    return Min(Min(w, x), Min(y, z));
}

inline double Min(const double w, const double x, const double y, const double z) {
    return Min(Min(w, x), Min(y, z));
}

// --------- Max(a, b) functions for base types -------------

inline int Max(const int x, const int y) {
	 return( (x > y) ? x : y );
}

inline unsigned int Max(const unsigned int x, const unsigned int y) {
	 return( (x > y) ? x : y );
}

inline long Max(const long x, const long y) {
	 return( (x > y) ? x : y );
}

inline long long Max(const long long x, const long long y) {
	 return( (x > y) ? x : y );
}

//inline size_t Max(const size_t x, const size_t y) {
//	 return( (x > y) ? x : y );
//}


inline unsigned long Max(const unsigned long x, const unsigned long y) {
	 return( (x > y) ? x : y );
}

inline float Max(const float x, const float y) {
	 return( (x > y) ? x : y );
}

inline double Max(const double x, const double y) {
	 return( (x > y) ? x : y );
}

// --------- Max(a, b, c) functions for base types -------------

inline int Max(const int x, const int y, const int z) {
    return( (x > y) ? (x > z ? x : z) : (y > z ? y : z));
}

inline unsigned int Max(const unsigned int x, const unsigned int y, const unsigned int z) {
    return( (x > y) ? (x > z ? x : z) : (y > z ? y : z));
}

inline long Max(const long x, const long y, const long z) {
    return( (x > y) ? (x > z ? x : z) : (y > z ? y : z));
}

inline long long Max(const long long x, const long long y, const long long z) {
    return( (x > y) ? (x > z ? x : z) : (y > z ? y : z));
}

inline unsigned long Max(const unsigned long x, const unsigned long y, const unsigned long z) {
    return( (x > y) ? (x > z ? x : z) : (y > z ? y : z));
}

inline float Max(const float x, const float y, const float z) {
    return( (x > y) ? (x > z ? x : z) : (y > z ? y : z));
}

inline double Max(const double x, const double y, const double z) {
    return( (x > y) ? (x > z ? x : z) : (y > z ? y : z));
}

// --------- Max(a, b, c, d) functions for base types -------------

inline int Max(const int w, const int x, const int y, const int z) {
    return Max(Max(w, x), Max(y, z));
}

inline unsigned int Max(const unsigned int w, const unsigned int x, 
			const unsigned int y, const unsigned int z) {
    return Max(Max(w, x), Max(y, z));
}

inline long Max(const long w, const long x, const long y, const long z) {
    return Max(Max(w, x), Max(y, z));
}

inline unsigned long Max(const unsigned long w, const unsigned long x, 
			 const unsigned long y, const unsigned long z) {
    return Max(Max(w, x), Max(y, z));
}

inline float Max(const float w, const float x, const float y, const float z) {
    return Max(Max(w, x), Max(y, z));
}

inline double Max(const double w, const double x, const double y, const double z) {
    return Max(Max(w, x), Max(y, z));
}


// --------- LimitMinMax(x, minlim, maxlim) functions for base types -------------

inline int LimitMinMax(const int x, const int minlim, const int maxlim) {
    return( (x > maxlim) ? maxlim : (x < minlim ? minlim : x) );
}

inline unsigned int LimitMinMax(const unsigned int x, const unsigned int minlim, const unsigned int maxlim) {
    return( (x > maxlim) ? maxlim : (x < minlim ? minlim : x) );
}

inline long LimitMinMax(const long x, const long minlim, const long maxlim) {
    return( (x > maxlim) ? maxlim : (x < minlim ? minlim : x) );
}

inline long long  LimitMinMax(const long long x, const long long minlim, const long long maxlim) {
    return( (x > maxlim) ? maxlim : (x < minlim ? minlim : x) );
}

inline float LimitMinMax(const float x, const float minlim, const float maxlim) {
    return( (x > maxlim) ? maxlim : (x < minlim ? minlim : x));
}

inline double LimitMinMax(const double x, const double minlim, const double maxlim) {
    return( (x > maxlim) ? maxlim : (x < minlim ? minlim : x));
}


// --------- special double functions -----------------------

inline int sameSide(double x, double y, double z, double w) {
    if ( (x >= w && y >= w && z >= w) ||
	(x <= w && y <= w && z <= w) ) {
	return ( 1 );
    }
    else {
	return ( 0 );
    }
}

inline int sameSign(double x, double y, double w = 0.0) {
    if ( (x >= w && y >= w) || (x <= w && y <= w) ) {
	return ( 1 );
    }
    else {
	return ( 0 );
    }
}



//! The parameter must be inside the interval [0,2*PI].
//! Therefore we "align" this value if necessary.
inline double AlignCyclic_2pi(const double aIn)
{
    double frac = aIn/pix2;
    double a;
    if(frac<0.0){
        assert(frac>-2.0e9);  // This test is for the integer conversion below.
	int div = int(frac);
	a = pix2*(frac+double(1-div)); // div negative or zero
    } else if(frac>=1.0){
        assert(frac<2.0e9);  // This test is for the integer conversion below.
	int div = int(frac);
	a = pix2*(frac-double(div));
    } else {
	a = pix2*frac;
    }

    // Due to numerical features it can happend that a = pix2.
    if (a>=pix2) a = 0.0;

    return a;
}

//! The parameter must be inside the interval [-pi,pi[.
//! Therefore we "align" this value if necessary.
inline double AlignCyclic_pi_pi(const double aIn)
{
    double frac = (aIn+pi)/pix2;
    double a;
    if(frac<0.0){
        assert(frac>-2.0e9); // This test is for the integer conversion below.
	int div = int(frac);
	a = -pi + pix2*(frac+double(1-div)); // div negative or zero
    } else if(frac>=1.0){
        assert(frac<2.0e9);  // This test is for the integer conversion below.
	int div = int(frac);
	a = -pi + pix2*(frac-double(div));
    } else {
	a = -pi + pix2*frac;
    }

    // Due to numerical features it can happend that a = pi.
    if (a>=pi) a = -pi;
    
    return a;
}


//! The parameter must be inside the interval [-pi,pi[.
//! Therefore we "align" this value if necessary.
//! Same as above, but uses long long to get a larger range.
inline double AlignCyclicLL_pi_pi(const double aIn)
{
    double frac = (aIn+pi)/pix2;
    double a;
    if(frac<0.0){
        assert(frac>-9.0e18); // This test is for the integer conversion below.
	long long div = static_cast<long long>(frac);
	a = -pi + pix2*(frac+double(1-div)); // div negative or zero
    } else if(frac>=1.0){
        assert(frac<9.0e18);  // This test is for the integer conversion below.
	long long div = static_cast<long long>(frac);
	a = -pi + pix2*(frac-double(div));
    } else {
	a = -pi + pix2*frac;
    }

    // Due to numerical features it can happend that a = pi.
    if (a>=pi) a = -pi;
    
    return a;
}


//! The parameter must be inside the interval [-pi,pi].
//! Therefore we "align" this value if necessary.
//! Both -pi and +pi are valid values
inline double AlignCyclic_pi_pi_closed(const double aIn)
{
    double frac = (aIn+pi)/pix2;

    double a;
    if(frac<0.0){
        assert(frac>-2.0e9); // This test is for the integer conversion below.
	int div = int(frac);
	a = -pi + pix2*(frac+double(1-div)); // div negative or zero
    } else if(frac>1.0){
        assert(frac<2.0e9);  // This test is for the integer conversion below.
	int div = int(frac);
	a = -pi + pix2*(frac-double(div));
    } else {
	a = -pi + pix2*frac;
    }

    return a;
}


#if 0

// We can not assume that compare operators are declared
// for all objects, therefore we build Min and Max functions
// only for base types.
// For other objects write special Min, Max class member functions.   

template <class Type>
inline Type Min(Type x, Type y) {
	 return( (x < y) ? x : y );
}

template <class Type>
inline Type Min(Type x, Type y, Type z) {
    return( (x < y) ? (x < z ? x : z) : (y < z ? y : z));
}

template <class Type>
inline Type Min(Type x1, Type x2, Type x3, Type x4) {
    return( Min(Min(x1, x2), Min(x3, x4)));
}
 
template <class Type>
inline Type Max(Type x, Type y) {
	 return( (x > y) ? x : y );
}

template <class Type>
inline Type Max(Type x, Type y, Type z) {
	 return( (x > y) ? (x > z ? x : z) : (y > z ? y : z));
}

template <class Type>
inline Type Max(Type x1, Type x2, Type x3, Type x4) {
	 return( Max(Max(x1, x2), Max(x3, x4)));
}

template <class Type>
inline int sameSide(Type x, Type y, Type z, Type w) {
	 if ( (x >= w && y >= w && z >= w) ||
	(x <= w && y <= w && z <= w) ) {
	return ( 1 );
	 }
	 else {
	return ( 0 );
	 }
}

template <class Type>
inline int sameSign(Type x, Type y, Type w) {
    if ( (x >= w && y >= w) || (x <= w && y <= w) ) {
	return ( 1 );
    }
    else {
	return ( 0 );
    }
}
#endif




#endif
