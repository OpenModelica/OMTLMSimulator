#include "double2.h"
#include <ostream>
#include "Bstring.h"
#include "tostr.h"
//////////////////////////////////////////////////////////////
//
// .SS Constants
//
// These are the defined constants
//

const double2 zerodouble2 = double2(0.0);


const double2 Min(const double2& a, const double2& b)
{
    return double2(a.XX < b.XX ? a.XX : b.XX,
           a.YY < b.YY ? a.YY : b.YY);
}

const double2 Max(const double2& a, const double2& b)
{
    return double2(a.XX > b.XX ? a.XX : b.XX,
           a.YY > b.YY ? a.YY : b.YY);
}
const double2 fabs(const double2&a) //************************************************
{
    return double2(fabs(a.XX),fabs(a.YY));
}
std::ostream& operator<<(std::ostream& o, const double2& v)
{
    o << "( ";
    o << v.XX << ' ' << v.YY << " )";
    return o;
}




Bstring ToStr(const double2& val)
{
    return Bstring("( " + ToStr(val.XX) + " " + ToStr(val.YY)  + " )");
}

