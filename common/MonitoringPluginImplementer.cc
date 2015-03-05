#include "MonitoringPluginImplementer.h"
#include "TLMCommUtil.h"

MonitoringPluginImplementer::MonitoringPluginImplementer()
{
}

//! CreateInstance static "factory" method returns
//! a pointer to the object implementing the interface.
MonitoringPluginImplementer* MonitoringPluginImplementer::CreateInstance() {
    return new MonitoringPluginImplementer();
}

void MonitoringPluginImplementer::ReceiveTimeData(TLMInterface* reqIfc, double time)  {
    while(time > reqIfc->GetNextRecvTime()) { // while data is needed

        // Receive data untill there is info for this interface
        string mess("Interface ");
        TLMErrorLog::Log(mess + reqIfc->GetName() +
                         " needs data for time= " + TLMErrorLog::ToStdStr(time));

        TLMInterface* ifc = NULL;

        do {

            // Receive a message
            if(!TLMCommUtil::ReceiveMessage(Message)) // on error leave this loop and use extrapolation
                break;

            // Get the target ID
            int id = Message.Header.TLMInterfaceID;

            // Use the ID to get to the right interface object
            int idx = GetInterfaceIndex(id);
            ifc = Interfaces[idx];

            // Unpack the message into the Interface object data structures
            ifc->UnpackTimeData(Message);

            // Received data
            TLMErrorLog::Log(string("Interface ") + ifc->GetName() + " got data until time= "
                             + TLMErrorLog::ToStdStr(ifc->GetNextRecvTime()));

        } while(ifc != reqIfc); // loop until a message for this interface arrives

        if(ifc == NULL) break; // receive error - breaking

        TLMErrorLog::Log(string("Got data until time=") + TLMErrorLog::ToStdStr(ifc->GetNextRecvTime()) );
    }
}

// Init method. Should be called after the default constructor. It will
// initialize the object and connect to TLMManager. Will return true
// on success, false otherwize. Note that the method can be called
// only once.
bool MonitoringPluginImplementer::Init(std::string name,
                                       double timeStart,
                                       double timeEnd,
                                       double maxStep,
                                       std::string ServerName) {
    if(Connected) return true;

    string::size_type colPos = ServerName.rfind(':');

    if(colPos == string::npos) {
        TLMErrorLog::Warning("In " + name + string(": server name string expected <server>:<port>, got:") + ServerName);
        return false;
    }

    int port = atoi(ServerName.c_str() + colPos + 1);

    string host = ServerName.substr(0,colPos);

    Message.SocketHandle = ClientComm.ConnectManager(host, port);

    if( Message.SocketHandle < 0 ){
        TLMErrorLog::Warning("In " + name + ": initialization failed, could not connect to TLM manager");
        return false;
    }

    StartTime = timeStart;
    EndTime = timeEnd;
    MaxStep = maxStep;

    Connected = true;

    // No model checking for monitoring connections
    ModelChecked = true;

    return true;
}
