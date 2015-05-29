#include "double2.h"
#include "Bstring.h"
#include "tostr.h"

Bstring ToStr(const float2& val)
{
    return Bstring("( " + ToStr((double)val.XX) + " " + ToStr((double)val.YY)  + " )");
}
