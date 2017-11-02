#ifndef REALN_H
#define REALN_H



#include <cassert>

#include "lightmatConfig.h"
#include "portable_longdouble.h"

////////////////////////////////////////////////////////////////////////
//
//! \class RealN
//!
//!
//!  This class describes an array of Real.
//!
//!
//!
//!
//!


#ifdef USING_LONG_DOUBLE

class double3;

class RealN {

// Friend class.
friend class aPackBuf ;
//! M == M
friend bool operator==(const RealN& a, const RealN& b);

public:

    //! Empty constructor. Len is set to zero. Resize is expected to be called later.
    RealN( );

    //! Constructor. No initialization of elements.
    RealN(const int lenIn);

    //! Constructor. Initialize with var.
    RealN(const int lenIn, const Real var);
    
    //! Constructor. Initialize each component.
    RealN(const Real x1, const Real x2, const Real x3);

    //! Constructor. Initialize 3 long and with a double3 value.
    explicit RealN(const double3& x);

    //! Copy constructor.
    RealN(const RealN& var);

    //! Destructor.
    ~RealN();

    //! Assignment operator.
    RealN& operator=(const RealN& var);

    //! Assignment operator. All elements get the value var.
    RealN& operator=(const Real var);

    //! Access element i of type Real.
    inline Real& operator()(const int i);

    //! Access element i of type Real. Cannot be used as lvalue.
    inline Real operator()(const int i) const;



    //! Gives the length of the dimension.
    int length() const { return len; };


    //! Resize the vector. No initialization of elements.
    void resize(const int lenIn);

    //! Resize the vector.
    //! If the new length is larger than the old one then var is used as init value for new elements.
    void resize(const int lenIn, const Real var);

    //! Resize the vector. Use external data storage.
    //! Note, it can onlybe done when created with constructor external data storage,
    //! or default constructor has been used.
    void resize4ExtMem(const int lenIn, Real* dataPtr);

    //! Get three first elements in the array. Assert len>=3.
    void Get3(Real& x1, Real& x2, Real& x3) const {
        assert(len>=3);
        x1 = data[0];
        x2 = data[1];
        x3 = data[2];
    };

    //! Get a double3 for the three first elements in the array. Assert len>=3.
    double3 Get3() const;

    //! Get four first elements in the array. Assert len>=4.
    void Get4(Real& x1, Real& x2, Real& x3, Real& x4) const {
        assert(len>=4);
        x1 = data[0];
        x2 = data[1];
        x3 = data[2];
        x4 = data[3];
    };


    //! Set four first elements in the array. Assert len>=4.
    void Set4(const Real x1, const Real x2, const Real x3, const Real x4) {
        assert(len>=4);
        data[0] = x1;
        data[1] = x2;
        data[2] = x3;
        data[3] = x4;
    };


private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointer to data area. Data index starts from 0.
    Real* data;

    //! If True then the data pointer is pointing to external memory
    //! and no local memory is allocated.
    int XMFlg;

    //! Length of preallocated short array
    enum {SLEN=6};

    Real sdata[SLEN];
};


////////////////////////////////////////////////////////////////////////
//
// .SS Declaration of functions
//
// Here we declare global functions related to RealN.
// For the description we use the notation:  "M" a RealN
//

 //! M == M
 bool operator==(const RealN& a, const RealN& b);


 // Inline definitions for RealN

 inline Real& RealN::operator()(const int i)
 {
 #ifdef DEBUGFLG
     assert(i>0 && i<=len);
 #endif
     return (data[i-1]);
 }

 inline Real RealN::operator()(const int i) const
 {
 #ifdef DEBUGFLG
     assert(i>0 && i<=len);
 #endif
     return (data[i-1]);
 }



#else

#include "doubleN.h"

typedef doubleN RealN;
#endif


#endif // REALN_H

