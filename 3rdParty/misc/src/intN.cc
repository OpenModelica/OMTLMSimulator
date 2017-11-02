#include <cassert>

#include <cstring>
// For memset

#include "mathdefs.h"

#include "intN.h"

// Methods for class intN


intN::intN(const int lenIn)
    : len(lenIn), data(sdata)
{
    assert(lenIn>0);

    if (len>SLEN) {
        data = new int[len];
        assert(data!=NULL);
    }
}

intN::intN(const int lenIn, const int var)
    : len(lenIn), data(sdata)
{
    assert(lenIn>0);

    if (len>SLEN) {
        data = new int[len];
        assert(data!=NULL);
    }

    if (len>SLEN && var==0.0) {
        // Should speedup construction of big arrays.
        memset(data, 0, sizeof(int)*len);
    }
    else {
      for (int i=0; i<len; i++) {
          data[i] = var;
      }
    }

}


intN::intN(const intN& var)
    : len(var.len), data(sdata)
{
    if (len>SLEN) {
        data = new int[len];
        assert(data!=NULL);
    }

    for (int i=0; i<len; i++) {
        data[i] = var.data[i];
    }
}

intN::~intN()
{
    if (len>SLEN) {
        assert(sdata!=data);
        delete [] data;
    }
}



intN& intN::operator=(const intN& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i] = var.data[i];
    }

    return *this;
}


intN& intN::operator=(const int var)
{

    for (int i=0; i<len; i++) {
        data[i] = var;
    }

    return *this;
}


