//!
//! \file TLMInterfaceOutput.h
//!
//! Provides a definition for the TLMInterfaceOutput class
//!
//!
//! \author   Robert Braun
//!

#ifndef TLMINTERFACESIGNALOUTPUT_H
#define TLMINTERFACESIGNALOUTPUT_H

#include "Interfaces/TLMInterfaceSignal.h"

//!
//! TLMInterfaceSignal provides the client side functionality for a single output interface
//!
class TLMInterfaceOutput : public TLMInterfaceSignal {
public:
  TLMInterfaceOutput(TLMClientComm &theComm, std::string &aName, double StartTime, std::string Domain="signal");

  //! Destructor. Sends the rest of the data if necessary.
  ~TLMInterfaceOutput();

  void SetTimeData(double time, double value);
};

#endif // TLMINTERFACESIGNALOUTPUT_H
