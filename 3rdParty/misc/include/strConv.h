#ifndef _STRCONV_H_
#define _STRCONV_H_

#include <string>
#include <vector>


//! Convert int to string. All leading and trailing spaces are elliminated.
std::string ToStr(const int val);

//! Convert int to string. All leading and trailing spaces are elliminated.
std::string Int2Str(const int val);

//! Convert double to string. All leading and trailing spaces are elliminated.
//! All trailing zeros are elliminated.
std::string ToStr(const double val);

//! Convert double to string. All leading and trailing spaces are elliminated.
//! All trailing zeros are elliminated. Maximum 6 digits of output.
//! This is one before dezimal point, three after dezimal point and two for exponent. 
std::string ToStr6(const double val);

//! Convert double to string with given precision.
//! String is retuned with "p" number of significant digits.
//! Example: 0.001999 will be converted to string "0.002" with precicion 1.
std::string ToStrP(const double val, int p=1);

//! Returns "true" or "false"
std::string ToBoolStr(const bool val);

#endif
