#include <cassert>

#include <cstring>
// For memset

#include "doubleN.h"




// Methods for class doubleN
doubleN::doubleN()
    : len(0), data(0), XMFlg(false)
{
}

doubleN::doubleN(const int lenIn)
    : len(lenIn), data(0), XMFlg(false)
{
    assert(lenIn>0);
    data = new double[len];
    assert(data!=0);
}

doubleN::doubleN(const int lenIn, const double var)
    : len(lenIn), data(0), XMFlg(false)
{
    assert(lenIn>0);
    data = new double[len];
    assert(data!=0);

    if (var==0.0) {
        //should speedup construction of big arrays. // BZ516
    memset(data,0,sizeof(double)*len);
    }
    else {

      for (int i=0; i<len; i++) {
        data[i] = var;
      }
    }

}


// doubleN::doubleN(const int lenIn, double* dataPtr)
// : len(lenIn), data(0), XMFlg(True)
// {
//     assert(lenIn>0);
//     assert(dataPtr!=0);
//     data = dataPtr;
// }

void doubleN::resize(const int lenIn)
{
    assert(!XMFlg);

    int oldLen = len;
    len = lenIn;
    assert(lenIn>0);
    double* oldData = data;
    data = new double[len];
    assert(data!=0);
    if(oldData != 0) {
    if(oldLen > len) oldLen = len;
    for(int i = 0; i < oldLen; i++)
        data[i]=oldData[i];
    delete [] oldData;
    }
}


void doubleN::resize(const int lenIn, const double var)
{
    assert(!XMFlg);

    int oldLen = len;
    len = lenIn;
    assert(lenIn>0);
    double* oldData = data;
    data = new double[len];
    assert(data!=0);

    if(oldData != 0) {
        assert(oldLen>0);
    if(oldLen > len) oldLen = len;
    for(int i = 0; i < oldLen; i++)
        data[i]=oldData[i];
    delete [] oldData;

        for(int ii = oldLen; ii < len; ii++)
            data[ii] = var;

    }
    else {
        for(int i = 0; i < len; i++)
            data[i] = var;
    }
}

void doubleN::resize4ExtMem(const int lenIn, double* dataPtr)
{
    assert(lenIn>0);
    assert(dataPtr!=0);

    if (XMFlg || (len==0 && data==0 && !XMFlg)) {
        // Only for default constructor or XMFlg=True
        len = lenIn;
        data = dataPtr;

        XMFlg = true;

    } else {
        assert(false);
    }
}



doubleN::doubleN(const doubleN& var)
    : len(var.len), data(0), XMFlg(false)
{
    // Is expensive, try to avoid using it!
    assert(false);

    data = new double[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

doubleN::~doubleN()
{
    if (!XMFlg) delete [] data;
}

doubleN& doubleN::operator=(const doubleN& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


doubleN& doubleN::operator=(const double var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}

// M == M
bool operator==(const doubleN& a, const doubleN& b)
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
