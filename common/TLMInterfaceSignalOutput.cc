#include "TLMInterfaceSignalOutput.h"
#include "TLMCommUtil.h"
#include "TLMPlugin.h"
#include <deque>
#include <string>
#include "double33Mat.h"


//TODO: This is used both by 1D and 3D, should probably be defined in one place. /robbr
static const double TLM_DAMP_DELAY = 1.5;

TLMInterfaceOutput::TLMInterfaceOutput(TLMClientComm &theComm, std::string &aName, double StartTime, InterfaceDomain Domain)
    : TLMInterfaceSignal(theComm, aName, StartTime, DimensionalitySignal, CausalityOutput, Domain) {}

TLMInterfaceOutput::~TLMInterfaceOutput() {
    if(DataToSend.size() != 0) {
        TLMErrorLog::Log(std::string("Interface ") + GetName() + " sends rest of data for time= " +
                         TLMErrorLog::ToStdStr(DataToSend.back().time));

        Comm.PackTimeDataMessageSignal(InterfaceID, DataToSend, Message);
        TLMCommUtil::SendMessage(Message);
    }
}





// Set motion data and communicate if necessary.
void TLMInterfaceOutput::SetTimeData(double time,
                                     double value) {
    // put the variables into TLMTimeData structure and the end of  DataToSend vector
    int lastInd = DataToSend.size();
    DataToSend.resize( lastInd + 1);
    TLMTimeDataSignal& item = DataToSend[lastInd];
    item.time = time;
    item.Value = value;

    TLMErrorLog::Log(std::string("Interface ") + GetName() +
                     " SET for time= " + TLMErrorLog::ToStdStr(time));

    // Send the data if we past the synchronization point or are in data request mode.
    if(time >= LastSendTime + Params.Delay / 2 || Params.mode > 0.0 ) {
        SendAllData();
    }
}








