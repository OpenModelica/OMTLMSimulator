#ifndef FLOAT33S_H
#define FLOAT33S_H
#include "lightmatConfig.h"
#include "Bstring.h"

#ifndef _DOUBLE33S_H_
#error This file should never be used alone. Use include double33s.h instead.
#endif

// for conversion

class BString;
class double33s;

class float33s {
    friend class ConverterFloat33sDouble33s;
    friend class aPackBuf;
    friend class double33s;
public:

    inline   float33s()
#ifdef DEBUG_LIGHTMAT
        : x11(1e38f), x22(1e38f), x33(1e38f), x12(1e38f), x23(1e38f), x13(1e38f)
    #endif
    {};

    //! Constructor that initializes the components to the given value.
    inline explicit  float33s(const float f_in);

    //! Access elements. Can not be used as lvalue.
    inline float operator()(const int row, const int col) const;


    //! Access elements. Can be used as lvalue.
    inline float& operator()(const int row, const int col) ;

#ifdef USE_IMPLICIT_CONV
    //! Constructor to convert double33s to  float33s
    inline float33s (const double33s & d)  {
        x11=static_cast<float>(d.x11);
        x22=static_cast<float>(d.x22);
        x33=static_cast<float>(d.x33);
        x12=static_cast<float>(d.x12);
        x23=static_cast<float>(d.x23);
        x13=static_cast<float>(d.x13);
    };
#endif


    // friend inline const float33s operator+(const float33s& a, const float33s& b);

    //! Constructor that initializes the components to the given values
    inline float33s(const float a11, const float a22, const float a33,
                    const float a12, const float a23, const float a13);

    friend inline const float33s operator+(const  float33s  & a, const   float33s & b);
    friend inline const float33s operator-(const  float33s  & a, const   float33s & b);

    friend inline bool operator==(const float33s & a, const float33s & b);
    friend inline bool operator!=(const float33s& a, const float33s& b);
    //! M += M
    inline float33s& operator+=(const float33s& m);
    //! M -= M
    inline float33s& operator-=(const float33s& m);

    //! Assignment.
    inline float33s& operator=(const float33s& m) ;
    friend Bstring ToStr(const float33s& val);


    //! Return true if all components are zeroes
    inline bool isZero() const;

    //!  Return true if at least one components is non zero
    inline bool isNonZero() const;

    //! Index to access calc_vonMises()
    static const int calc_vonMises_index_7=7;

     //! Compute effective (von Misses) stress
    inline float calc_vonMises() const;

    //! Protected data area.
private:
    float x11, x22, x33, x12, x23, x13;

};

#ifndef USE_IMPLICIT_CONV
class ConverterFloat33sDouble33s {
public:
    inline static  float33s convertToFloat33s(const double33s & d) {
        float33s f(static_cast<const float>(d.x11),
                   static_cast<const float>(d.x22),
                   static_cast<const float>(d.x33),
                   static_cast<const float>(d.x12),
                   static_cast<const float>(d.x23),
                   static_cast<const float>(d.x13));
        return f;
    };
    inline static double33s convertToDouble33s(const float33s & f) {
        double33s d(f.x11,f.x22,f.x33,f.x12,f.x23,f.x13);
        return d;
    };
};
#endif


#endif // FLOAT33S_H
