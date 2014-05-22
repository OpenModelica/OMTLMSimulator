#ifndef _TOSTR_H_
#define _TOSTR_H_
#include "Bstring.h"

#include "portable_long.h"
// (for long64)

//! Convert int to string. All leading and trailing spaces are elliminated.
Bstring ToStr(const int val);

//! Convert int to string. All leading and trailing spaces are elliminated. Some compilers do not like to pass
//! integers to ToStr.
Bstring Int2Str(const int val);


//! Convert adress to string in hex. All leading and trailing spaces are elliminated.
Bstring ToStr(const void* val);

//! Convert int to string. All leading and trailing spaces are elliminated.
//! Set hexFlg to 1 for hexadezimal output, default is decimal.
Bstring ToStr(const long64 val, int hexFlg);

//! Convert double to string. All leading and trailing spaces are elliminated.
//! All trailing zeros are elliminated.
Bstring ToStr(const double val);

//! Convert int to string. All leading and trailing spaces are elliminated
//! except a space representing a plus sign.
Bstring ToStrS(const int val);

//! Convert double to string. All leading and trailing spaces are elliminated
//! except a space representing a plus sign.
//! All trailing zeros are elliminated.
Bstring ToStrS(const double val);


//! This is for variables that should be a multiple of pi.
//! Convert double to string. All leading and trailing spaces are elliminated.
//! All trailing zeros are elliminated.
Bstring ToStrA(const double val);

//! This is for variables that should be a multiple of pi.
//! Convert double to string. All leading and trailing spaces are elliminated
//! except a space representing a plus sign.
//! All trailing zeros are elliminated.
Bstring ToStrSA(const double val);

//! Convert double to string. All leading and trailing spaces are elliminated.
//! All trailing zeros are elliminated. Maximum 7 digits of output. 
//! This is one before dezimal point, fout after dezimal point and two for exponent. 
Bstring ToStr7(const double val);

//! Convert double to string. All leading and trailing spaces are elliminated.
//! All trailing zeros are elliminated. Maximum 6 digits of output.
//! This is one before dezimal point, three after dezimal point and two for exponent. 
Bstring ToStr6(const double val);

//! Convert double to string. All leading and trailing spaces are elliminated.
//! All trailing zeros are elliminated. Maximum 5 digits of output. 
//! This is one before dezimal point, two after dezimal point and two for exponent. 
Bstring ToStr5(const double val);

//! Convert double to string with given precision.
//! String is retuned with "p" number of significant digits.
//! Example: 0.001999 will be converted to string "0.002" with precicion 1.
Bstring ToStrP(const double val, int p=1);

//! Convert double to string. Hexadecimal representation exactly 16 characters.
Bstring ToStrHex(const double val);

//! Returns "true" or "false"
Bstring ToBoolStr(const bool val);












#endif
