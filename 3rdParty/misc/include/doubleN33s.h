#ifndef DOUBLEN33S_H
#define DOUBLEN33S_H

////////////////////////////////////////////////////////////////////////
//
//! \class doubleN33s
//!
//!
//!  This class describes an array of double33s.
//!
//!
//!
//!
//!
class double33s;

class doubleN33s {

// Friend class.
friend class aPackBuf ;

public:

    //! Default constructor.
    doubleN33s();

    //! Constructor.
    explicit doubleN33s(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN33s(const int lenIn, const double var);

    //! Copy constructor.
    doubleN33s(const doubleN33s& var);

    //! Destructor.
    ~doubleN33s();

    //! Initialization
    void Init(const int lenIn);

    //! Initialization
    void Init(const int lenIn, const double var);

    //! Assignment operator.
    doubleN33s& operator=(const doubleN33s& var);

    //! Assignment operator. All elements get the value var.
    doubleN33s& operator=(const double var);

    //! Access element i1 of type double33s.
    //! i1 in [1..N]
    inline double33s& operator()(const int i1);

    //! Access element i1 of type double33s. Cannot be used as lvalue.
    //! i1 in [1..N]
    inline const double33s& operator()(const int i1) const;

    //! Gives the length of the last dimension, i.e. i3.
    inline int length() const;

    //! Set the length of the first dimension, i.e. i1.
    inline void SetLength(const int lenIn);

private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointerto data area. Data index starts from 0.
    double33s* data;


};

#include "double33s.h"

// Inline definitions for doubleN33s

inline double33s& doubleN33s::operator()(const int i1)
{
#ifdef DEBUGFLG
    assert(i1>0 && i1<=len);
#endif
    return (data[i1-1]);
}

inline const double33s& doubleN33s::operator()(const int i1) const
{
#ifdef DEBUGFLG
    assert(i1>0 && i1<=len);
#endif
    return (data[i1-1]);
}

inline int doubleN33s::length() const
{
    return len;
}


#endif // DOUBLEN33S_H
