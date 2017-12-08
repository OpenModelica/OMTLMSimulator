/**
* File: TLMErrorLog.cc
* 
* Implementat of the functions used for error logging 
*/

#include "Logging/TLMErrorLog.h"
#include <iostream>
#include <cstdlib>
#include <cstring>


//#define USE_ERRORLOG
// to use Error/Log file as well for reporting, also with time stamps 
// Due to BZ306 we had to add detailed logging on Windows.
// If you get any difficulties in linking with lib/misc
// just remove this #define.

#ifdef  USE_ERRORLOG
#include "ErrorLog.h"
#endif

#include <ctime>
#include <sys/timeb.h>
char tmpbuf[128]; 


//#ifndef _MSC_VER
#if !(defined(WIN32) || defined(__MINGW32__))
void _strtime(char* timestring) {
    (void)timestring;
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strcpy(tmpbuf,asctime (timeinfo));

}
#endif



TLMLogLevel TLMErrorLog::LogLevel = TLMLogLevel::Fatal;
bool  TLMErrorLog::ExceptionOn = false;
bool  TLMErrorLog::NormalErrorLogOn = false;
bool  TLMErrorLog::LogTimeOn = false;
std::ostream* TLMErrorLog::outStream = NULL;


void TLMErrorLog::Open() {
    if(TLMErrorLog::outStream==NULL) {
        TLMErrorLog::outStream=new std::ofstream("TLMlogfile.log");
        *outStream << TimeStr() << " Starting log" << std::endl;
    }
}


// FatalError function writes a message to log file
// then terminates the program abnormally.
void TLMErrorLog::FatalError(const std::string& mess) {
    Open();
    std::cout << TimeStr() << " Fatal error: " << mess << std::endl;
    *outStream << TimeStr() << " Fatal error: " << mess << std::endl;
    if(NormalErrorLogOn) {
        _strtime(tmpbuf);
#ifdef USE_ERRORLOG
        Error("TMLLog:"+Bstring(tmpbuf)+" "+mess);
#endif
    }

    if(!ExceptionOn) {
#ifdef DEBUGFLG
        abort(); // note that abort results in strange messages on Windows
        // Useful on Linux since it generate a core
#endif
        exit(1);
    }
    else {
        throw mess;
    }
}

// Warning function prints a warning message to log file
//
void  TLMErrorLog::Warning(const std::string& mess) {
    if(LogLevel < TLMLogLevel::Warning) return;
    Open();
    std::cout << TimeStr() << " Warning: " << mess << std::endl;
    *outStream << TimeStr() << " Warning: " << mess << std::endl;

    if(NormalErrorLogOn) {
        _strtime(tmpbuf);
#ifdef USE_ERRORLOG
        ::Warning("TMLLog:"+Bstring(tmpbuf)+" "+mess);
#endif
    }
}

// Log function logs a message to log file
void  TLMErrorLog::Info(const std::string& mess) {
    if(LogLevel < TLMLogLevel::Info) return;
    Open();
    *outStream << TimeStr() << " Info: " << mess << std::endl;
    if(NormalErrorLogOn) {
        _strtime(tmpbuf);
#ifdef USE_ERRORLOG
        Log1("TMLLog:"+Bstring(tmpbuf)+" "+mess);
#endif
    }
}


// Log function logs a message to log file
void  TLMErrorLog::Debug(const std::string& mess) {
    if(LogLevel < TLMLogLevel::Debug) return;
    Open();
    *outStream << TimeStr() << " Debug: " << mess << std::endl;
    if(NormalErrorLogOn) {
        _strtime(tmpbuf);
#ifdef USE_ERRORLOG
        Log1("TMLLog:"+Bstring(tmpbuf)+" "+mess);
#endif
    }
}


// A utility function often used to log numerical information
std::string  TLMErrorLog::ToStdStr(double val) {
    TLMErrorLog::Debug("Debug 1.");
    return std::to_string(val);
}

// A utility function often used to log numerical information
std::string  TLMErrorLog::ToStdStr(int val) {
    return std::to_string(val);
}

std::string  TLMErrorLog::TimeStr() {
    if(LogTimeOn) {
        time_t rawtime;
        struct tm * timeinfo;

        time (&rawtime);
        timeinfo = localtime (&rawtime);
        return std::string(asctime(timeinfo));
    }
    else
        return std::string("");
}
