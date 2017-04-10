#include "Parameters/TLMParameter.h"
#include "Plugin/TLMPlugin.h"
#include <deque>
#include <string>
#include "double33Mat.h"

using std::deque;
using std::vector;

#include <iostream>
using std::cout;

#include <fstream>
using std::ofstream;
using namespace tlmMisc;


//! Constrctor for the TLMParameter class, which handles submodel parameters
TLMParameter::TLMParameter(TLMClientComm& theComm, std::string& aName, std::string& aDefaultValue):
    Name(aName),
    Value(aDefaultValue),
    ParameterID(-1),
    Comm(theComm) {
    Comm.CreateParameterRegMessage(aName, aDefaultValue, Message);
    Message.SocketHandle = Comm.GetSocketHandle();

    TLMCommUtil::SendMessage(Message);

    TLMCommUtil::ReceiveMessage(Message);
    while(Message.Header.MessageType != TLMMessageTypeConst::TLM_REG_PARAMETER) {
        TLMCommUtil::ReceiveMessage(Message);
    }
    ParameterID =  Message.Header.TLMParameterID;

    Comm.UnpackRegParameterMessage(Message, Value);

    TLMErrorLog::Log(std::string("Parameter ") + GetName() + " got ID " + TLMErrorLog::ToStdStr(ParameterID));
}














