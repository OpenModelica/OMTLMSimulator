#ifndef TLMINTERFACESIGNALINPUT_H
#define TLMINTERFACESIGNALINPUT_H

#include "TLMInterfaceSignal.h"

class TLMInterfaceSignalInput : public TLMInterfaceSignal
{
public:
  TLMInterfaceSignalInput(TLMClientComm &theComm, std::string &aName, double StartTime);

  //! Destructor. Sends the rest of the data if necessary.
  ~TLMInterfaceSignalInput();

  void GetValue(double time, double *value);


};

#endif // TLMINTERFACESIGNALINPUT_H
