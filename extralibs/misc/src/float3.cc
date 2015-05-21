#include "double3.h"
#include "Bstring.h"
#include "tostr.h"
Bstring ToStr(const float3& val)
{
    return Bstring("( " + ToStr(val.XX) + " " + ToStr(val.YY) + " " + ToStr(val.ZZ) + " )");
}
