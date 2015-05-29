#ifndef ALANS_CSTRING
#define ALANS_CSTRING

//
// 	The following methods/operators are inherited from std::string and as such it
// 	wasnt' necessary to implement them. 
//	
// 	append(const char* cs);
// 	append(const char* cs, size_t N);
// 	append(const Bstring& cstr);
// 	append(const Bstring& cstr, size_t N);
//	
// 	insert(size_t pos, const char* cs);
// 	insert(size_t pos, const char* cs, size_t N);
// 	insert(size_t pos, const Bstring& str);
// 	insert(size_t pos, const Bstring& str, size_t N);
//	
// 	replace(size_t pos, size_t N, const Bstring& str)
// 	replace(size_t pos, size_t N1, const Bstring& str, size_t N2)
//	
// 	operator == (const Bstring&, const char* );
// 	operator == (const char*, const Bstring& );
// 	operator == (const Bstring&,const Bstring& );
//
// 	operator != (const Bstring&, const char* );
// 	operator != (const char*, const Bstring& );
// 	operator != (const Bstring&,const Bstring& );
//	
// 	operator < (const Bstring&, const char*)
// 	operator < (const char*, const Bstring& )
// 	operator < (const Bstring&,const Bstring& )
//
// 	operator > (const Bstring&, const char*)
// 	operator > (const char*, const Bstring& )
// 	operator > (const Bstring&,const Bstring& )
//
// 	operator >= (const Bstring&, const char*)
// 	operator >= (const char*, const Bstring& )
// 	operator >= (const Bstring&,const Bstring& )
//
// 	operator <= (const Bstring&, const char*)
// 	operator <= (const char*, const Bstring& )
// 	operator <= (const Bstring&,const Bstring& )
//
// 	operator + (const Bstring& , const Bstring&);
// 	operator + (const char* , const Bstring& )
// 	operator + (const Bstring& , const char*)
// 	
// ----------------------------------------------------
// 
// 	The Following methods/operators were never used in any of the Beast
// 	programs and as such weren't implemented. 
// 	binaryStoreSize() const //in any case this function is defunct when using std::string
// 	capacity()const; //ditto
// 	capacity(size_t capac);//ditto
//
// 	collate(const char* str) const;
// 	collate(const Bstring& str) const;
//	
// 	hash(caseCompare=exact)const;
//
// 	mbLength() const;
//
// 	readToken(istream& s);
//
// 	subString(const char* cs, size_t start =0, caseCompare=exact);
// 	subString(const char* cs,size_t start =0, caseCompare=exact)const;
//
// 	initialCapacity(size_t ic = 15);
// 	maxWaste(size_t mw = 15);
// 	resizeIncrement(size_t ri =16);
//
// 	operator<<(ostream&s, const Bstring&);
// 	operator>>(istream& s, const Bstring&);
//
// 	operator<<(RWvsotream&, const Bstring& str);
// 	operator<<(RWFile&, const Bstring& str);
//
// 	operator>>(RWvistream&, Bstring& str);
// 	operator>>(RWFile&,  Bstring& str);
//
// 	strXForm(const Bstring&);
// 	toLower(const Bstring& str);
// 	toUpper(const Bstring& str);



#include <cstring>
#include <string>
//#include <iostream>
//#include <fstream>

using std::string;

#if defined(_MSC_VER)
// It is a MSC warning C4100 about unused formal parameters.
// We often need virtual functions that have formal parameter names,
// for better documentaion,
// and we do not use these names in code.
// it applies to whole code of Beast project therefore it is placed here,
// not in portability.h
#pragma warning( disable : 4100)
#endif

// This is because there are too many virtual
// functions that have trivial body
// and we wanted their parameters be
// nicely displayed.
#ifdef WEXTRADIAGNOSTICS
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#define RWBoolean bool

class Bstring: public std::string{
    
public:
    
    
    //! This enumerator is used by the strip function. Leading = strip 1st 
    //! occurance. Trailing = last occurance. Both = Both.
    enum stripType{leading,trailing,both};
    
    //! Used in all comparison functions. States whether or not to takes the
    //! case (i.e. capitals or non-capitals) into account when comparing strings.
    enum caseCompare{exact,ignoreCase};

    //! Used by replace functions. Indecates whether to replace all occurences of 
    //! the string or only the first one.
    enum replaceWhat{firstStr,allStr};

    //
    // Only constructors take the inheritance parameters directly after them. 
    // the std::string class has analagous functions for all the constructors. 
    // These fucntions follow each constructor.

    
    
    //! The default constructor. 
    Bstring():string(){};
    
    //! This creates a string containing s. 
    Bstring(string s):string(s){};
    
    //! This creates a string containing cs. 
    //! If NULL is passed then empty string is used for initialization.
    Bstring(const char* cs):string( (cs!=NULL ? cs : "") ){};
    
    //! This creates a string containing cs. 
    //! If NULL is passed then empty string is used for initialization.
    Bstring(char * cs):string( (cs!=NULL ? cs : "") ){};

    //! This creates a string which contains only 1 character.
    //! But it is forbidden to convert automatically.
    explicit Bstring(char c):string(1,c){};
    
    //! Here the 2 parameters are switched around on conversion. 
    //! A type conversion to size_type  must also be made. 
    //! A string is created consisting of N copies of the input character. 
    Bstring(char c,size_t N):string((size_type)N,c){};
    
    //! A string is created using the first N characters of c. If N>c.length()
    //! then a segmentation fault will occur.   
    Bstring(char*c, size_t N):string(c,(size_type)N){};
    
    //! The copy constructor. 
    Bstring(const Bstring& str):string(str){};
    
    //! The destructor
    ~Bstring(){};

	
    
    //! Removes the first occurance of the char c. It removes it from the 
    //! front if stripType=Leading,  from the back if stripType = trailing
    //! and from both sides if stripType=both.
    //! strip returns modifies "this" and returns a copy of "this"
    Bstring strip( stripType s=trailing, char c=' ');
    
    //! Changes all upper-case letters in the string to lower-case. 
    void toLower();
    
    //! Changes all lower-case letters in the string to upper-case. 
    void toUpper();
    
    //! This compare function returns less than or greather than zero 
    //! depending when the  strings aren't equal or zero if they are. Case
    //! sensitivity is dictated by the the caseCompare arguament. 
    int compareTo(const char* str, caseCompare c=exact)const;
    
    //! This compare function returns less than or greather than zero 
    //! depending when the  strings aren't equal or zero if they are. Case
    //! sensitivity is dictated by the the caseCompare arguament. 
    int compareTo(const Bstring& str, caseCompare c=exact)const;
    
    
    const char* data()const;
    
    
    //! returns a string starting at index start and of length len. 
    Bstring operator() (size_t start,size_t len)const;
    
    //! returns the i'th character in the string. 
    char operator()(int i)const;
    
    //! This is used so Bstrings is automatically converted to const char*
    //! when a Bstring is sent to a function that normally takes a const char*.  
    //! It doesn't however work when we wish to add a string to a const char* and
    //! pass the result to a function that takes const char*. In such cases the 
    //! summation must be surrounded by brackest and followed by .c_str(). This may
    //! be because for safety reasons string doesn't have an automatic type conversion
    //! to const char*. 

    //! A problem detected  : BString a;ToStr(a) calls conversion to
    //!  const char* and then printing with ToStr(void*).
    operator  const char*() const{return c_str();}
      
    //! Pattern Maching.
    //! Starts with index i, searches for 1st occurance of pat in self.
    size_t index(const char* pat, size_t i=0,caseCompare c=exact)const;
    
    //! Pattern Maching.
    //! Starts with index i, searches for 1st occurance of pat in self.
    size_t index(const Bstring& pat,size_t i=0,caseCompare c=exact)const;
    
    //! Starts with index i, searches for 1st occurance of 1st patlen characters from
    //! pat in self. 
    size_t index(const char* pat, size_t patlen, size_t i, caseCompare c)const;

    //! Starts with index i, searches for 1st occurance of 1st patlen characters from
    //! pat in self. 
    size_t index(const Bstring& pat, size_t patlen, size_t i, caseCompare c)const;
    
    //! Returns true if str occurs in self. Case sensitivity according to caseCompare.
    bool contains(const char* str,caseCompare c=exact)const;
    
    //! Returns true if str occurs in self. Case sensitivity according to caseCompare.
    bool contains(const Bstring& str, caseCompare c=exact)const;
    
    //! Returns the index of the 1st occurance of the char c in self. 
    size_t first(char c)const;
  
    //! Returns the index of the last occurance of the char c in self. 
    size_t last(char c)const;
    
    //! returns true if the string is empty. 
    bool isNull()const;
    
    //! returns true if the MSB of all the chars in the string is set to 0
    bool isAscii()const;
 
    //! This is a mirror funcion of append. 
    Bstring& prepend(const Bstring& str);
    
    //! This is a mirror funcion of append.
    Bstring& prepend(const Bstring& cstr,size_t N);
    
    //! This is a mirror funcion of append. 
    Bstring& prepend(const char* cs);

    //! This is a mirror funcion of append. 
    Bstring& prepend(const char* cs, size_t N);
    
    //! This is replacement of push_back missing in some STL releases 
    void push_back_c(char c);
    
    
    //! removes the characters from the position pos to the end of the string. 
    Bstring& remove(size_t pos);
    
    //! removes length characters from position pos. 
    Bstring& remove(size_t pos, size_t length);
    
    //! Changes the length of self to n adding char c or truncating as necessary. 
    void resize(size_t n,char c =' ');
    
    //! Reads characters from the input stream s, preplacing the previous contents of self, until
    //! EOF is reached. Null characters are treated the same as other characters.
    std::istream& readFile(std::istream& s);

    //! Reads characters from the input stream s, replacing the previous contents of self, until
    //! a newline (or an EOF) is encountered. The newline is removed from the input stream but is 
    //! not stored. Null characters are treated the same as other characters.
    //! The 2nd argument (skipWS) is always ignored.
    std::istream& readLine(std::istream& s, bool skipWS=true);


    //! Reads characters from the input stream s, replacing the previous contents of self, until 
    //! an EOF or null terminator is encountered. If the number of bytes remaining in the stream 
    //! is large, you should resize the Bstring to approximately the number of bytes to be read
    //! prior to using this method. See "Implementation Details" in the User's Guide for more 
    //! information. This function is incompatible with strings with embedded nulls. This function 
    //! may be incompatible with MBCS strings.
    std::istream& readString(std::istream& s);


    //! Reads characters from the input stream s, replacing the previous contents of self, until
    //! an EOF or the delimiting character delim is encountered. The delimiter is removed from the
    //! input stream but is not stored. Null characters are treated the same as other characters.
    //! If delim is '\0' then this function is incompatible with strings with embedded nulls. 
    //! If delim is '\0' then this function may be incompatible with MBCS strings.
    std::istream& readToDelim(std::istream& s, char delim ='\n');


    //! This takes in the string inString, searches for the 1st occurancd of replaceString 
    //! in it and replaces the occurance of replaceString in inString with insertString. 
    //! It returns a copy of "this"
    Bstring replacePartOfString(const Bstring& replaceString, const Bstring& insertString, replaceWhat rWhat=firstStr);
    
    //! Takes in 2 strings and removes all characters at the start of both
    //! strings that are common to both. For example if the 2 strings are
    //! "abcefg" and "abcdef" this function should return "efg" and "def"
    //! The altered strings are placed in s1out and s2out. 
    void removeInitialCommonPart(const Bstring& s1, const Bstring& s2, Bstring& s1out, Bstring& s2out);

    //! Check whether the suffix match
    bool hasSuffix(const Bstring& s1) const;

    //! Removes the suffix, otherwise asserts(false)
    void dropSuffix(const Bstring& s1);

    //! Check whether the prefix match
    bool hasPrefix(const Bstring& s1) const;

    //! Removes the prefix, otherwise asserts(false)
    void dropPrefix(const Bstring& s1);

    //! Adds spaces to the right, to get so many characters totally, and do not remove any. Return this.
    Bstring padRight(size_t chars);

    //! Adds spaces to the left, to get so many characters totally, and do not remove any. Return this.
    Bstring padLeft(size_t chars);

private:
    
};

// BZ333
#endif
