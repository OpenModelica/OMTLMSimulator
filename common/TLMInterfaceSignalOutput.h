#ifndef TLMINTERFACESIGNALOUTPUT_H
#define TLMINTERFACESIGNALOUTPUT_H

#include "TLMInterfaceSignal.h"

class TLMInterfaceSignalOutput : public TLMInterfaceSignal
{
public:
  TLMInterfaceSignalOutput(TLMClientComm &theComm, std::string &aName, double StartTime);

  //! Destructor. Sends the rest of the data if necessary.
  ~TLMInterfaceSignalOutput();

  void SetTimeData(double time, double value);
};

#endif // TLMINTERFACESIGNALOUTPUT_H
