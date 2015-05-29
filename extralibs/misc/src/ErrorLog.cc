#ifdef MULTITHREAD
#include <thread.h>
#include <synch.h>
#include <unistd.h>
#include <cerrno>


//! Locking of error file.
extern mutex_t ErrorFileLock;
#endif 


#include <cstdlib>
 
 
#include <iostream>
#include <fstream>
#include <cassert>

using std::cout;
using std::cerr;
using std::endl;
using std::ios;

#include <cmath>
// for isnan

#include "ErrorLog.h"
#include "tostr.h"
#include "portable_isnan.h"
#include <ctime>
#if !defined(_MSC_VER)
#include <sys/time.h>
#endif

#ifdef __PARIX__     // npnfix the $#@@ PARIX writes close(-1) on stderr
#include <unistd.h>  // annoying, when destruckting local ifstream
#include <cerrno>   

extern "C" {
    int OS_close(int);
    
    int close(int fd)
	{
	    if (fd >= 0)
		return OS_close(fd);
	    errno = EBADF;
	    return -1;
	}    
}
#endif

static long long byteCounter=0;

// nr of gigabytes in the log file now.
double  logFileByteCounter(){
    return byteCounter / 1.0e9;
}


#ifdef MULTITHREAD 
mutex_t ErrorFileLock;
// Locking of error file.
#endif

#ifdef HEAPCHECK
int  heapdump( char * cmt );
#endif

#if defined (_MSC_VER)
#define WINDOWSDEBUGGER
#endif

#ifdef WINDOWSDEBUGGER
#include "windows.h"
#endif



// Some explanation regarding the GCC3_IOSTREAMIO.
//  We use it for big files (which may become >2GB)  
//  GCC3_IOSTREAMIO is OFF (normal, default)
//    Uses "FILE*" for input/output of big files    
//  GCC3_IOSTREAMIO is ON
//    Uses "ofstream/ifstream " for input/output of big files 

// Currently  GCC3_IOSTREAMIO is nerver tuned on anywhere.
// #ifdef  regarding  GCC3_IOSTREAMIO was introduced by Hakan 2004-06-17.
// He changed all surface-data and oud-data writing and reading to
// C routines (FILE-based) instead of iostream (fstream-based).
// You need to #define GCC3_IOSTREAMIO to turn back to iostream rotuines.
// On 2006-04-28 noone uses this flag GCC3_IOSTREAMIO. 
// According to Alexander Siemers there was a problem with some GCC version
// that were not able to support fstream-based interaction.
// It could probably be related to 2GB limit.

// GCC on Linux /Europa
//sizeof long long: 8, sizeof off_t: 4
//sizeof  pos_type: 12
//sizeof  off_type: 4

// GCC on Linux/Gonzo (gcc 3.4.4)
//sizeof long long: 8, sizeof off_t: 4
//sizeof  pos_type: 16
//sizeof  off_type: 8

// GCC on CygWin 
//sizeof long long: 8, sizeof off_t: 8
//sizeof  pos_type: 16
//sizeof  off_type: 8

// With GCC one can use FILE, fseeko, ftello, off_t (these are used today)
// With GCC one can use fstream, tellg(),seekg() pos_type off_type streampos streamoff


// VC++ 7.1 (.net 2003)
//sizeof long long: 8, sizeof off_t: 4
//sizeof  pos_type: 24
//sizeof  off_type: 4

// With VC++ one can use FILE, fseek, ftell, long (i.e. always under 2GB limit)
// With VC++ one can use fstream, tellg(),seekg() pos_type off_type streampos streamoff


#ifdef GCC3_IOSTREAMIO

static ofstream oError;
static ostream* stdErr = &cerr;
static int oError_OpenFlg = 0;
static int oError_NameDefined = 0;
static Bstring TheErrorFileName = "";


int IsOpenQ()
// Local function for opening the file.
{  
    if (oError_OpenFlg) {
	return 1;

    } else if (oError_NameDefined) {
	oError.open(TheErrorFileName, ios::out );

	if ( ! oError ) {	    
	    oError.open( ("/tmp/" + TheErrorFileName).c_str() );

	    if ( !oError ) {	    
		char *home = getenv("HOME");

		oError.open( (Bstring( home ) + "/" + TheErrorFileName).c_str() );

		if ( !oError ) {	    
		    cerr << "\nCouldn't open error file " << TheErrorFileName << "!\n" << endl;
		    exit(-1);
		}
		else
		    cerr << "Opened error log at: " << getenv("HOME") 
			 << "/" << TheErrorFileName << endl;
	    }
	    else {
		cerr << "Opened error log at: /tmp/" << TheErrorFileName << endl;
            }
		
	}

        assert(oError.good());

	oError_OpenFlg = 1;
	byteCounter=0;
	oError.precision(17);
	oError.setf( ios::scientific, ios::floatfield );

      	return 1;

    } else {
	return 0;
    }
}


void SetErrorFileName(const Bstring& ErrorFileName, const int OpenItFlg)
{  
#ifdef MULTITHREAD
    if (oError_OpenFlg==0 &&  oError_NameDefined==0) {
	mutex_init(&ErrorFileLock, USYNC_THREAD, (void *)NULL);
	// I am unsure if we can do mutex_init several times.
	// So we check for now that it is the first time.
    }
#endif 

    if (oError_OpenFlg) CloseErrorFile();

    if (ErrorFileName=="") {
        cerr << "\nNo error file name!\n" << endl;
        exit(-1);
    }

    TheErrorFileName = ErrorFileName;
    oError_NameDefined = 1;

    cout.precision(17);
    cout.setf( ios::scientific, ios::floatfield );

    cerr.precision(17);
    cerr.setf( ios::scientific, ios::floatfield );

    if (OpenItFlg) IsOpenQ();
}




void CloseErrorFile()
{  
    if (oError_OpenFlg) {

#ifdef MULTITHREAD
	mutex_lock(&ErrorFileLock) ;
#endif
	oError.close();
#ifdef MULTITHREAD
	mutex_unlock(&ErrorFileLock) ;
#endif

	oError_OpenFlg = 0;
    }
}

void FlushErrorFile()
{  

    if (oError_OpenFlg) {

#ifdef MULTITHREAD
	mutex_lock(&ErrorFileLock) ;
#endif
	oError.flush();
#ifdef MULTITHREAD
	mutex_unlock(&ErrorFileLock) ;
#endif
    }
}


// No difference between error and warning functions right now. Might be
// introduced later.



ostream & GetErrorFile()
{
    return oError ;
}

ostream* SetStdError( ostream* n_stdErr )
{

    assert(n_stdErr != 0);
    assert(n_stdErr->good());

    ostream *os = stdErr;
    stdErr = n_stdErr;

    return os;
}

#else
static FILE *oError;
static FILE *stdErr = stderr;
static FILE *stdOut = stdout;
static int oError_OpenFlg = 0;
static int oError_NameDefined = 0;
static Bstring TheErrorFileName = "";


#ifdef _MSC_VER
FILE   * heapdumpfile;
size_t  heapdump( char * cmt )
{  
    if(_heapchk()!=_HEAPOK) 
        if (cmt) cerr << "HEAPNOK:  " << cmt <<endl;
    assert(_heapchk()==_HEAPOK);

    if (cmt) {
        heapdumpfile=fopen("mem.dmp","a+t");
        if (!heapdumpfile) assert(0);
        fprintf(heapdumpfile,"DUMPSTART %s \n",cmt);
    }
	_HEAPINFO hinfo;
   int heapstatus;
   int idx=0;
   size_t totalmem=0;
   size_t totalusedmem=0;
   hinfo._pentry = NULL;
   
   while( ( heapstatus = _heapwalk( &hinfo ) ) == _HEAPOK )
   {
	idx++;
	if (hinfo._useflag == _USEDENTRY) totalusedmem+=hinfo._size ;
	totalmem+=hinfo._size ; 
	// %Fp
	int bad=0;
	if (hinfo._size>20000000) bad=1;
    
    if (cmt) {
    // Bstring type_string_b= typeid(*this).name() ;
    const char* tp=typeid(*(hinfo._pentry)).name();
   	fprintf(heapdumpfile, "%d: %6s block at %p of size %d BAD=%d %s\n", idx,
        ( hinfo._useflag == _USEDENTRY ? "USED" : "FREE" ),
          hinfo._pentry, hinfo._size,bad,tp );
    fflush(heapdumpfile);
    }

	assert(hinfo._size<20000000);
   }
   if (!cmt) return totalusedmem;
   if (cmt) {
       switch( heapstatus )
       {
       case _HEAPEMPTY:
           fprintf(heapdumpfile, "OK - empty heap\n" );
           break;
       case _HEAPEND:
           fprintf(heapdumpfile, "OK - end of heap\n" );
           break;
       case _HEAPBADPTR:
           fprintf(heapdumpfile, "ERROR - bad pointer to heap\n" );
           break;
       case _HEAPBADBEGIN:
           fprintf(heapdumpfile, "ERROR - bad start of heap\n" );
           break;
       case _HEAPBADNODE:
           fprintf(heapdumpfile, "ERROR - bad node in heap\n" );
           break;
       }

       fprintf(heapdumpfile,"Total %d blocks, size %d, used %d\n\n",
           idx, totalmem, totalusedmem);
       fclose(heapdumpfile);
   }
   return totalusedmem;
}
#endif


int IsOpenQ()
// Local function for opening the file.
{  
    if (oError_OpenFlg) {
	return 1;

    } else if (oError_NameDefined) {
        oError = fopen(TheErrorFileName, "w");

	if (oError == NULL) {	    
            oError = fopen(("/tmp/" +TheErrorFileName).c_str(), "w");

	    if (oError == NULL) {	    
		char *home = getenv("HOME");

		oError = fopen((Bstring( home ) + "/" + TheErrorFileName).c_str(), "w");

		if (oError == NULL) {	    
		    fprintf(stderr,  ("\nCouldn't open error file "  + TheErrorFileName +"!\n").c_str());
		    exit(-1);
		} else {
                    fprintf(stderr, "Opened error log at: %s/%s\n", getenv("HOME"), TheErrorFileName.c_str());
                }
	    } else {
		cerr << "Opened error log at: /tmp/" << TheErrorFileName << endl;
            }
	}
        
        assert(!ferror(oError));

	oError_OpenFlg = 1;
        byteCounter = 0; 
       
	return 1;

    } else {
	return 0;
    }
}


void SetErrorFileName(const Bstring& ErrorFileName, const int OpenItFlg)
{  
#ifdef MULTITHREAD
    if (oError_OpenFlg==0 &&  oError_NameDefined==0) {
	mutex_init(&ErrorFileLock, USYNC_THREAD, (void *)NULL);
	// I am unsure if we can do mutex_init several times.
	// So we check for now that it is the first time.
    }
#endif 

    if (oError_OpenFlg) CloseErrorFile();

    if (ErrorFileName=="") {
        fprintf(stderr, "\nNo error file name!\n");
        exit(-1);
    }

    TheErrorFileName = ErrorFileName;
    oError_NameDefined = 1;
    
    cout.precision(17);
    cout.setf( ios::scientific, ios::floatfield );

    cerr.precision(17);
    cerr.setf( ios::scientific, ios::floatfield );

    if (OpenItFlg) IsOpenQ();
}

void CloseErrorFile()
{  
    if (oError_OpenFlg) {
#ifdef MULTITHREAD
	mutex_lock(&ErrorFileLock) ;
#endif
	fclose(oError);
#ifdef MULTITHREAD
	mutex_unlock(&ErrorFileLock) ;
#endif
	oError_OpenFlg = 0;
    }
}

void FlushErrorFile()
{  
    if (oError_OpenFlg) {
#ifdef MULTITHREAD
	mutex_lock(&ErrorFileLock) ;
#endif
	fflush(oError);
#ifdef MULTITHREAD
	mutex_unlock(&ErrorFileLock) ;
#endif
    }
}
FILE * GetErrorFile()
{
    return oError ;
}

FILE* SetStdError(FILE* n_stdErr)
{
    assert(n_stdErr != NULL);
    assert(!ferror(n_stdErr));

    FILE *os = stdErr;
    stdErr = n_stdErr;

    return os;
}
#endif




static int monitoredErrors = 0;

void clearErrorMonitorStatus(){
    monitoredErrors = 0;
}
int  getErrorMonitorStatus(){
    return monitoredErrors;
}


static bool useErrorForBtrace=false;
static bool useLog2ForBtrace=false;
static bool btraceRedirectionDetermined=false;
static bool useReportProgress=false;
static int  progessStamp=0;

void Btracef(const Bstring & bs){
    if(!btraceRedirectionDetermined){
        if(getenv("BTRACEF")!=NULL){
            useErrorForBtrace=true;
        }
        if(getenv("BTRACEFLOG2")!=NULL){
            useLog2ForBtrace=true;
        }
        if(getenv("BTRACEPRO")!=NULL){
            useReportProgress=true;
        }

        btraceRedirectionDetermined=true;
    }

    if(bs.hasSuffix("BTRACEFON")){
       useErrorForBtrace=true;
    }
    if(bs.hasSuffix("BTRACEFOFF")){
       useErrorForBtrace=false;
    }

    bool toPrint=false;

    if(useReportProgress && bs.hasSuffix("%")){
        int nowStamp=int ( double(clock())/double(CLOCKS_PER_SEC * 1) );
        if( nowStamp != progessStamp) {
           toPrint=true;
           progessStamp=nowStamp;
        }


    } else {
       toPrint=true;
    }

    if (toPrint){
        if(useErrorForBtrace) BtracefError(bs);
        if(useLog2ForBtrace)  BtracefLog2(bs);
    }
}


static long prev_clocklong=0;


void (*storedErrorReportingFunction)(int severity,const Bstring &  errorMsg)=NULL;

void setMessageReportingFunction(void (myErrorReportingFunction)(int severity,const Bstring &  errorMsg)){
    storedErrorReportingFunction=myErrorReportingFunction;
}


const Bstring & getMessagePrefix(int severity){
  //static Bstring messagePrefix[]={"",prL1,prL2,prTr,prW,"",prTr,prE};
   static Bstring messagePrefix[]={"","LOG1    : ", "LOG2    : ", "BTRACEF : ",
                                    "WARNING : ",   "",       "BTRACEF : ", "ERROR   : "};
   return messagePrefix[severity];
}

const int useLargerForPrintingToStderr=5;

void  messageHandler(int severity, const Bstring& Message){
#ifdef MULTITHREAD
    mutex_lock(&ErrorFileLock) ;
#endif




    Bstring timeStamp;
    bool useTimeStamps=false;
    if (useTimeStamps) {
        long clocklong=clock();
        timeStamp=Int2Str(clocklong)+" "+Int2Str(clocklong-prev_clocklong);
        prev_clocklong= clocklong;
        if(storedErrorReportingFunction!=NULL)storedErrorReportingFunction(severity,timeStamp+Message);
    } else {
        if(storedErrorReportingFunction!=NULL)storedErrorReportingFunction(severity,Message);
    }

    Bstring fullMessage=Message;
    if(    fullMessage.hasPrefix("NOW:")  ){
        fullMessage.dropPrefix("NOW:");

    }

    fullMessage=getMessagePrefix(severity)+timeStamp+fullMessage;
    fullMessage.replacePartOfString("<br>"," ",Bstring::allStr);

    if(IsOpenQ()) {
#ifdef GCC3_IOSTREAMIO
        oError <<fullMessage<<endl;
        if(severity>=useLargerForPrintingToStderr) *stdErr<<fullMessage<<endl;
#else
        fprintf(oError, "%s\n",fullMessage.c_str());
        fflush(oError);
        if(severity>useLargerForPrintingToStderr) fprintf(stdErr, "%s\n",fullMessage.c_str());

        // If you want to redirect PrintInfo to stdout do this:
        if(severity==useLargerForPrintingToStderr) fprintf(stdErr, "%s\n",fullMessage.c_str());

#endif

        byteCounter+=fullMessage.length();
    }
    else
    {
        fprintf(stderr, "%s\n", fullMessage.c_str());

    }


#ifdef MULTITHREAD
    mutex_unlock(&ErrorFileLock) ;
#endif

}

void Log1(const Bstring & Message)
{
    messageHandler(1,Message);
}

void Log2( const Bstring & Message)
{
    messageHandler(2,Message);
}

void BtracefLog2( const Bstring & Message)
{
    messageHandler(3,Message);
}

void Warning(const Bstring& Message)
{
    if(Message=="") {
        return;
    }
    messageHandler(4,Message);
}

void PrintInfo(const Bstring& Message)
{
    messageHandler(5,Message);
}

void BtracefError( const Bstring & Message)
{
    messageHandler(6,Message);
}

void Error(const Bstring& Message)
{
    if(Message=="") {
        return;
    }
    messageHandler(7,Message);
    monitoredErrors++;
#ifdef WINDOWSDEBUGGER
    OutputDebugString((Message+"\n").c_str());
#endif
}





int validity_warnings=1;

void valid(double x, const Bstring & fromWhere, bool allowErrors )
{
    if (portable_isnan(x)) {
        Error("Not a number detected (NaN) in call to valid(). Probably non-initialized value. Happened at "+fromWhere+".");
        if (!allowErrors) {
            assert(0);
        }else{
            Error("Allow this error.");
        }

    }

    if (!portable_isfinite(x)) {
        Error("Infinte number detected (inf) in call to valid(). Probably non-initialized value. Happened at "+fromWhere+".");
        if (!allowErrors) {
            assert(0);
        }else{
            Error("Allow this error.");
        }

    }

    //cerr << "valid(" << x << ")" << endl;
    if ((x>1.0e+200 || x<-1.0e+200)) {

        // double * p=0;
        // *p=x; // A way to force stop execution

        Error("Suspicious number detected in call to valid() in compress.cc.  x=" + ToStr(x)+
              ". Suspicious because |x|>1e200. Simulation is not permitted. Probably non-initialized value. Happened at "+fromWhere+".");

        if (!allowErrors) {
            assert(0);
        }else{
            Error("Allow this error.");
        }

    }

#ifdef DEBUGFLG
    if (  ((x<1.0e-200)&&(x>0))
          ||
          ((x>-1.0e-200)&&(x<0))  ) {

        if (validity_warnings)
        {
            validity_warnings--;
            Warning("Under DEBUGFLG: Very small number is passed to valid()=" + ToStr(x) + ". This might be caused by non-initialized value."+
                    "Further messages of this kind are suppressed ."+ " Happened at "+fromWhere+".");
        }
    }
#endif
}



//--------------------------------------------------------------------
bool  isvalid(double x )
{
    if (portable_isnan(x)) {
        return false;
    }
    if (!portable_isfinite(x)) {
        return false;
    }
    //cerr << "valid(" << x << ")" << endl;
    if ((x>1.0e+200 || x<-1.0e+200)) {
        return false;
    }
#ifdef DEBUGFLG
    if (  ((x<1.0e-200)&&(x>0))
          ||
          ((x>-1.0e-200)&&(x<0))  ) {

        if (validity_warnings)
        {


            return false;

        }
    }
#endif
    return true;
}



