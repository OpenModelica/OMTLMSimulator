#include <cassert>

#include <cstring>
// For memset

#include "mathdefs.h"
#include "double3.h"

#include "realN.h"

#ifdef USING_LONG_DOUBLE

// Methods for class RealN

RealN::RealN()
    : len(0), data(sdata), XMFlg(false)
{
}

RealN::RealN(const int lenIn)
    : len(lenIn), data(sdata), XMFlg(false)
{
    assert(lenIn>0);

    if (len>SLEN) {
        data = new Real[len];
        assert(data!=NULL);
    }
}

RealN::RealN(const int lenIn, const Real var)
    : len(lenIn), data(sdata), XMFlg(false)
{
    assert(lenIn>0);

    if (len>SLEN) {
        data = new Real[len];
        assert(data!=NULL);
    }

    if (len>SLEN && var==0.0) {
        // Should speedup construction of big arrays.
        memset(data, 0, sizeof(Real)*len);
    }
    else {
      for (int i=0; i<len; i++) {
          data[i] = var;
      }
    }

}


RealN::RealN(const Real x1, const Real x2, const Real x3)
    : len(3), data(sdata), XMFlg(false)
{
    data[0] = x1;
    data[1] = x2;
    data[2] = x3;
}


RealN::RealN(const double3& x)
    : len(3), data(sdata), XMFlg(false)
{	
    data[ 0 ] = static_cast< Real >( x( 1 ) );
    data[ 1 ] = static_cast< Real >( x( 2 ) );
    data[ 2 ] = static_cast< Real >( x( 3 ) );
}

void RealN::resize(const int lenIn)
{
    assert(!XMFlg);
    assert(lenIn>0);

    int oldLen = len;
    len = lenIn;

    if (len<=SLEN && oldLen<=SLEN) {
        // Both new and old fit inside SLEN so we don't need to do anything.
    }
    else {
        // Either old and/or new exceed SLEN, thus the data is stored in different places.

        Real* oldData = data;

        if (len>SLEN) {
            data = new Real[len];
            assert(data!=NULL);
        }
        else {
            data = sdata;
        }

        // Copy old data
        int cplen = Min(oldLen, len);
        for (int i = 0; i < cplen; i++) data[i] = oldData[i];

        // The old storage was dynamically allocated, so it must be deleted.
        if (oldLen>SLEN) {
            assert(sdata!=oldData);
            delete [] oldData;
        }
        
    }
}


void RealN::resize(const int lenIn, const Real var)
{
    assert(!XMFlg);
    assert(lenIn>0);

    int oldLen = len;
    len = lenIn;

    if (len<=SLEN && oldLen<=SLEN) {
        // Both new and old fit inside SLEN.
        for (int ii = oldLen; ii < len; ii++) data[ii] = var;
    }
    else {
        // Either old or new exceed SLEN, thus the data is stored in different places.

        Real* oldData = data;

        if (len>SLEN) {
            data = new Real[len];
            assert(data!=NULL);
        }
        else {
            data = sdata;
        }

        // Copy old data
        int cplen = Min(oldLen, len);
        for (int i = 0; i < cplen; i++) data[i] = oldData[i];

        // Set new data to "var"
        for (int ii = oldLen; ii < len; ii++) data[ii] = var;

        // The old storage was dynamically allocated, so it must be deleted.
        if (oldLen>SLEN) {
            assert(sdata!=oldData);
            delete [] oldData;
        }
    }
}

void RealN::resize4ExtMem(const int lenIn, Real* dataPtr)
{
    assert(lenIn>0);
    assert(dataPtr!=NULL);

    if (XMFlg || (len==0 && data==sdata && !XMFlg)) {
        // Only for default constructor or XMFlg=True
        len = lenIn;
        data = dataPtr;
        XMFlg = true;
    } 
    else {
        assert(false);
    }
}



RealN::RealN(const RealN& var)
    : len(var.len), data(sdata), XMFlg(false)
{
    if (len>SLEN) {
        data = new Real[len];
        assert(data!=NULL);
    }

    for (int i=0; i<len; i++) {
        data[i] = var.data[i];
    }
}

RealN::~RealN()
{
    if (!XMFlg && len>SLEN) {
        assert(sdata!=data);
        delete [] data;
    }
}




RealN& RealN::operator=(const RealN& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


RealN& RealN::operator=(const Real var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}

//! Get a double3 for three first values in the array. Assert len>=3.
double3 RealN::Get3() const 
{
    assert(len>=3);

    return double3(data[0], data[1], data[2]);
}


// M == M
bool operator==(const RealN& a, const RealN& b)
{
    if (a.len!=b.len) return false;
    int equal=true;

    for (int i=0; i<a.len; i++) {
        if (a.data[i]!=b.data[i]) {
            equal=false;
            break;
        }
    }

    return equal;
}



#endif
