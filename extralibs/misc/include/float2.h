#ifndef FLOAT2_H
#define FLOAT2_H

class Bstring;
class double2;

#ifndef _DOUBLE2_H
#error This file should never be used alone. Use include double3.h instead.
#endif

#include "lightmatConfig.h"




class float2 {
    friend class double2;
    friend class ConverterFloat2Double2;
    friend class aPackBuf;
    friend inline const float2 operator-(const float2& v);
 public:
    inline float2 ()
#ifdef DEBUG_LIGHTMAT
    // The intention is to detect errors when these
    // fields are not initialized.
        :XX(1e38f),YY(1e38f)
#endif
        {};

    inline explicit  float2(const float f_in):
        XX(f_in), YY(f_in)
    {};

    inline float2 (const float2& f3):
        XX(f3.XX),
        YY(f3.YY)
            {};

#ifdef USE_IMPLICIT_CONV
    inline float2 (const double2& conv_in){
        XX=static_cast<float>(conv_in.XX);
        YY=static_cast<float>(conv_in.YY);

    };
#endif


  //! Constructor that initializes the components to the given values
 inline float2(const float a1, const float a2);

 friend inline const float2 operator+(const  float2  & a, const   float2 & b);
 friend inline const float2 operator-(const  float2  & a, const   float2 & b);
 friend inline bool operator==(const float2 & a, const float2 & b);
 friend inline bool operator!=(const float2& a, const float2& b);
   //! M += M
 inline float2& operator+=(const float2& m);
 //! M -= M
 inline float2& operator-=(const float2& m);



 //! Assignment.
 inline float2& operator=(const float2& m) ;
 friend Bstring ToStr(const float2& val);

 //! Access element 1 or 2. Can not be used a lvalue.
 //! Bounds checking if BOUNDS_CHECK is defined
 inline float operator()(const int i) const;

 //! Access element 1 or 2. Can be used as lvalue.
 //! Bounds checking if BOUNDS_CHECK is defined
 inline float& operator()(const int i);

 //! Index to access norm2()
 static const int norm2_index_3=3;

 //! Return "length" of vector. Earlier it was function length(). //BZ297
 inline float norm2() const;

 //! Return true if all components are zeroes
 inline bool isZero() const;

 //!  Return true if at least one components is non zero
 inline bool isNonZero() const;

 private:

    float XX;
    float YY;

};


//! -V
inline const float2 operator-(const float2& v);

#ifdef USE_IMPLICIT_CONV
// Class not needed
#else

class ConverterFloat2Double2 {
 public:
    // This class is needed in order to privide
    // access to private members of  float2,  double3
    // without declaring a friend function.
    inline static float2 convertToFloat3(const double3 & d) {
        float2 f(static_cast<const float>(d.XX),
            static_cast<const float>(d.YY)
            );
        return f;
    };

    inline static double3 convertToDouble3(const float2 & f) {
        double3 d(f.XX,f.YY);
        return d;
    };
};
#endif



#endif // FLOAT2_H
