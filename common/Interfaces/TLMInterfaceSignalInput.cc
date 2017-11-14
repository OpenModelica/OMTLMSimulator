#include "Interfaces/TLMInterfaceSignalInput.h"
#include "Communication/TLMCommUtil.h"
#include "Plugin/TLMPlugin.h"
#include <deque>
#include <string>
#include "double33.h"


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

    //Default value is the initial value
    (*value)=InitialValue;

    TLMPlugin::GetValueSignal(request, Params, value);
}







