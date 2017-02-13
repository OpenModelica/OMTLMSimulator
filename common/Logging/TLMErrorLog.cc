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
#ifndef WIN32
void _strtime(char* timestring) {

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strcpy(tmpbuf,asctime (timeinfo));

}
#endif


bool  TLMErrorLog::LoggingOn = false;
bool  TLMErrorLog::WarningOn = false;
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

void TLMErrorLog::SetDebugOut(bool Enable) {

    LoggingOn =  Enable;
    if(Enable) Log("Logging is enabled in TLMErrorLog::SetDebugOut");
}

// FatalError function writes a message to log file
// then terminates the program abnormally.
void TLMErrorLog::FatalError(const std::string& mess) {
    Open();
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

    if(WarningOn) {
        Open();
        *outStream << TimeStr() << " Warning: " << mess << std::endl;

        if(NormalErrorLogOn) {
            _strtime(tmpbuf);
#ifdef USE_ERRORLOG
            ::Warning("TMLLog:"+Bstring(tmpbuf)+" "+mess);
#endif
        }

    }
}

// Log function logs a message to log file
void  TLMErrorLog::Log(const std::string& mess) {

    if(!LoggingOn) return;
    Open();
    *outStream << TimeStr() << " Log: " << mess << std::endl;
    if(NormalErrorLogOn) {
        _strtime(tmpbuf);
#ifdef USE_ERRORLOG
        Log1("TMLLog:"+Bstring(tmpbuf)+" "+mess);
#endif
    }

}

// A utility function often used to log numerical information
std::string  TLMErrorLog::ToStdStr(double val) {
    char buf[30];

    sprintf(buf, "%.10f", val);

    return std::string(buf);
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
