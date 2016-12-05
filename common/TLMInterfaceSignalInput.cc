#include "TLMInterfaceSignalInput.h"
#include "TLMCommUtil.h"
#include "TLMPlugin.h"
#include <deque>
#include <string>
#include "double33Mat.h"


//TODO: This is used both by 1D and 3D, should probably be defined in one place. /robbr
static const double TLM_DAMP_DELAY = 1.5;

TLMInterfaceSignalInput::TLMInterfaceSignalInput(TLMClientComm &theComm, std::string &aName, double StartTime)
    : TLMInterfaceSignal(theComm, aName, StartTime, InterfaceSignalInput) {}

TLMInterfaceSignalInput::~TLMInterfaceSignalInput() {}




void TLMInterfaceSignalInput::GetValue( double time,
                                   double* value) {
    TLMTimeDataSignal request;
    request.time = time - Params.Delay;
    GetTimeData(request);

    TLMPlugin::GetValueSignal(request, Params, value);
}



