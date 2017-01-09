#include "TLMInterfaceSignalInput.h"
#include "TLMCommUtil.h"
#include "TLMPlugin.h"
#include <deque>
#include <string>
#include "double33Mat.h"


//TODO: This is used both by 1D and 3D, should probably be defined in one place. /robbr
static const double TLM_DAMP_DELAY = 1.5;

TLMInterfaceInput::TLMInterfaceInput(TLMClientComm &theComm, std::string &aName, double StartTime, std::string Domain)
    : TLMInterfaceSignal(theComm, aName, StartTime, 1, "Input", Domain) {}

TLMInterfaceInput::~TLMInterfaceInput() {}




void TLMInterfaceInput::GetValue( double time,
                                   double* value) {
    TLMTimeDataSignal request;
    request.time = time - Params.Delay;
    GetTimeData(request);

    TLMPlugin::GetValueSignal(request, Params, value);
}



