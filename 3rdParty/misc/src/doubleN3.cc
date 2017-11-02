#include "doubleN3.h"

// Methods for class doubleN3

doubleN3::doubleN3()
    : len(0), data(0)
{
}


doubleN3::doubleN3(const int lenIn)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double3[len];
    assert(data!=0);
}


doubleN3::doubleN3(const int lenIn, const double var)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double3[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN3::doubleN3(const doubleN3& var)
    : len(var.len), data(0)
{
    // Is expensive, try to avoid using it!
    assert(false);

    data = new double3[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

doubleN3::~doubleN3()
{
    delete [] data;
}

void doubleN3::Init(const int lenIn)
{
    len = lenIn;
    assert(len>0);
    delete data;
    data = new double3[len];
    assert(data!=0);
}

void doubleN3::Init(const int lenIn, const double var)
{
    len = lenIn;
    delete data;
    data = new double3[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN3& doubleN3::operator=(const doubleN3& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


doubleN3& doubleN3::operator=(const double var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}



