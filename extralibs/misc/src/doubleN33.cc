#include "doubleN33.h"


// Methods for class doubleN33

doubleN33::doubleN33()
    : len(0), data(0)
{
}


doubleN33::doubleN33(const int lenIn)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double33[len];
    assert(data!=0);
}


doubleN33::doubleN33(const int lenIn, const double var)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double33[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN33::doubleN33(const doubleN33& var)
    : len(var.len), data(0)
{
    // Is expensive, try to avoid using it!
    assert(false);

    data = new double33[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

doubleN33::~doubleN33()
{
    delete [] data;
}

void doubleN33::Init(const int lenIn)
{
    len = lenIn;
    assert(len>0);
    delete data;
    data = new double33[len];
    assert(data!=0);
}


void doubleN33::Init(const int lenIn, const double var)
{
    len = lenIn;
    assert(len>0);
    data = new double33[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN33& doubleN33::operator=(const doubleN33& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


doubleN33& doubleN33::operator=(const double var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}





