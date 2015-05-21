#include "double3.h"
#include <ostream>
#include "Bstring.h"
#include "tostr.h"
//////////////////////////////////////////////////////////////
//
// .SS Constants
//
// These are the defined constants
//

const double3 zerodouble3 = double3(0.0);


const double3 Min(const double3& a, const double3& b)
{
    return double3(a.XX < b.XX ? a.XX : b.XX,
           a.YY < b.YY ? a.YY : b.YY,
           a.ZZ < b.ZZ ? a.ZZ : b.ZZ);
}

const double3 Max(const double3& a, const double3& b)
{
    return double3(a.XX > b.XX ? a.XX : b.XX,
           a.YY > b.YY ? a.YY : b.YY,
           a.ZZ > b.ZZ ? a.ZZ : b.ZZ);
}
const double3 fabs(const double3&a) //************************************************
{
    return double3(fabs(a.XX),fabs(a.YY),fabs(a.ZZ));
}
std::ostream& operator<<(std::ostream& o, const double3& v)
{
    o << "( ";
    o << v.XX << ' ' << v.YY << ' ' << v.ZZ << " )";
    return o;
}




Bstring ToStr(const double3& val)
{
    return Bstring("( " + ToStr(val.XX) + " " + ToStr(val.YY) + " " + ToStr(val.ZZ) + " )");
}



