
#ifndef ALANS_CTOKENIZER
#define ALANS_CTOKENIZER

#include "Bstring.h"

class Btokenizer
{
 public:
  //!Construct a tokenize to lex the string s
  explicit Btokenizer(const Bstring& s);

  //! Initialize with another string.
  //! The purpose is to reuse the same Btokenizer object
  //! when the string is consumed.
  void reInitialize(const Bstring& s);


  //!Advance to the next token and return it as a substring. 
  //!The tokens are soncidered to be deliminated by any of 
  //!the characters in s. 
  Bstring operator()(const char* s = "\t \n");

  //! Nothing left in the token anymore
  bool empty();
private:
    //! Copy constructor.
    //! Should never be used.
    Btokenizer(const Btokenizer & aBtokenizer_In);


    // FUNCTIONS: 
    //! = operator override.
    //! Should never be used.
    const Btokenizer & operator=(const Btokenizer & aBtokenizer_In);


private:
    size_t marker;
    Bstring currentString;
    size_t len;
    const char* data;
};
#endif
