 
#include <iostream>
#include "tostr.h"
#include <cstdio>
#include "mathdefs.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "portability.h"

bool extremePrecision = true;
// For -1.264911064034566731234e+7
// you must have extreme precision, otherwise conversion
// to ToStr abd back gives different results.

Bstring ToStr(const int val)
{
    // Convert to string without leading or trailing spaces.
    char buf[100];
    sprintf(buf,"%d", val);
    
    return (Bstring(buf));

}

Bstring Int2Str(const int val)
{
    // Convert to string without leading or trailing spaces.
    char buf[100];
    sprintf(buf,"%d", val);
    
    return (Bstring(buf));

}

Bstring Pointer2Str(const void* val)
{
    // Convert to string without leading or trailing spaces.
    char buf[100];
    sprintf(buf,"%p", val);// was %x
    
    return (Bstring(buf));

}

Bstring ToStr(const long64 x, int hexFlg)// Must be two arguments to make sure it is not mixed with  ToStr(const int val).
{
    std::ostrstream s;
    if (hexFlg != 0) { // use hexadezimal output
	
#ifdef _MSC_VER
	//  cerr << hex << x;
	s << std::hex << std::setfill('0') << std::setw(8) << (long)((x>>32)) <<
	    std::setfill('0') << std::setw(8) << (long) (((x<<32)>>32)) << std::dec ;
#else
	s << std::hex << x << std::dec;
#endif
	
    }
    else {// use dezimal output. IoStream library normally supports it directly.
	
#ifdef _MSC_VER
	s << std::dec << x;
#else
	s << std::dec << x;
#endif	
    }
    
    return Bstring(s.str(), s.pcount());
}

Bstring ToStr(const double val)
{
    char buf[100];

    if (((val>999)||(val<-999))&& finite(val)) {
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        
        
        //Experiments with a 10e6 random numbers show that 
        //using "e" you need  %1.16e in most cases, but %1.18e for numbers <-1000
        //using "g" you need  %1.17g  
        //to unambiguously represent a "double"
        //Tested on g++ and Visual C++.
        
        
        
        sprintf(buf,"%1.13e",val);
        char *  resdouble=NULL;
        if(strtod(buf,&resdouble)!=val){
            // This precision is not sufficient
            sprintf(buf,(extremePrecision?"%1.16e":"%1.15e"), val);
        }


        Bstring ValStr = Bstring(buf);

        size_t len = ValStr.length();
        size_t epos =ValStr.index("e");
        if(epos!= Bstring::npos){
            // e found
            Bstring man = ValStr(0,epos);
            Bstring tmp= ValStr(epos,len-epos);
            size_t p;
            if ((p=tmp.index("e+00"))!= Bstring::npos) tmp.erase(p+2,2);
            else if ((p=tmp.index("e+0"))!= Bstring::npos) tmp.erase(p+2,1);
            return (man.strip(Bstring::trailing, '0') + tmp);
        }else{
            return (ValStr.strip(Bstring::trailing, '0'));
        }



    } else if (val==0.0) {
        // To avoid "-0".
        return (Bstring("0"));

    } else {
        // Here comes "nan" and "inf" as well.
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.

        sprintf(buf,"%1.13g",val);
        char *  resdouble=NULL;
        if(strtod(buf,&resdouble)!=val){
            // This precision is not sufficient

            sprintf(buf,(extremePrecision?"%1.17g":"%1.15g"), val);
        }

        Bstring ValStr = Bstring(buf);

        size_t len = ValStr.length();
        size_t epos =ValStr.index("e");
        if(epos!= Bstring::npos){
            // e found
            Bstring man = ValStr(0,epos);
            Bstring tmp= ValStr(epos,len-epos);
            size_t p;
            if ((p=tmp.index("e-00"))!= Bstring::npos) tmp.erase(p+2,2);
            else if ((p=tmp.index("e-0"))!= Bstring::npos) tmp.erase(p+2,1);
            return (man.strip(Bstring::trailing, '0') + tmp);
        }else{
            return ValStr;
        }
    }
}

Bstring ToStr(const std::vector<double> & vals) {

    Bstring res = "[";
    
    for(unsigned int i=0; i < vals.size(); i++) {
        res += " " + ToStr(vals.at(i));
        if(i<vals.size()-1)
            res += ",";
    }
    res += "]";
    return res;
}

//#ifdef DEBUGFLG
struct TwoIntForPrint {
    int high;
    int low;
};
Bstring ToStrHex(const double val)
{
    char buf[100];
    TwoIntForPrint* valp = (TwoIntForPrint*)(void*)&val;

#ifdef _BIG_ENDIAN
    sprintf(buf,"%0X%0X", valp->high, valp->low );
#else
 #ifdef _LITTLE_ENDIAN
    sprintf(buf,"%0X%0X", valp->low , valp->high);
 #else
   #error "No endian"
 #endif
#endif

    return (Bstring(buf));
}
//#endif





Bstring ToStrSA(const double val)
{
    double val2 = fabs(val/pi);
    Bstring ppp;
    if (Sign(val)==1) {
        ppp =" pi";
    } else {
        ppp ="-pi";
    }

    if (val2<1.0) {

        if (fabs(val2 - 1.0/2) < 1.0e-16) {
            return (ppp + Bstring("/2"));
        }

        if (fabs(val2 - 1.0/3)  < 1.0e-16) {
            return (ppp +  Bstring("/3"));
        }

        if (fabs(val2 - 1.0/4)  < 1.0e-16) {
            return (ppp +  Bstring("/4"));
        }

        return (ppp + Bstring("*") + ToStr(val2));

    } else {
        return (ppp + Bstring("*") + ToStr(val2));

    }


}

Bstring ToStrA(const double val)
{
    double val2 = fabs(val/pi);
    Bstring ppp;
    if (Sign(val)==1) {
        ppp ="pi";
    } else {
        ppp ="-pi";
    }

    if (val2<1.0) {

        if (fabs(val2 - 1.0/2) < 1.0e-16) {
            return (ppp + "/2");
        }

        if (fabs(val2 - 1.0/3)  < 1.0e-16) {
            return (ppp + "/3");
        }

        if (fabs(val2 - 1.0/4)  < 1.0e-16) {
            return (ppp + "/4");
        }

        return (ppp + "*" + ToStr(val2));

    } else {
        return (ppp + "*" + ToStr(val2));

    }


}

Bstring ToStr7(const double val)
{
    char buf[100];
    if ((val>9999)||(val<-9999)){
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        sprintf(buf,"%1.4e", val);
        Bstring ValStr = Bstring(buf);




        size_t epos = (4+2);//Exactly 2 more above.
        size_t len = ValStr.length();
        Bstring man = ValStr(0,epos-1);
        Bstring tmp= ValStr(epos,len-epos);
        size_t p;
        if ((p=tmp.index("e+00"))!= Bstring::npos) tmp.erase(p+2,2);
        else if ((p=tmp.index("e+0"))!= Bstring::npos) tmp.erase(p+2,1);
        return (man.strip(Bstring::trailing, '0') + tmp);





    } else if (val==0.0) {
	// To avoid "-0".
        return (Bstring("0"));

    } else {
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        sprintf(buf,"%1.4g", val);

        Bstring tmp=buf;
        size_t p;
        if ((p=tmp.index("e-00"))!= Bstring::npos) tmp.erase(p+2,2);
        else if ((p=tmp.index("e-0"))!= Bstring::npos) tmp.erase(p+2,1);
        return (tmp);
    }
}

Bstring ToStr6(const double val)
{
    char buf[100];
    if ((val>9999)||(val<-9999)) {
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        sprintf(buf,"%1.3e", val);

        Bstring ValStr = Bstring(buf);
        return ValStr; //.strip(Bstring::trailing, '0'); //BZ1088 #7-#11 Alex

    } else if (val==0.0) {
	// To avoid "-0".
        return (Bstring("0"));

    } else {
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        sprintf(buf,"%1.3g", val);
        return (Bstring(buf));
    }
}

Bstring ToStr5(const double val)
{
    char buf[100];
    if ((val>9999)||(val<-9999)) {
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        sprintf(buf,"%1.2e", val);

        Bstring ValStr = Bstring(buf);
        size_t p;
        if ((p=ValStr.index("e+00"))!= Bstring::npos) ValStr.erase(p+2,2);
        else if ((p=ValStr.index("e+0"))!= Bstring::npos) ValStr.erase(p+2,1);
        return ValStr; // Do not remove trailing zeroes,
        // as numbers like 1e+10  truncate their last zero as well.
        //  .strip(Bstring::trailing, '0');

    } else if (val==0.0) {
        // To avoid "-0".
        return (Bstring("0"));

    } else {
        // Convert to string without leading or trailing spaces.
        // Also elliminate trailing zeros.
        sprintf(buf,"%1.2g", val);
        Bstring ValStr = Bstring(buf);
        size_t p;
        if ((p=ValStr.index("e-00"))!= Bstring::npos) ValStr.erase(p+2,2);
        else if ((p=ValStr.index("e-0"))!= Bstring::npos) ValStr.erase(p+2,1);
        return (ValStr);
    }
}

Bstring ToStrP(const double val, int p)
{
    std::strstream valConv;

    // Value to string using given precicion.
    valConv << std::setprecision(p) << val;

    // NOTE: the .str() function of strstream does not
    //       work correctly on my compiler. Therefore I
    //       use this temporay string. Alex 10/11/29
    Bstring valStr;
    valConv >> valStr;

    return valStr;
}

Bstring ToBoolStr(const bool val){
    if (val) return "true";
    else return "false";
}



void setExtremePrecision(const bool val){
//! In case it is needed for debugging of computer arithmetics
//! it is possible to set a extremely high precision.
//! so that ToStr(double) uses so many digits
//! that any different double values are printed differently 
  extremePrecision=val;
}



