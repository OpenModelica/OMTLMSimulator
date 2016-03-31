#include "double3Vec.h"
#include <ostream>
#include <string>
#include "strConv.h"

//////////////////////////////////////////////////////////////
//
// .SS Constants
//
// These are the defined constants
//

namespace tlmMisc
{


const double3Vec zerodouble3Vec = double3Vec(0.0);


const double3Vec Min(const double3Vec& a, const double3Vec& b)
{
    return double3Vec(a.XX < b.XX ? a.XX : b.XX,
                      a.YY < b.YY ? a.YY : b.YY,
                      a.ZZ < b.ZZ ? a.ZZ : b.ZZ);
}

const double3Vec Max(const double3Vec& a, const double3Vec& b)
{
    return double3Vec(a.XX > b.XX ? a.XX : b.XX,
                      a.YY > b.YY ? a.YY : b.YY,
                      a.ZZ > b.ZZ ? a.ZZ : b.ZZ);
}

#if 0
const double3Vec fabs(const double3Vec&a) //************************************************
{
    return double3Vec(::fabs(a.XX), ::fabs(a.YY), ::fabs(a.ZZ));
}
#endif

std::ostream& operator<<(std::ostream& o, const double3Vec& v)
{
    o << "( ";
    o << v.XX << ' ' << v.YY << ' ' << v.ZZ << " )";
    return o;
}




std::string ToStr(const double3Vec& val)
{
    return std::string("( " + ToStr(val.XX) + " " + ToStr(val.YY) + " " + ToStr(val.ZZ) + " )");
}


}
