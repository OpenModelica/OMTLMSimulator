#ifndef DOUBLEN3_H
#define DOUBLEN3_H

#include "lightmatConfig.h"

#include "double3.h"
// Needed for inlines


////////////////////////////////////////////////////////////////////////
//
//! \class doubleN3
//!
//!
//!  This class describes an array of double3.
//!
//!
//!
//!
//!
class double3;

class doubleN3 {

// Friend class.
friend class aPackBuf ;

public:

    //! Default constructor.
    doubleN3();

    //! Constructor.
    doubleN3(const int lenIn);

    //! Constructor. Initialize with var.
    doubleN3(const int lenIn, const double var);

    //! Copy constructor.
    doubleN3(const doubleN3& var);

    //! Destructor.
    ~doubleN3();

    //! Initialization.
    void Init(const int lenIn);

    //! Initialization with var.
    void Init(const int lenIn, const double var);

    //! Assignment operator.
    doubleN3& operator=(const doubleN3& var);

    //! Assignment operator. All elements get the value var.
    doubleN3& operator=(const double var);

    //! Access element i1 of type double3.
    inline double3& operator()(const int i1);

    //! Access element i1 of type double3. Cannot be used as lvalue.
    inline const double3& operator()(const int i1) const;

    //! Access element [i1,i2].
    //! i1 in [1..N] and i2 in [1,2,3]
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline double& operator()(const int i1, const int i2);

    //! Access element [i1,i2]. Cannot be used as lvalue.
    //! i1 in [1..N] and i2 in [1,2,3]
    //! Deactivated since not used and not most efficient. DF 2006-03-09
    //inline const double operator()(const int i1, const int i2) const;

    //! Gives the length of the first dimension, i.e. i1.
    inline int length() const;

    //! Set the length of the first dimension, i.e. i1.
    inline void SetLength(const int lenIn);

private:

    //! Number of components. Index starts from 1.
    int len;

    //! Pointerto data area. Data index starts from 0.
    double3* data;


};


// Inline definitions for doubleN3

inline double3& doubleN3::operator()(const int i1)
{
#ifdef DEBUGFLG
    assert(i1>0 && i1<=len);
#endif
    return (data[i1-1]);
}

inline const double3& doubleN3::operator()(const int i1) const
{
#ifdef DEBUGFLG
    assert(i1>0 && i1<=len);
#endif
    return (data[i1-1]);
}

#if 0
//! Deactivated since not used and not most efficient. DF 2006-03-09
inline double& doubleN3::operator()(const int i1, const int i2)
{
#ifdef DEBUGFLG
    assert(i2>0 && i2<=3);
    assert(i1>0 && i1<=len);
#endif
    return ( data[i1-1](i2) );
}

//! Deactivated since not used and not most efficient. DF 2006-03-09
inline const double doubleN3::operator()(const int i1, const int i2) const
{
#ifdef DEBUGFLG
    assert(i2>0 && i2<=3);
    assert(i1>0 && i1<=len);
#endif
    return ( data[i1-1](i2) );
}
#endif

// inline void doubleN3::Pack(const PackType pdir, aPackBuf& bp)
// {
//     if (pdir==Down) {
//         bp << len ;
//     } else {
//         int len_tmp ;
//         bp >> len_tmp ;
//         assert(len==len_tmp);
//     }

//     for(int i=1; i<=len; i++)
// 	data[i-1].Pack(pdir, bp) ;
// }

inline int doubleN3::length() const
{
    return len;
}




#endif // DOUBLEN3_H
