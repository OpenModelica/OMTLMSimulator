//!
//! \file TLMInterfaceInput.h
//!
//! Provides a definition for the TLMInterfaceInput class
//!
//!
//! \author   Robert Braun
//!

#ifndef TLMINTERFACESIGNALINPUT_H
#define TLMINTERFACESIGNALINPUT_H

#include "TLMInterfaceSignal.h"

//!
//! TLMInterfaceSignal provides the client side functionality for a single input interface
//!
class TLMInterfaceInput : public TLMInterfaceSignal
{
public:
  TLMInterfaceInput(TLMClientComm &theComm, std::string &aName, double StartTime, std::string Domain="Signal");

  //! Destructor. Sends the rest of the data if necessary.
  ~TLMInterfaceInput();

  void GetValue(double time, double *value);


};

#endif // TLMINTERFACESIGNALINPUT_H
