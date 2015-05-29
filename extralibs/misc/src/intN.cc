#include <cassert>

#include <cstring>
// For memset

#include "intN.h"

// Methods for class intN

intN::intN(const int lenIn)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new int[len];
    assert(data!=0);
}


intN::intN(const int lenIn, const int var)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new int[len];
    assert(data!=0);

    if (var==0.0) {
        //should speedup construction of big arrays. // BZ516
    memset(data,0,sizeof(int)*len);
    }
    else
    {
       for (int i=0; i<len; i++) {
          data[i] = var;
       }
    }
}

intN::intN(const intN& var)
    : len(var.len), data(0)
{
    // Is expensive, try to avoid using it!
    assert(false);

    data = new int[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

intN::~intN()
{
    delete [] data;
}

intN& intN::operator=(const intN& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


intN& intN::operator=(const int var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}


