// File: PluginImplementer.cc
// Provides the implementation of methods for PluginImplementer.
#include "Plugin/TLMPlugin.h"
#include "Communication/TLMCommUtil.h"
#include "Plugin/PluginImplementer.h"
#include <cassert>
#include <iostream>
#include <csignal>
#include <sstream>
#include <algorithm>
#include <locale>
#ifndef _WIN32
#include <unistd.h>
#endif

using std::string;
using std::vector;
using std::map;

#include <fstream>
using std::ofstream;

PluginImplementer* PluginImplementerInstance = 0;
TLMPlugin* TLMPlugin::CreateInstance() {
    PluginImplementerInstance = new PluginImplementer;
    return PluginImplementerInstance;
}


void PluginImplementer::InterfaceReadyForTakedown(std::string IfcName) {
    ++nIfcWaitingForTakedown;

    TLMErrorLog::Debug("Interface "+IfcName+" is ready for takedown.");

    if(nIfcWaitingForTakedown >= Interfaces.size()) {
        AwaitClosePermission();
        exit(0);
    }
}

void PluginImplementer::AwaitClosePermission()
{
    Message->Header.MessageType = TLMMessageTypeConst::TLM_CLOSE_REQUEST;
    TLMCommUtil::SendMessage(*Message);
    while(Message->Header.MessageType != TLMMessageTypeConst::TLM_CLOSE_PERMISSION) {
        TLMErrorLog::Info("Awaiting close permission...");
        TLMCommUtil::ReceiveMessage(*Message);
    }
    TLMErrorLog::Info("Close permission received.");
}

void PluginImplementer::SetInitialForce3D(int interfaceID, double f1, double f2, double f3, double t1, double t2, double t3)
{
    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterface3D* ifc = dynamic_cast<TLMInterface3D*>(Interfaces[idx]);

    assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

    ifc->SetInitialForce(f1,f2,f3,t1,t2,t3);
}

void PluginImplementer::SetInitialFlow3D(int interfaceID, double v1, double v2, double v3, double w1, double w2, double w3)
{
  // Use the ID to get to the right interface object
  int idx = GetInterfaceIndex(interfaceID);
  TLMInterface3D* ifc = dynamic_cast<TLMInterface3D*>(Interfaces[idx]);

  assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

  ifc->SetInitialFlow(v1,v2,v3,w1,w2,w3);
}

void PluginImplementer::SetInitialValue(int interfaceID, double value)
{
    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterfaceSignal* ifc = dynamic_cast<TLMInterfaceSignal*>(Interfaces[idx]);

    assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

    ifc->SetInitialValue(value);
}

void PluginImplementer::SetInitialForce1D(int interfaceID, double force)
{
    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterface1D* ifc = dynamic_cast<TLMInterface1D*>(Interfaces[idx]);

    assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

    ifc->SetInitialForce(force);
}

void PluginImplementer::SetInitialFlow1D(int interfaceID, double flow)
{
  // Use the ID to get to the right interface object
  int idx = GetInterfaceIndex(interfaceID);
  TLMInterface1D* ifc = dynamic_cast<TLMInterface1D*>(Interfaces[idx]);

  assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

  ifc->SetInitialFlow(flow);
}


void signalHandler_(int signum) {
    // Install default signal handler
    signal(signum, SIG_DFL);

    if(PluginImplementerInstance != 0) {
        PluginImplementerInstance->HandleSignal(signum);
    }

    // Call default handler.
    raise(signum);
}

PluginImplementer::PluginImplementer():
    Connected(false),
    ModelChecked(false),
    Interfaces(),
    ClientComm(),
    MapID2Ind(),
    StartTime(0.0),
    EndTime(0.0),
    MaxStep(0.0) {
    // Install out own signal handler.
    signal(SIGABRT, signalHandler_);
    signal(SIGFPE, signalHandler_);
    signal(SIGILL, signalHandler_);
    signal(SIGINT, signalHandler_);
    signal(SIGSEGV, signalHandler_);
    signal(SIGTERM, signalHandler_);
}


PluginImplementer::~PluginImplementer() {

    for(vector<omtlm_TLMInterface*>::iterator it = Interfaces.begin();
        it != Interfaces.end(); ++it) {
        delete (*it);
    }

    delete Message;
}

void PluginImplementer::HandleSignal(int signum) {
    if(Connected) {
        Message->Header.MessageType = TLMMessageTypeConst::TLM_ABORT;
        TLMCommUtil::SendMessage(*Message);
    }

    TLMErrorLog::Info("Got signal " + TLMErrorLog::ToStdStr(signum));
}

void PluginImplementer::CheckModel() {

    if(!Connected) {
        TLMErrorLog::FatalError("Check model cannot be called before the TLM client is connected to manager");
    }

    Message->Header.MessageType =  TLMMessageTypeConst::TLM_CHECK_MODEL;

    TLMCommUtil::SendMessage(*Message);
    TLMCommUtil::ReceiveMessage(*Message);

    if(! Message->Header.TLMInterfaceID) {
        TLMErrorLog::Info("Error detected on TLM manager while checking meta model");
        TLMErrorLog::FatalError("Header id is " + TLMErrorLog::ToStdStr(int(Message->Header.TLMInterfaceID)));
    }

    ModelChecked = true;
}


// Init method. Should be called after the default constructor. It will
// initialize the object and connect to TLMManager. Will return true
// on success, false otherwize. Note that the method can be called
// only once.
bool PluginImplementer::Init(std::string model,
                              double timeStart,
                              double timeEnd,
                              double maxStep,
                              std::string ServerName) {
    if(Connected) return true;

    string::size_type colPos = ServerName.rfind(':');

    if(colPos == string::npos) {
        TLMErrorLog::Warning(string("Server name string expected <server>:<port>, got:") + ServerName);
        return false;
    }

    int port = atoi(ServerName.c_str() + colPos + 1);

    string host = ServerName.substr(0,colPos);

    Message = new TLMMessage();

    if((Message->SocketHandle = ClientComm.ConnectManager(host, port)) < 0) {
        TLMErrorLog::Warning("Init failed: could not connect to TLM manager");
        return false;
    }

    TLMErrorLog::Info("Sending Component registration request");


    ClientComm.CreateComponentRegMessage(model, *Message);
    TLMCommUtil::SendMessage(*Message);
    TLMCommUtil::ReceiveMessage(*Message);

    TLMErrorLog::Info(string("Got component ID: ") +
                     TLMErrorLog::ToStdStr(Message->Header.TLMInterfaceID));

    StartTime = timeStart;
    EndTime = timeEnd;
    MaxStep = maxStep;

    Connected = true;
    SetInitialized();

    return true;
}


// Register TLM interface sends a registration request to TLMManager
// and returns the ID for the interface. '-1' is returned if
// the interface is not connected in the CompositeModel.
int  PluginImplementer::RegisteTLMInterface(std::string name , int dimensions,
                                            std::string causality, std::string domain) {
    TLMErrorLog::Info(string("Register Interface ") + name);

    //Convert causality and domain to lower-case for backwards compatibility
    std::locale loc;
    causality[0] = std::tolower(causality[0],loc);
    domain[0] = std::tolower(domain[0],loc);

    omtlm_TLMInterface *ifc;
    if(dimensions==6) {
        TLMErrorLog::Info("Registers TLM interface of type 3D");
        ifc = new TLMInterface3D(ClientComm, name, StartTime, domain);
    }
    else if(dimensions == 1 && causality == "bidirectional") {
        TLMErrorLog::Info("Registers TLM interface of type 1D");
        ifc = new TLMInterface1D(ClientComm, name, StartTime, domain);
    }
    else if(dimensions == 1 && causality == "input") {
        TLMErrorLog::Info("Registers TLM interface of type SignalInput");
        ifc = new TLMInterfaceInput(ClientComm, name, StartTime, domain);
    }
    else if(dimensions == 1 && causality == "output") {
        TLMErrorLog::Info("Registers TLM interface of type SignalOutput");
        ifc = new TLMInterfaceOutput(ClientComm, name, StartTime, domain);
    }
    else {
        TLMErrorLog::FatalError("Unknown interface type : "+domain+":"+std::to_string(dimensions));
    }

    int id = ifc->GetInterfaceID();

    TLMErrorLog::Info(string("Got interface ID: ") + TLMErrorLog::ToStdStr(id));

    // Check that this interface is connected
    if(id < 0) {
        // no need for the interface object that is not connected
        delete ifc;
        return id;
    }

    // The index of the new interface:
    int idx = Interfaces.size();

    Interfaces.push_back(ifc);

    MapID2Ind[id] = idx;

    return id;
}


int PluginImplementer::RegisterComponentParameter(std::string name, std::string defaultValue) {
    ComponentParameter *par = new ComponentParameter(ClientComm, name, defaultValue);

    int id = par->GetParameterID();

    TLMErrorLog::Info(string("Got parameter ID: ") + TLMErrorLog::ToStdStr(id));

    // The index of the new interface:
    int idx = Parameters.size();

    Parameters.push_back(par);

    MapID2Par[id] = idx;

    return id;
}


// ReceiveTimeData receives time-stamped data from coupled simulations.
// Since the order of messages can vary the specified interfaceID
// is used only to detect the last message expected when the function
// is called. Any number of messages to other interfaces can arrive
// before the desired message is received.
// Input:
//   interfaceID - ID of a TLM interface that triggered the request
void PluginImplementer::ReceiveTimeData(omtlm_TLMInterface* reqIfc, double time) {
    while(time > reqIfc->GetNextRecvTime()) { // while data is needed

        // Receive data untill there is info for this interface
        if(TLMErrorLog::GetLogLevel() >= TLMLogLevel::Info) {
           TLMErrorLog::Info("Interface " + reqIfc->GetName() +
                            " needs data for time= " + TLMErrorLog::ToStdStr(time));
        }

        double allowedMaxTime = reqIfc->GetLastSendTime() + reqIfc->GetConnParams().Delay;

        if(allowedMaxTime < time && reqIfc->GetCausality() != "input") {            //Why not for signal interfaces?
            TLMErrorLog::Warning("Interface " + reqIfc->GetName() +
                             " is NOT ALLOWED to ask data after time= " + TLMErrorLog::ToStdStr(allowedMaxTime) +
                             ". The error is: "+TLMErrorLog::ToStdStr(time - allowedMaxTime));
            break;
        }

        omtlm_TLMInterface* ifc = NULL;

        do {

            // Receive a message
            if(!TLMCommUtil::ReceiveMessage(*Message)) // on error leave this loop and use extrapolation
                break;

            // Get the target ID
            int id = Message->Header.TLMInterfaceID;

            // Use the ID to get to the right interface object
            int idx = GetInterfaceIndex(id);
            ifc = Interfaces[idx];

            // Unpack the message into the Interface object data structures
            ifc->UnpackTimeData(*Message);

            // Received data
            if(TLMErrorLog::GetLogLevel() >= TLMLogLevel::Info) {
                TLMErrorLog::Info(string("Interface ") + ifc->GetName() + " got data until time= " +
                                 TLMErrorLog::ToStdStr(ifc->GetNextRecvTime()));
            }

        } while(ifc != reqIfc); // loop until a message for this interface arrives

        if(ifc == NULL) break; // receive error - breaking

        if(TLMErrorLog::GetLogLevel() >= TLMLogLevel::Info) {
            TLMErrorLog::Info(string("Got data until time=") +
                             TLMErrorLog::ToStdStr(ifc->GetNextRecvTime()));
        }
    }
}


void PluginImplementer::GetValueSignal(int interfaceID, double time, double *value) {
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterfaceInput* ifc = dynamic_cast<TLMInterfaceInput*>(Interfaces[idx]);

    assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

    if(!ifc) {
        (*value) = 0.0;

        TLMErrorLog::Warning(string("No interface in GetForce1D()"));

        return;
    }

    // Check if the interface expects more data from the coupled simulation
    // Receive if necessary .Note that potentially more that one receive is possible
    ReceiveTimeData(ifc, time);

    // evaluate the reaction force from the TLM connection
    ifc->GetValue(time, value);

    if(ifc->waitForShutdown()) {
        InterfaceReadyForTakedown(ifc->GetName());
    }
}

//This function is for backwards compatibility, remove when position variables has been removed from all wrappers
void PluginImplementer::GetForce1D(int interfaceID, double time, double position, double speed, double *force) {
    (void)position; //Unused
    this->GetForce1D(interfaceID,time,speed,force);
}

void PluginImplementer::GetForce1D(int interfaceID, double time, double speed, double *force) {
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterface1D* ifc = dynamic_cast<TLMInterface1D*>(Interfaces[idx]);

    assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

    if(!ifc) {
        (*force) = 0.0;

        TLMErrorLog::Warning(string("No interface in GetForce1D()"));

        return;
    }

    // Check if the interface expects more data from the coupled simulation
    // Receive if necessary .Note that potentially more that one receive is possible
    ReceiveTimeData(ifc, time);

    // evaluate the reaction force from the TLM connection
    ifc->GetForce(time, speed, force);
}


void PluginImplementer::GetForce3D(int interfaceID,
                                   double time,
                                   double position[],
                                   double orientation[],
                                   double speed[],
                                   double ang_speed[],
                                   double* force) {

    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterface3D* ifc = dynamic_cast<TLMInterface3D*>(Interfaces[idx]);

    assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

    if(!ifc) {
        for(int i = 0; i < 6; i++) {
            force[i] = 0.0;
        }

        TLMErrorLog::Warning(string("No interface in GetForce3D()"));

        return;
    }

    // Check if the interface expects more data from the coupled simulation
    // Receive if necessary .Note that potentially more that one receive is possible
    ReceiveTimeData(ifc, time);

    // evaluate the reaction force from the TLM connection
    ifc->GetForce(time, position, orientation, speed, ang_speed, force);
}



void PluginImplementer::GetWaveImpedance1D(int interfaceID, double time, double *impedance, double *wave) {
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterface1D* ifc = dynamic_cast<TLMInterface1D*>(Interfaces[idx]);

    assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

    if(!ifc) {
        (*wave) = 0.0;
        (*impedance) = 0.0;

        TLMErrorLog::Warning(string("No interface in GetForce1D()"));

        return;
    }

    // Check if the interface expects more data from the coupled simulation
    // Receive if necessary .Note that potentially more that one receive is possible
    ReceiveTimeData(ifc, time);

    // evaluate the reaction force from the TLM connection
    ifc->GetWave(time, wave);
    (*impedance) = ifc->GetConnParams().Zf;
}

void PluginImplementer::GetWaveImpedance3D(int interfaceID, double time, double *Zt, double *Zr, double *wave)
{
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterface3D* ifc = dynamic_cast<TLMInterface3D*>(Interfaces[idx]);

    assert(!ifc || (ifc -> GetInterfaceID() == interfaceID));

    if(!ifc) {
        for(int i = 0; i < 6; i++) {
            wave[i] = 0.0;
        }
        (*Zt) = 0.0;

        TLMErrorLog::Warning(string("No interface in GetForce1D()"));

        return;
    }

    // Check if the interface expects more data from the coupled simulation
    // Receive if necessary .Note that potentially more that one receive is possible
    ReceiveTimeData(ifc, time);

    // evaluate the reaction force from the TLM connection
    ifc->GetWave(time, wave);
    (*Zt) = ifc->GetConnParams().Zf;
    (*Zr) = ifc->GetConnParams().Zfr;
}


void PluginImplementer::SetMotion3D(int forceID,
                                    double time,
                                    double position[],
                                    double orientation[],
                                    double speed[],
                                    double ang_speed[]) {

    if(!ModelChecked) CheckModel();
    if(forceID < 0) return;
    // Find the interface object by its ID
    int idx = GetInterfaceIndex(forceID);
    TLMInterface3D* ifc = dynamic_cast<TLMInterface3D*>(Interfaces[idx]);
    assert(ifc -> GetInterfaceID() == forceID);

    if(!ifc->waitForShutdown()) {
        // Store the data into the interface object
        TLMErrorLog::Info(string("calling SetTimeData()"));
        ifc->SetTimeData(time, position, orientation,speed,ang_speed);
    }
    else {
        // Check if all interfaces wait for shutdown
        std::vector<omtlm_TLMInterface*>::iterator iter;
        for(iter=Interfaces.begin(); iter!=Interfaces.end(); iter++) {
            if((*iter)->GetCausality() == "input") continue;
            if(! (*iter)->waitForShutdown()) return;
        }
#ifdef _MSC_VER
        WSACleanup(); // BZ306 fixed here
#else
        // needed anything ?
#endif

        InterfaceReadyForTakedown(ifc->GetName());
    }
}


void PluginImplementer::SetValueSignal(int valueID,
                                       double time,
                                       double value) {
    if(!ModelChecked) CheckModel();

    if(valueID < 0) return;

    // Find the interface object by its ID
    int idx = GetInterfaceIndex(valueID);
    TLMInterfaceOutput* ifc = dynamic_cast<TLMInterfaceOutput*>(Interfaces[idx]);
    assert(ifc -> GetInterfaceID() == valueID);

    if(!ifc->waitForShutdown()) {
        // Store the data into the interface object
        TLMErrorLog::Info(string("calling SetTimeData()"));
        ifc->SetTimeData(time, value);
    }
    else {
        // Check if all interfaces wait for shutdown
        std::vector<omtlm_TLMInterface*>::iterator iter;
        for(iter=Interfaces.begin(); iter!=Interfaces.end(); iter++) {
            if((*iter)->GetCausality() == "input") continue;
            if(! (*iter)->waitForShutdown()) return;
        }
#ifdef _MSC_VER
        WSACleanup(); // BZ306 fixed here
#else
        // needed anything ?
#endif

        InterfaceReadyForTakedown(ifc->GetName());
    }
}

void PluginImplementer::SetMotion1D(int forceID,
                                    double time,
                                    double position,
                                    double speed) {

    if(!ModelChecked) CheckModel();
    if(forceID < 0) return;
    // Find the interface object by its ID
    int idx = GetInterfaceIndex(forceID);
    TLMInterface1D* ifc = dynamic_cast<TLMInterface1D*>(Interfaces[idx]);
    assert(ifc -> GetInterfaceID() == forceID);

    if(!ifc->waitForShutdown()) {
        // Store the data into the interface object
        if(TLMErrorLog::GetLogLevel() >= TLMLogLevel::Info) {
            TLMErrorLog::Info(string("calling SetTimeData()"));
        }
        ifc->SetTimeData(time, position, speed);
    }
    else {
        // Check if all interfaces wait for shutdown
        std::vector<omtlm_TLMInterface*>::iterator iter;
        for(iter=Interfaces.begin(); iter!=Interfaces.end(); iter++) {
            if((*iter)->GetCausality() == "input") continue;
            if(! (*iter)->waitForShutdown()) return;
        }
#ifdef _MSC_VER     
        WSACleanup(); // BZ306 fixed here
#else
        // needed anything ?
#endif     

        InterfaceReadyForTakedown(ifc->GetName());
    }
}

// GetConnectionParams returnes the ConnectionParams for
// the specified interface ID. Interface must be registered
// first.
void PluginImplementer::GetConnectionParams(int interfaceID, TLMConnectionParams& ParamsOut) {

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    omtlm_TLMInterface* ifc = Interfaces[idx];
    assert(ifc -> GetInterfaceID() == interfaceID);

    ParamsOut = ifc->GetConnParams();
}

void PluginImplementer::GetTimeDataSignal(int interfaceID, double time, TLMTimeDataSignal &DataOut, bool monitoring) {
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);

    if(!monitoring) {
        // Use the ID to get to the right interface object
        TLMInterfaceInput* ifc = dynamic_cast<TLMInterfaceInput*>(Interfaces[idx]);
        assert(ifc -> GetInterfaceID() == interfaceID);
        // Check if the interface expects more data from the coupled simulation
        // Receive if necessary .Note that potentially more that one receive is possible
        ReceiveTimeData(ifc, time);
        DataOut.time = time - ifc->GetConnParams().Delay;

        ifc->GetTimeData(DataOut);
    }
    else {          //Monitoring = receive time data for output interface
        TLMInterfaceOutput* ifc = dynamic_cast<TLMInterfaceOutput*>(Interfaces[idx]);

        assert(ifc -> GetInterfaceID() == interfaceID);
        // Check if the interface expects more data from the coupled simulation
        // Receive if necessary .Note that potentially more that one receive is possible
        ReceiveTimeData(ifc, time);
        DataOut.time = time - ifc->GetConnParams().Delay;

        ifc->GetTimeData(DataOut);
    }
}

void PluginImplementer::GetTimeData1D(int interfaceID, double time, TLMTimeData1D &DataOut) {
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterface1D* ifc = dynamic_cast<TLMInterface1D*>(Interfaces[idx]);
    assert(ifc -> GetInterfaceID() == interfaceID);

    // Check if the interface expects more data from the coupled simulation
    // Receive if necessary .Note that potentially more that one receive is possible
    ReceiveTimeData(ifc, time);

    DataOut.time = time - ifc->GetConnParams().Delay;

    ifc->GetTimeData(DataOut);
}

// GetTimeData returnes the necessary time stamped information needed
// for the calculation of the reaction force at a given time.
// The function might result in a request sent to TLM manager.
void PluginImplementer::GetTimeData3D(int interfaceID, double time, TLMTimeData3D& DataOut) {
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterface3D* ifc = dynamic_cast<TLMInterface3D*>(Interfaces[idx]);
    assert(ifc -> GetInterfaceID() == interfaceID);

    // Check if the interface expects more data from the coupled simulation
    // Receive if necessary .Note that potentially more that one receive is possible
    ReceiveTimeData(ifc, time);

    DataOut.time = time - ifc->GetConnParams().Delay;

    ifc->GetTimeData(DataOut);

}

void PluginImplementer::GetParameterValue(int parameterID, std::string &Name, std::string &Value) {
    int idx = GetParameterIndex(parameterID);
    ComponentParameter* pPar = Parameters[idx];
    Name = pPar->GetName();
    Value = pPar->GetValue();
}

