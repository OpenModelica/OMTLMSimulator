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
    intN(const int lenIn);

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
    inline int length() const;

private:

    //! Number of components. Data index starts from 1.
    const int len;

    //! Pointer to data area. Data starts from index 0.
    int* data;


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

inline int intN::length() const
{
    return len;
}

#endif // INTN_H
