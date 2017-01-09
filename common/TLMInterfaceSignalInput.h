#ifndef TLMINTERFACESIGNALINPUT_H
#define TLMINTERFACESIGNALINPUT_H

#include "TLMInterfaceSignal.h"

class TLMInterfaceInput : public TLMInterfaceSignal
{
public:
  TLMInterfaceInput(TLMClientComm &theComm, std::string &aName, double StartTime, InterfaceDomain Domain=DomainSignal);

  //! Destructor. Sends the rest of the data if necessary.
  ~TLMInterfaceInput();

  void GetValue(double time, double *value);


};

#endif // TLMINTERFACESIGNALINPUT_H
