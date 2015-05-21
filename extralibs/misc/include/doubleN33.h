#ifndef DOUBLEN33_H
#define DOUBLEN33_H
////////////////////////////////////////////////////////////////////////
//
//! \class doubleN33
//!
//!
//!  This class describes an array of double33.
//!
//!
//!
//!
//!

#include "lightmatConfig.h"

#include "double33.h"
// Needed for inlines

class double33;

class doubleN33 {

// Friend class.
friend class aPackBuf ;

public:

    //! Default constructor.
    doubleN33();

    //! Constructor.
    doubleN33(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN33(const int lenIn, const double var);

    //! Copy constructor.
    doubleN33(const doubleN33& var);

    //! Destructor.
    ~doubleN33();

    //! Initialization
    void Init(const int lenIn);

    //! Initialization
    void Init(const int lenIn, const double var);

    //! Assignment operator.
    doubleN33& operator=(const doubleN33& var);

    //! Assignment operator. All elements get the value var.
    doubleN33& operator=(const double var);

    //! Access element i1 of type double33.
    //! i1 in [1..N]
    inline double33& operator()(const int i1);

    //! Access element i1 of type double33. Cannot be used as lvalue.
    //! i1 in [1..N]
    inline const double33& operator()(const int i1) const;

    //! Access element [i1,i2]. Cannot be used as lvalue.
    //! Not so efficient implemented right now.
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline const double3 operator()(const int i1, const int i2) const;

    //! Access element [i1,i2,i3].
    //! i1 in [1..N] ,  i2 in [1,2,3] , and i3 in [1,2,3]
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline double& operator()(const int i1, const int i2, const int i3);

    //! Access element [i1,i2,i3]. Cannot be used as lvalue.
    //! i1 in [1..N] ,  i2 in [1,2,3] , and i3 in [1,2,3]
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline const double operator()(const int i1, const int i2, const int i3) const;

    //! Gives the length of the last dimension, i.e. i3.
    inline int length() const;

    //! Set the length of the first dimension, i.e. i1.
    inline void SetLength(const int lenIn);

private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointerto data area. Data index starts from 0.
    double33* data;


};


// Inline definitions for doubleN33

inline double33& doubleN33::operator()(const int i1)
{
#ifdef DEBUGFLG
    assert(i1>0 && i1<=len);
#endif
    return (data[i1-1]);
}

inline const double33& doubleN33::operator()(const int i1) const
{
#ifdef DEBUGFLG
    assert(i1>0 && i1<=len);
#endif
    return (data[i1-1]);
}

#if 0
//! Deactivated since not used and not most efficient. DF 2006-03-09
inline const double3 doubleN33::operator()(const int i1, const int i2) const
{
#ifdef DEBUGFLG
    assert(i2>0 && i2<=3);
    assert(i1>0 && i1<=len);
#endif
    return ( data[i1-1](i2) );
}


//! Deactivated since not used and not most efficient. DF 2006-03-09
inline double& doubleN33::operator()(const int i1, const int i2, const int i3)
{
#ifdef DEBUGFLG
    assert(i3>0 && i3<=3);
    assert(i2>0 && i2<=3);
    assert(i1>0 && i1<=len);
#endif
    return ( data[i1-1](i2,i3) );
}


//! Deactivated since not used and not most efficient. DF 2006-03-09
inline const double doubleN33::operator()(const int i1, const int i2, const int i3) const
{
#ifdef DEBUGFLG
    assert(i3>0 && i3<=3);
    assert(i2>0 && i2<=3);
    assert(i1>0 && i1<=len);
#endif
    return ( data[i1-1](i2,i3) );
}
#endif

inline int doubleN33::length() const
{
    return len;
}




#endif // DOUBLEN33_H
