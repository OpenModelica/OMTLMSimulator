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

class doubleN {

// Friend class.
friend class aPackBuf ;
//! M == M
friend bool operator==(const doubleN& a, const doubleN& b);

public:

    //! Empty constructor. Len is set to zero. Resize is expected to be called later.
    doubleN( );

    //! Constructor. No initialization of elements.
    doubleN(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN(const int lenIn, const double var);

    //doubleN(const int lenIn, double* dataPtr); Overloading problem.
    // Constructor. Initialize with external data storage.

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
    inline int length() const;

    //! Resize the vector. No initialization of elements.
    void resize(const int lenIn);

    //! Resize the vector.
    //! If the new length is larger than the old one then var is used as init value for new elements.
    void resize(const int lenIn, const double var);

    //! Resize the vector. Use external data storage.
    //! Note, it can onlybe done when created with constructor external data storage,
    //! or default constructor has been used.
    void resize4ExtMem(const int lenIn, double* dataPtr);

private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointer to data area. Data index starts from 0.
    double* data;

    //! If True then the data pointer is pointing to external memory
    //! and no local memory is allocated.
    int XMFlg;


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

 inline int doubleN::length() const
 {
     return len;
 }


#endif // DOUBLEN_H
