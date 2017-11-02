#ifndef INTN_H
#define INTN_H
////////////////////////////////////////////////////////////////////////
//
//! \class intN
//!
//!
//!  This class describes an array of int.
//!
//!
//!
//!
//!

#include "lightmatConfig.h"

class intN {

// Friend class.
friend class aPackBuf ;


public:

    //! Constructor.
    explicit intN(const int lenIn);

    //! Constructor. Initialize with var.
    intN(const int lenIn, const int var);

    //! Copy constructor.
    intN(const intN& var);

    //! Destructor.
    ~intN();

    //! Assignment operator.
    intN& operator=(const intN& var);

    //! Assignment operator. All elements get the value var.
    intN& operator=(const int var);

    //! Access element i of type int.
    int& operator()(const int i);

    //! Access element i of type int. Cannot be used as lvalue.
    inline int operator()(const int i) const;

    //! Gives the length of the dimension.
    int length() const { return len; };

    //! Get three first elements in the array. Assert len>=3.
    void Get3(int& x1, int& x2, int& x3) const {
        assert(len>=3);
        x1 = data[0];
        x2 = data[1];
        x3 = data[2];
    };

    //! Get four first elements in the array. Assert len>=4.
    void Get4(int& x1, int& x2, int& x3, int& x4) const {
        assert(len>=4);
        x1 = data[0];
        x2 = data[1];
        x3 = data[2];
        x4 = data[3];
    };


    //! Set four first elements in the array. Assert len>=4.
    void Set4(const int x1, const int x2, const int x3, const int x4) {
        assert(len>=4);
        data[0] = x1;
        data[1] = x2;
        data[2] = x3;
        data[3] = x4;
    };



private:

    //! Number of components. Data index starts from 1.
    const int len;

    //! Pointer to data area. Data starts from index 0.
    int* data;

    //! Length of preallocated short array
    enum {SLEN=6};

    int sdata[SLEN];
};

// Inline definitions for intN

inline int& intN::operator()(const int i)
{
#ifdef DEBUGFLG
    assert(i>0 && i<=len);
#endif
    return (data[i-1]);
}


inline int intN::operator()(const int i) const
{
#ifdef DEBUGFLG
    assert(i>0 && i<=len);
#endif
    return (data[i-1]);
}


#endif // INTN_H
