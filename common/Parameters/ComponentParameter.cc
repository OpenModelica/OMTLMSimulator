#include "Parameters/ComponentParameter.h"
#include "Plugin/TLMPlugin.h"
#include <deque>
#include <string>
#include "double33.h"

using std::deque;
using std::vector;

#include <iostream>
using std::cout;

#include <fstream>
using std::ofstream;


//! Constrctor for the ComponentParameter class, which handles submodel parameters
ComponentParameter::ComponentParameter(TLMClientComm& theComm, std::string& aName, std::string& aDefaultValue):
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
    ParameterID =  Message.Header.ComponentParameterID;

    Comm.UnpackRegParameterMessage(Message, Value);

    TLMErrorLog::Info(std::string("Parameter ") + GetName() + " got ID " + TLMErrorLog::ToStdStr(ParameterID));
}














