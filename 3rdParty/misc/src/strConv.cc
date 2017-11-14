 
//#include <iostream>
#include "strConv.h"
#include <cstdio>
//#include "mathdefs.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>


std::string ToStr(const int val)
{
    // Convert to string without leading or trailing spaces.
    char buf[100];
    sprintf(buf,"%d", val);
    
    return (std::string(buf));

}

std::string Int2Str(const int val)
{
    // Convert to string without leading or trailing spaces.
    char buf[100];
    sprintf(buf,"%d", val);
    
    return (std::string(buf));

}

std::string ToStr(const double val)
{
    char buf[100] = {0};

    if (((val>999)||(val<-999))) {
        sprintf(buf,"%1.13e",val);
    } else if (val==0.0) {
        // To avoid "-0".
        return (std::string("0"));
    } else {
        sprintf(buf,"%1.13g",val);
    }

    return (std::string(buf));
}


std::string ToStr6(const double val)
{
    char buf[100];
    if ((val>9999)||(val<-9999)) {
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        sprintf(buf,"%1.3e", val);

        std::string ValStr = std::string(buf);
        return ValStr; //.strip(std::string::trailing, '0'); //BZ1088 #7-#11 Alex

    } else if (val==0.0) {
	// To avoid "-0".
        return (std::string("0"));

    } else {
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        sprintf(buf,"%1.3g", val);
        return (std::string(buf));
    }
}


std::string ToStrP(const double val, int p)
{
  std::stringstream valConv;

    // Value to string using given precicion.
    valConv << std::setprecision(p) << val;

    // NOTE: the .str() function of strstream does not
    //       work correctly on my compiler. Therefore I
    //       use this temporay string. Alex 10/11/29
    std::string valStr;
    valConv >> valStr;

    return valStr;
}

std::string ToBoolStr(const bool val){
    if (val) return "true";
    else return "false";
}

