//!
//! \file TLMErrorLog.h
//! 
//! Defines the functions used for error logging 
//!
//!
//! \author   Iakov Nakhimovski
//!

#ifndef TLMErrorLog_h_
#define TLMErrorLog_h_

#include <string>
#include <ostream>
#include <fstream>
#include <iostream>

enum TLMLogLevel { Fatal, Warning, Info, Debug };

//! Error handling is implemented in the most simple way
//! with the functions that write messages to standard error output (cerr).
//! In addition FatalError calls abort() to terminate the application.
//! Graceful error handling, possibly with exceptions may
//! be implemented later.
class TLMErrorLog {

public:
    //! FatalError function writes a message to standard error output
    //! then terminates the program abnormally.
    static void FatalError(const std::string& mess);

    //! Warning function prints a warning message to standard error output
    static void Warning(const std::string& mess);

    //! Info function logs an info message to standard error output
    static void Info(const std::string& mess);

    //! Debug function logs a debug message to standard error output
    static void Debug(const std::string& mess);

    //! Utility functions often used to log numerical information
    static std::string ToStdStr(double val);
    static std::string ToStdStr(int val);

    //! SetDebugOut function enables/disables debug information
    //! output from TLM plugin.
    //! Input: if Enable is true - output is on, othewise - off.
    static void SetLogLevel(TLMLogLevel logLevel) { LogLevel = logLevel; }

    static TLMLogLevel GetLogLevel() { return LogLevel; }

    //! This function enables so that logs are duplicated to the normal *.log file as well.
    //! Input: if Enable is true - output is on, othewise - off.
    static void SetNormalErrorLogOn(bool Enable) { NormalErrorLogOn = Enable; }

    static bool IsNormalErrorLogOn() { return NormalErrorLogOn; }

    //! Sets the output stream for output of all log, warning, and error messages.
    //! Default output stream id std::cout
    static void SetOutStream(std::ostream& of) { outStream = &of; }

    //! Sets the error mode to exception instead of abort()/exit().
    //! When exception mode is on TLMErrorLog will throw an exception on fatal error.
    //! Otherwise abort()/exit() is called on fatal error.
    static void SetErrorException(bool Enable) { ExceptionOn = Enable; }

    //! Returns the state of the error exception mode.
    static bool ExceptionEnabled() { return ExceptionOn; }

    //! Return current time as string.
    static std::string TimeStr();
private:

    //! LogLevel specifies level of logging to be used
    static TLMLogLevel LogLevel;

    //! Exception mode on in enabled.
    //! See also SetErrorException(...).
    static bool ExceptionOn;

    //! All logs are duplicated to the normal *.log file as well.
    //! It is ON by default.
    static bool NormalErrorLogOn;

    //! All log entries start with a time stamp.
    //! It is OFF by default.
    static bool LogTimeOn;

    //! The used output stream for all log, warning, and error messages.
    static std::ostream* outStream;

    //! Open log file
    static  void Open();
};



#endif
