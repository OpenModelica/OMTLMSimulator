#include "TLMParameter.h"
#include "TLMPlugin.h"
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


TLMParameter::TLMParameter(TLMClientComm& theComm, std::string& aName, std::string& aDefaultValue):
    Name(aName),
    Value(aDefaultValue),
    ParameterID(-1),
    Comm(theComm)
{ // TLMParameter constructor
    Comm.CreateParameterRegMessage(aName, aDefaultValue, Message);
    Message.SocketHandle = Comm.GetSocketHandle();

    TLMCommUtil::SendMessage(Message);

    TLMCommUtil::ReceiveMessage(Message);
    while(Message.Header.MessageType != TLMMessageTypeConst::TLM_REG_PARAMETER) {
        TLMCommUtil::ReceiveMessage(Message);
    }
    ParameterID =  Message.Header.TLMInterfaceID;

    TLMErrorLog::Log(std::string("Parameter ") + GetName() + " got ID " + TLMErrorLog::ToStdStr(ParameterID));
}














