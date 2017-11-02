#ifndef DOUBLEN_H
#define DOUBLEN_H

#include <cassert>

////////////////////////////////////////////////////////////////////////
//
//! \class doubleN
//!
//!
//!  This class describes an array of double.
//!
//!
//!
//!
//!

class double3;

class doubleN {

// Friend class.
friend class aPackBuf ;
//! M == M
friend bool operator==(const doubleN& a, const doubleN& b);

public:

    //! Empty constructor. Len is set to zero. Resize is expected to be called later.
    doubleN( );

    //! Constructor. No initialization of elements.
    explicit doubleN(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN(const int lenIn, const double var);

    //! Constructor. Initialize 3 long and with three double values.
    doubleN(const double x1, const double x2, const double x3);

    //! Constructor. Initialize 3 long and with a double3 value.
    explicit doubleN(const double3& x);


    //! Copy constructor.
    doubleN(const doubleN& var);

    //! Destructor.
    ~doubleN();

    //! Assignment operator.
    doubleN& operator=(const doubleN& var);

    //! Assignment operator. All elements get the value var.
    doubleN& operator=(const double var);

    //! Access element i of type double.
    inline double& operator()(const int i);

    //! Access element i of type double. Cannot be used as lvalue.
    inline double operator()(const int i) const;



    //! Gives the length of the dimension.
    int length() const { return len; };

    //! Resize the vector. No initialization of elements.
    void resize(const int lenIn);

    //! Resize the vector.
    //! If the new length is larger than the old one then var is used as init value for new elements.
    void resize(const int lenIn, const double var);

    //! Resize the vector. Use external data storage.
    //! Note, it can onlybe done when created with constructor external data storage,
    //! or default constructor has been used.
    void resize4ExtMem(const int lenIn, double* dataPtr);

    //! Get three first elements in the array. Assert len>=3.
    void Get3(double& x1, double& x2, double& x3) const {
        assert(len>=3);
        x1 = data[0];
        x2 = data[1];
        x3 = data[2];
    };

    //! Get a double3 for the three first elements in the array. Assert len>=3.
    double3 Get3() const;

    //! Get four first elements in the array. Assert len>=4.
    void Get4(double& x1, double& x2, double& x3, double& x4) const {
        assert(len>=4);
        x1 = data[0];
        x2 = data[1];
        x3 = data[2];
        x4 = data[3];
    };


    //! Set four first elements in the array. Assert len>=4.
    void Set4(const double x1, const double x2, const double x3, const double x4) {
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
    double* data;

    //! If True then the data pointer is pointing to external memory
    //! and no local memory is allocated.
    int XMFlg;

    //! Length of preallocated short array
    enum {SLEN=6};

    //! Preallocated data. By intntion and for speed it is not-initialized in constructors.
    double sdata[SLEN];
};


////////////////////////////////////////////////////////////////////////
//
// .SS Declaration of functions
//
// Here we declare global functions related to doubleN.
// For the description we use the notation:  "M" a doubleN
//

 //! M == M
bool operator==(const doubleN& a, const doubleN& b);


 // Inline definitions for doubleN

 inline double& doubleN::operator()(const int i)
 {
 #ifdef DEBUGFLG
     assert(i>0 && i<=len);
 #endif
     return (data[i-1]);
 }

 inline double doubleN::operator()(const int i) const
 {
 #ifdef DEBUGFLG
     assert(i>0 && i<=len);
 #endif
     return (data[i-1]);
 }



#endif // DOUBLEN_H
