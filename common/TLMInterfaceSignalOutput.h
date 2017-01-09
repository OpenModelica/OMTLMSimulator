#ifndef TLMINTERFACESIGNALOUTPUT_H
#define TLMINTERFACESIGNALOUTPUT_H

#include "TLMInterfaceSignal.h"

class TLMInterfaceOutput : public TLMInterfaceSignal
{
public:
  TLMInterfaceOutput(TLMClientComm &theComm, std::string &aName, double StartTime, InterfaceDomain Domain=DomainUnspecified);

  //! Destructor. Sends the rest of the data if necessary.
  ~TLMInterfaceOutput();

  void SetTimeData(double time, double value);
};

#endif // TLMINTERFACESIGNALOUTPUT_H
