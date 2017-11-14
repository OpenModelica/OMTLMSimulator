#ifndef _ERRORLOG_H
#define _ERRORLOG_H


#if 0

// REmoved, as there exist no reliable way to handle   new(i)
// operators from Qt4 library.
// In general one can probably try to place #include ErrorLog.h
// *after* #includes of Qt4 files.
// But it will not work really well with templates.

#if defined (_MSC_VER)
#ifdef DEBUGFLG
#define CRTDBG_MAP_ALLOC
#include <strstream>
#include "crtdbg.h"

#ifdef _DEBUG
   
   // This is used in _CrtMemDumpAllObjectsSince to see where all allocated
   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
   #define DEBUG_CLIENTBLOCK
#endif // _DEBUG
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

#endif
#endif
#endif



#include "Bstring.h"


#define Mog1(x) Log1(Bstring(__FILE__)+":"+Int2Str(__LINE__)+Bstring(" ")+x)


#ifdef USE_PRETTY_FUNCTION
#define _HERE_ Bstring(__FILE__)+":"+Int2Str(__LINE__)+":"+Bstring(__PRETTY_FUNCTION__)+":"+Bstring(" ")
#else
#define _HERE_ Bstring(__FILE__)+":"+Int2Str(__LINE__)+":"+Bstring(__FUNCTION__)+":"+Bstring(" ")
#endif

#ifdef DEBUG_BTRACEF
// Used for debugging. Must be off in any products, release tests, beacuse strings waste time
#define BTRACEF(x) Btracef(_HERE_+x)
#define BVALID(x) valid(x,_HERE_,false)
#else
// Should not waste time for evaluation of x, because string conversions is there.
#define BTRACEF(x)
#define BVALID(x)
#endif

// Prints name and value
#define BTRACEN(x) BTRACEF(#x+"="+ToStr(x))
#define BTRACEN2(x,y) BTRACEF(#x+("="+ToStr(x)+" ")+ #y+("="+ToStr(y) ))
#define BTRACEN3(x,y,z) BTRACEF(#x+("="+ToStr(x)+" ") +#y+ ("="+ToStr(y)  + " ")  +#z+  ("="+ToStr(z) ) )

#define BTRACEI(x) BTRACEF(#x+"="+Int2Str(x))
#define BTRACEI2(x,y) BTRACEF(#x+("="+Int2Str(x)+" ")+ #y+("="+Int2Str(y) ))
#define BTRACEI3(x,y,z) BTRACEF(#x+("="+Int2Str(x)+" ") +#y+ ("="+Int2Str(y)  + " ")  +#z+  ("="+Int2Str(z) ) )

#define BTRACES(x) BTRACEF(#x+"=\""+x+"\"")
#define BTRACES2(x,y) BTRACEF(#x+("=\""+x+"\" ")+ #y+("=\""+y+"\"" ))
#define BTRACES3(x,y,z) BTRACEF(#x+("=\""+x+"\" ") +#y+ ("=\""+y  + "\" ")  +#z+  ("=\""+z+"\"" ) )



//! Macro to print a NewMat object to the BTRACEF log.
#define NTRACEF(xf)\
    { std::ostrstream valConv;\
    valConv << xf;\
    Bstring valStr(valConv.str(),valConv.pcount());\
    BTRACEF(#xf+Bstring("=")+valStr);\
    }


//! Sets the error file name and optionally opens the file.
void SetErrorFileName(const Bstring& ErrorFileName, const int OpenItFlg=0, bool isAppend = false);

//! Close the error file.
void CloseErrorFile();

//! Flush the error file.
void FlushErrorFile();

//! Current size of the log file.
double logFileByteCounter();

//! Returns number of Error() messages after the last
//! clearErrorMonitorStatus() call.
int  getErrorMonitorStatus();

//! Sets  number of monitored messages to zero.
void clearErrorMonitorStatus();


#ifdef GCC3_IOSTREAMIO
//! Return the ref. to the ErrorFile.
ostream & GetErrorFile() ;

//! Set standard error stream used in Error function.
//! Returns current error stream, default is cerr.
ostream* SetStdError( ostream* n_stdErr );
#else
#include <cstdio>
//! Return a FILE * pointer
FILE *GetErrorFile() ;

//! Set standard error stream used in Error function.
//! Returns current error stream, default is stderr.
FILE *SetStdError( FILE* n_stdErr );
#endif

const Bstring prE  = "ERROR   : " ;
const Bstring prTr = "BTRACEF : " ;
const Bstring prW  = "WARNING : " ;
const Bstring prL1 = "LOG1    : " ;
const Bstring prL2 = "LOG2    : " ;

const Bstring nlE  = "\nERROR   : " ;
const Bstring nlTr = "\nBTRACEF : " ;
const Bstring nlW  = "\nWARNING : " ;
const Bstring nlL1 = "\nLOG1    : " ;
const Bstring nlL2 = "\nLOG2    : " ;

const Bstring & getMessagePrefix(int severity);

// Prefix Strings that are written on the oError file for error,
// warning, log1, and log2 messages respectively.
// Multithread safe.

//! Write error message on error file and on cerr.
//! An error is something that is not allowed. In most cases the
//! calculation terminated after an error have been found.
//! A correct calculation should have no error messages.
//! Multithread safe.
void Error(const Bstring& Message);

//! Write warning message on error file.
//! A warning is undesired behaviour that should be noticed by the user.
//! However, we can continue the calculation by means of some
//! default behaviour but the result might then be of little interest.
//! A correct calculation should have no warning messages.
//! Warnings do not appear in stderr during simulations.
//! Multithread safe.
void Warning(const Bstring& Message);

//! Write any merssage that should not contain any prefix at all.
//! It will appear in the log and in stderr.
//! Used mainly for Out2In that generates reports, and to avoid
//! any prefixes that makes no sense there.
void PrintInfo(const Bstring& Message);

//! Write log message of level 1 on error file.
//! A log message of level 1 is output that is always desired
//! but is not saved on other output files, e.g. *.out, *.log, 
//! for some reason. 
//! Multithread safe.
void Log1(const Bstring& Message);

//! Write log message of level 2 on error file.
//! A log message of level 2 is always turned off/on by a check 
//! outside the Log2() function call. 
//! The check "if (debugflg)" is used for extra output
//! in special running modes of production versions, e.g. single step, 
//! more contact load data, etc. 
//! The "ifdef DEBUGFLG" is used only by developers for extra debug information 
//! since that require that the program is recompiled to a special debug version
//! which is not distributed to users.
//! Multithread safe.
void Log2(const Bstring& Message);



//! Can be used for quick tracing problems when debugger is inconvenient.
//! Called from BTRACEF(Bstring)
//! Reliees on environment variables BTRACEF,BTRACEFLOG2
//! If BTRACEF set then writes using BtracefError()
//! If BTRACEFLOG2 set then writes using BtracefLog2()
//! These  environment variables are never on at production machines.
//! But normally  #ifdef DEBUGFLG should be placed around such calls
//! to avoid any time consuming evaluation of the argument.
void Btracef(const Bstring& Message);

//! Called from Btracef(Bstring)
//! If BTRACEF set then writes using BtracefError()
void BtracefError(const Bstring& Message);

//! Called from Btracef(Bstring)
//! If BTRACEFLOG2 set then writes using BtracefLog2()
void BtracefLog2(const Bstring& Message);


//! A function that should be declared as
//!   void myErrorReportingFunction(int severity,const Bstring & errorMsg)
//! is automatically called each time when Error, Log1,Log2,Warning are called.
void setMessageReportingFunction(void (myErrorReportingFunction)(int severity,const Bstring&  errorMsg));


//! For double value validation.
//! Any value outside 1e-200,1e+200 will cause a warning
//! nan will cause assert if we do not allow errors.
//! Best use is valid(77.77,_HERE_,false);
void valid(double x, const Bstring & fromwhere, bool allowErrors);

//! For double value validation, no string evaluation.
//!Any value outside 1e-200,1e+200 will cause false
//!isnan will cause false
bool isvalid(double x);

extern  int validity_warnings;

Bstring getTheErrorFileName();

#ifdef _MSC_VER
#ifdef HEAPCHECK
// Writes heap walker report to mem.dmp
size_t  heapdump( char * cmt );
#endif
#endif

#endif // !_ERRORLOG_H
