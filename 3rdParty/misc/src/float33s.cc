#include "double33s.h"



Bstring ToStr(const float33s& val)
{
    Bstring res;
    res = "( " + ToStr(val.x11) + " " + ToStr(val.x22) + " " + ToStr(val.x33) + "  ";
    res+= "  " + ToStr(val.x12) + " " + ToStr(val.x23) + " " + ToStr(val.x13) + " )";
    return res;
}
