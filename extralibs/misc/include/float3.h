#ifndef FLOAT3_H
#define FLOAT3_H

class Bstring;
class double3;

#ifndef _DOUBLE3_H
#error This file should never be used alone. Use include double3.h instead.
#endif

#include "lightmatConfig.h"




class float3 {
    friend class double3;
    friend class ConverterFloat3Double3;
    friend class aPackBuf;

 public:
    inline float3 ()
#ifdef DEBUG_LIGHTMAT
    // The intention is to detect errors when these
    // fields are not initialized.
        :XX(1e38f),YY(1e38f),ZZ(1e38f)
#endif
        {};

    inline explicit  float3(const float f_in):
        XX(f_in), YY(f_in), ZZ(f_in)
    {};

    inline float3 (const float3& f3):
        XX(f3.XX),
        YY(f3.YY),
        ZZ(f3.ZZ)
            {};

#ifdef USE_IMPLICIT_CONV
    inline float3 (const double3& conv_in){
        XX=static_cast<float>(conv_in.XX);
        YY=static_cast<float>(conv_in.YY);
        ZZ=static_cast<float>(conv_in.ZZ);
    };
#endif


  //! Constructor that initializes the components to the given values
 inline float3(const float a1, const float a2, const float a3);

 friend inline const float3 operator+(const  float3  & a, const   float3 & b);
 friend inline const float3 operator-(const  float3  & a, const   float3 & b);
 friend inline bool operator==(const float3 & a, const float3 & b);
 friend inline bool operator!=(const float3& a, const float3& b);
   //! M += M
 inline float3& operator+=(const float3& m);
 //! M -= M
 inline float3& operator-=(const float3& m);

 //! Assignment.
 inline float3& operator=(const float3& m) ;
 friend Bstring ToStr(const float3& val);

 //! Access element 1,2 or 3. Can not be used a lvalue.
 //! Bounds checking if BOUNDS_CHECK is defined
 inline float operator()(const int i) const;

 //! Access element 1,2 or 3. Can be used as lvalue.
 //! Bounds checking if BOUNDS_CHECK is defined
 inline float& operator()(const int i);

 //! Index to access norm2()
 static const int norm2_index_5=5;

 //! Index to access norm2XY()
 static const int norm2XY_index_4=4;

 //! Return "length" of vector. Earlier it was function length(). //BZ297
 inline float norm2() const;

 //! Return "length" of XY components of vector.
 inline float norm2XY() const;

 //! Return true if all components are zeroes
 inline bool isZero() const;

 //!  Return true if at least one components is non zero
 inline bool isNonZero() const;

 private:

    float XX;
    float YY;
    float ZZ;
};

#ifdef USE_IMPLICIT_CONV
// Class not needed
#else

class ConverterFloat3Double3 {
 public:
    // This class is needed in order to privide
    // access to private members of  float3,  double3
    // without declaring a friend function.
    inline static float3 convertToFloat3(const double3 & d) {
        float3 f(static_cast<const float>(d.XX),
            static_cast<const float>(d.YY),
            static_cast<const float>(d.ZZ)
            );
        return f;
    };

    inline static double3 convertToDouble3(const float3 & f) {
        double3 d(f.XX,f.YY,f.ZZ);
        return d;
    };
};
#endif



#endif // FLOAT3_H
