#include "Bstring.h"
#include "Btokenizer.h"
#include <iostream> 
#include <cstring> 

Btokenizer::Btokenizer(const Bstring& s)
    : marker(0)
    , currentString(s)
    , len(currentString.size())
    , data(currentString.c_str())
{
}

void Btokenizer::reInitialize(const Bstring& s){
    marker=0;
    currentString=s;
    len=currentString.size();
    data=currentString.c_str();
}


#if 0
Bstring Btokenizer::operator()(const char* s)
{
  Bstring delims,stringOut;
  delims=s;
  
  for(int x=marker;x<currentString.length();x++)
    {
      for(int y=0;y<delims.length();y++)
	{
	  if((currentString.operator[](x))==(delims.operator[](y)))
	    {
	      marker++;
	      //cout<<"Comparing :"<<currentString[x]<<" and "<<delims[y]<<endl;
              if(marker != currentString.size()&&stringOut.isNull())
                  {stringOut=" ";
                  }
                      
	      return stringOut;
	    }
	}    
      // cout<<"Appending :"<<currentString[x]<<endl;
      stringOut.append(1,currentString.operator[](x));
      marker++;
	    
    }

  return stringOut;
}
#endif

#if 0
// New version for better performance.
// Alex

Bstring Btokenizer::operator()(const char* s)
{
    if( marker < currentString.size() ){
        //Bstring delims,stringOut;
        //Bstring delims(s);
        size_t p = 100000;
        int len = strlen(s);
        
        for(int y=0 ; y<len ; y++){
            int np = currentString.find( &s[y], marker, 1 );
            if( np < p ) p = np;
        }
        
        if( p == 100000 ) p = currentString.size();
        
        if( p < 100000 && p > 0 ){
            int m = marker;
            marker = p+1;
            return currentString.substr(m, p-m);
        }
    }
    return Bstring("");
}
#endif

// Yet another version with lowlevel string functions
Bstring Btokenizer::operator()(const char* s)
{
    if( data != 0 && marker < len ){
        marker += strspn(&data[marker], s); // Skip all leading characters
        size_t pos = strcspn(&data[marker], s); // Find token
        
        if( pos > 0 ){
            size_t m = marker;
            marker += pos;
            
            return currentString.substr(m, pos);
        }
    }

    return "";
}

bool Btokenizer::empty(){

    return data == 0 || marker >= len;
}
