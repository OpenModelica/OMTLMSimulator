#include <cassert>

#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Bstring.h"
#include "ErrorLog.h"
#include "tostr.h"
using namespace std;

#if _MSC_VER >= 1200

//MSC does not fullfill standard, as explained in
// http://www.josuttis.com/libbook/new.html

#define TOLOWER ::tolower
#define TOUPPER ::toupper

#else

#define TOLOWER std::tolower
#define TOUPPER std::toupper
#endif 



void Bstring::push_back_c(char c)
{
#if _MSC_VER == 1200
// MSC does not implement basic_string::push_back
      append(1,c);
#else 
      push_back(c);                   
#endif
}

int Bstring::compareTo(const char* str, caseCompare c) const
{

  
  if(c==ignoreCase){
      Bstring s1(str);
      Bstring s2(*this);

      transform(s1.begin(),s1.end(),s1.begin(), static_cast<int(*)(int)>(TOLOWER));
      transform(s2.begin(),s2.end(),s2.begin(), static_cast<int(*)(int)>(TOLOWER));
      
      return s1.compare(s2);
  }
  else{ // if(c==exact){
      return compare(str);
  }
  //return -1;
}

int Bstring::compareTo(const Bstring& str, caseCompare c) const
{
    if(c==ignoreCase){
        Bstring s1(str);
        Bstring s2(*this);

        transform(s1.begin(),s1.end(),s1.begin(), static_cast<int(*)(int)>(TOLOWER));
        transform(s2.begin(),s2.end(),s2.begin(), static_cast<int(*)(int)>(TOLOWER));
        
        return s1.compare(s2);
    }
    else{ //if(c==exact)
        return compare(str);
    }  
    //return -1;
}
 
const char* Bstring::data()const
{
    return c_str();
}

#if 0
Bstring Bstring::strip(stripType s, char c)
{
    Bstring s1(*this);

    if(s==leading && !empty() ){
        while(s1.operator[](0)==c){
            s1.erase(0,1);
	}
    }
    else if(s==trailing && !empty()){
        int len = s1.length();
        
        while(s1.operator[](len-1)==c){
            s1.erase(len-1,1);
            len=s1.length();
        }
    }
    else if(s==both && !empty()){
        //cout<<"s1 : "<<s1<<endl;
        while(s1.operator[](0)==c){
            s1.erase(0,1);
	}
        int len = s1.length();
        if(!empty()){
            while(s1.operator[](len-1)==c){
                s1.erase(len-1,1);
                len=s1.length();
            }
        }
    }

    assign(s1);

    return *this;
}

#else

Bstring Bstring::strip(stripType s, char c)
{

    if(s==leading && !empty() ){
      while ((length()>0) && (c_str()[0]==c)){
           erase(0,1);
	} 
    }
    else if(s==trailing && !empty()){
       int len = length();
       while ((len>0) && (c_str()[len-1]==c)){
            erase(len-1,1);
            len=length();
        }
    }
    else if(s==both && !empty()){
      //cout<<"s1 : "<<s1<<endl;
        while(c_str()[0]==c){
         erase(0,1);
	}
        int len = length();
        if(!empty()){
            while((len>0) && (c_str()[len-1]==c)){
                erase(len-1,1);
                len=length();
            }
        }
    }
    return *this;
}
#endif

void Bstring::toLower()
{
    transform(begin(),end(),begin(),static_cast<int(*)(int)>(TOLOWER));
}
void Bstring::toUpper()
{
   transform(begin(),end(),begin(),static_cast<int(*)(int)>(TOUPPER));  
}
/*
Bstring& Bstring::append(const char* cs)
{
  
     I wanted to use the append function in the class std::string.
     However this kept coming up with a segmentation error. This 
     code does the same thing but is less efficient. When the 
     segmentation error problem is fixed this part of the code should
     be returned to.
  
  int len=this->length();
  insert(len,cs);
  

  return *this;
}

Bstring& Bstring::append(const Bstring& cstr)
{
  int len=this->length();
  insert(len,cstr);
 
  return *this;
}

Bstring& Bstring::append(const char* cs, size_t N)
{
  size_type chars_len= N;
  
  int len=this->length();
  

  insert(len,cs,chars_len);
 
  
  return *this;
}

Bstring& Bstring::append(const Bstring& cs, size_t N)
{
  int len=this->length();
  size_type chars_len= N;
  
  insert(len,cs,chars_len);
  return *this;
}
*/

bool Bstring::contains(const char* str, caseCompare c)const
{
    if(c==ignoreCase){
        Bstring s2(*this);
        
        transform(s2.begin(),s2.end(),s2.begin(),static_cast<int(*)(int)>(TOLOWER));
        
        if(s2.find(str)!=Bstring::npos){
            return true;
        }
        return false;
    }
    
    if(find(str)!=Bstring::npos){
        return true;
    }
    
    return false;
}

bool Bstring::contains(const Bstring& str,caseCompare c)const
{
    return contains(str.c_str(),c);  
}

size_t Bstring::index(const char* pat, size_t i, caseCompare c )const
{
  Bstring pat2(pat);
  return index(pat,pat2.size(),i,c); 
}

size_t Bstring::index(const Bstring& pat, size_t i, caseCompare c)const
{
  return index(pat.c_str(),pat.size(),i,c); 
}

size_t Bstring::index(const Bstring& pat, size_t patlen, size_t i    , caseCompare c        )const
{
  return index(pat.c_str(),patlen,i,c);
}

size_t Bstring::index(const char* pat, size_t patlen, size_t i  , caseCompare c       )const
{
    if(c==ignoreCase){
      Bstring s2(*this);
     
      transform(s2.begin(),s2.end(),s2.begin(),static_cast<int(*)(int)>(TOLOWER));  
     
      return s2.find(pat,i,patlen); 
    }
    else {    
        return  find(pat,i,patlen);
    }
}

size_t Bstring::first(char c)const
{
    return find(c);
}

bool Bstring::isNull()const
{
    return empty();
}
bool Bstring::isAscii()const
{
    for(unsigned int i=0;i<size();i++){
        char c=(*this)(i);
        int d=(int) c;
        if(d>127){
            return false;
        }
    }
    return true;
}

Bstring& Bstring::prepend(const Bstring& str)
{
    int n=str.size();
    insert(0,str,n);
    return *this;
}
Bstring& Bstring::prepend(const Bstring& cstr,size_t N)
{
    if(N>cstr.size()){ 
        insert(0,cstr,cstr.size());
    }
    else { 
        insert(0,cstr,N);
    }

    return *this;
}

Bstring& Bstring::prepend(const char* cs)
{
   insert(0,cs);
   return *this;
}

Bstring& Bstring:: prepend(const char* cs, size_t N)
{
   insert(0,cs,N);
   return *this;
}

size_t Bstring::last(char c)const
{
  return rfind(c);
}

Bstring&  Bstring::remove(size_t pos)
{
    size_type buffer=pos;
    size_type len = size();

    len-=buffer;
    erase(buffer,len);

    return *this;

}

Bstring&  Bstring::remove(size_t pos, size_t lengthIn)
{
    //size_type buffer=pos;
    //size_type len=length;
 
    erase(pos, lengthIn);
  
    return *this;
}

void Bstring::resize(size_t n, char c)
{
    
    size_t sze=size();
    //cout<<"size :"<<sze<<endl;
    if(sze<n)
        {
            for(size_t i=0;i<(n-sze);i++) {
                     push_back_c(c);
                }
        }
    else if (sze>n)
        {
            Bstring buffer(*this);
            erase();
            replace(0,n,buffer,0,n);
            /*cout<<"within Bstring.cc "<<erase((sze-n),(sze-(sze-n)))<<endl;
            //cout<<"erase ("<<sze-(sze-n)<<","<<sze-n<<" "<<")"<<endl;
            //cout<<"within Bstring.cc "<<erase((sze-(sze-n),(sze-n)))<<endl;
            */
        }
}



istream&  Bstring::readFile(istream& s)
{
  erase();
 
  char c; 
  while(s.get(c)) {
      //Bstring buffer(c);
      //append(buffer);
      // append(c);
      push_back_c(c);
  }
 
  return s;
}
/*
In the RWC implementation readLine had 2 arguaments. However the 
2nd arguament defaulted to false. Nowhere in beast is this second
arguament used. Unfortuantely the std::istream doesn't provide 
a skip white space flag. So I haven't implemented this. I have 
placed a function which takes these 2 arguaments in the class 
definition. The single arguament and double arguament readLine
functions do exactly the same thing.
*/
istream& Bstring::readLine(istream& s, bool  )
{
    //! This implementation allos reading DOS files on UNIX as well.
   	std::string str;
    getline(s,str);
    if( str.length()>0 && str[str.length()-1]=='\r') str.erase(str.length()-1,1);
    *this=str;
    return s;
}




Bstring Bstring::operator() (size_t start,size_t len)const
{
  return substr((size_type)start,(size_type)len);
}

char Bstring::operator()(int i)const
{
  Bstring buf(*this);
  return buf.operator[](i);
  //return *this[i];
}


Bstring Bstring::replacePartOfString(const Bstring& replaceString, 
                                     const Bstring& insertString, 
                                     replaceWhat rWhat)
{
    size_t pos;
    pos=0;

    // We set max. iterations to 10000 to avoid endless while loops.
    size_t nIterations = ((rWhat==firstStr) ? 1 : 10000);

    while( find(replaceString,pos) != Bstring::npos && nIterations > 0 ){
        pos=find(replaceString,pos);
        replace(pos,replaceString.size(),insertString);            
        nIterations--;
        pos+=insertString.length();
    }

    if( rWhat == allStr && nIterations == 0 ){
        Error( " in Bstring::replacePartOfString(...): max. iterations reached, probably string missmatch." );
    }

    return *this;    
}


void Bstring::removeInitialCommonPart(const Bstring& s1, const Bstring& s2, Bstring& s1out, Bstring& s2out)
{
    //Takes in 2 strings and removes all characters at the start of both
    //strings that are common to both. For example if the 2 strings are
    //"abcefg" and "abcdef" this function should return "efg" and "def"
    //The altered strings are placed in s1out and s2out. 
    Bstring buffer1=s1;
    Bstring buffer2=s2;
   
    while((buffer1.operator[](0)==buffer2.operator[](0)) && !(buffer1.empty()))
        {
            buffer1.erase(0,1);
            buffer2.erase(0,1);
        }
    
    s1out=buffer1;
    s2out=buffer2;
       
}



 //! Check whether the suffix match   // BZ298
bool Bstring::hasSuffix(const Bstring& s1) const{
    size_t s1Len=s1.length();
    if (s1Len>length()) return false;
    return !compare(length()-s1Len, s1Len, s1);

}

//! Removes the suffix, otherwise asserts(false)  // BZ298
void Bstring::dropSuffix(const Bstring& s1){
    if(hasSuffix(s1)){
        size_t s1Len=s1.length();
        erase(length()-s1Len, s1Len);
    } else {


        Error("Internal error: The  function dropSuffix(\""+
              s1+
              "\") applied to \""+
              this->c_str()
              +"\". You must use hasSuffix() before this");
        assert(false); // cannot drop suffix which is missing
    }
 
}


//! Check whether the prefix match
bool Bstring::hasPrefix(const Bstring& s1) const{
   size_t s1Len=s1.length();
   if (s1Len>length()) return false;
   return !compare(0, s1Len, s1);

}

//! Removes the prefix, otherwise asserts(false)
void Bstring::dropPrefix(const Bstring& s1){
   if(hasPrefix(s1)){
       size_t s1Len=s1.length();
       erase(0, s1Len);
   } else {
       assert(false); // cannot drop suffix which is missing
   }

}

Bstring Bstring::padRight(size_t chars)
{
    if(chars > length()) this->append(chars-length(),' ');
    return *this;
}

Bstring Bstring::padLeft(size_t chars)
{
    if(chars > length()) this->insert(0,chars-length(),' ');
    return *this;
}

