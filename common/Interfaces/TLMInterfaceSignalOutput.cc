#include "Interfaces/TLMInterfaceSignalOutput.h"
#include "Communication/TLMCommUtil.h"
#include "Plugin/TLMPlugin.h"
#include <deque>
#include <string>
#include "double33Mat.h"


//TODO: This is used both by 1D and 3D, should probably be defined in one place. /robbr
static const double TLM_DAMP_DELAY = 1.5;

TLMInterfaceOutput::TLMInterfaceOutput(TLMClientComm &theComm, std::string &aName, double StartTime, std::string Domain)
    : TLMInterfaceSignal(theComm, aName, StartTime, 1, "Output", Domain) {}

TLMInterfaceOutput::~TLMInterfaceOutput() {
    if(DataToSend.size() != 0) {
        TLMErrorLog::Log(std::string("Interface ") + GetName() + " sends rest of data for time= " +
                         TLMErrorLog::ToStdStr(DataToSend.back().time));

        Comm.PackTimeDataMessageSignal(InterfaceID, DataToSend, Message);
        TLMCommUtil::SendMessage(Message);
    }
}










