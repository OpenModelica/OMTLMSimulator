#include "doubleN33s.h"



// Methods for class doubleN33s

doubleN33s::doubleN33s()
    : len(0), data(0)
{
}


doubleN33s::doubleN33s(const int lenIn)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double33s[len];
    assert(data!=0);
}


doubleN33s::doubleN33s(const int lenIn, const double var)
    : len(lenIn), data(0)
{
    assert(lenIn>0);
    data = new double33s[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN33s::doubleN33s(const doubleN33s& var)
    : len(var.len), data(0)
{
    // Is expensive, try to avoid using it!
    assert(false);

    data = new double33s[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }
}

doubleN33s::~doubleN33s()
{
    delete [] data;
}

void doubleN33s::Init(const int lenIn)
{
    len = lenIn;
    assert(len>0);
    delete data;
    data = new double33s[len];
    assert(data!=0);
}


void doubleN33s::Init(const int lenIn, const double var)
{
    len = lenIn;
    assert(len>0);
    data = new double33s[len];
    assert(data!=0);

    for (int i=0; i<len; i++) {
        data[i] = var;
    }
}

doubleN33s& doubleN33s::operator=(const doubleN33s& var)
{
    if (this == &var) return *this;

    assert(len==var.len);

    for (int i=0; i<len; i++) {
        data[i]   = var.data[i];
    }

    return *this;
}


doubleN33s& doubleN33s::operator=(const double var)
{

    for (int i=0; i<len; i++) {
        data[i]   = var;
    }

    return *this;
}
