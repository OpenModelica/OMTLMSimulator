// File: PluginImplementer.cc
// Provides the implementation of methods for PluginImplementer.
#include "TLMPlugin.h"
#include "TLMCommUtil.h"
#include "PluginImplementer.h"
#include <cassert>
#include <iostream>
#include <csignal>

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


void signalHandler_( int signum )
{
  // Install default signal handler
  signal(signum, SIG_DFL);

  if( PluginImplementerInstance != 0){
    PluginImplementerInstance->signalHandler(signum);
  }

  // Call default handler.
  raise(signum);
}

PluginImplementer::PluginImplementer():
  Connected(false),
  ModelChecked(false),
  Interfaces(),
  ClientComm(),
  Message(),
  MapID2Ind(),
  StartTime(0.0),
  EndTime(0.0),
  MaxStep(0.0)
{    
  // Install out own signal handler.
  signal(SIGABRT, signalHandler_);
  signal(SIGFPE, signalHandler_);
  signal(SIGILL, signalHandler_);
  signal(SIGINT, signalHandler_);
  signal(SIGSEGV, signalHandler_);
  signal(SIGTERM, signalHandler_);
}


PluginImplementer::~PluginImplementer() {

  for(vector<TLMInterface*>::iterator it = Interfaces.begin();
      it != Interfaces.end(); ++it) {
    delete (*it);
  }
}

void PluginImplementer::signalHandler( int signum )
{
  if(Connected) {
    Message.Header.MessageType = TLMMessageTypeConst::TLM_ABORT;
    TLMCommUtil::SendMessage(Message);
  }

  TLMErrorLog::Log("Got signal " + TLMErrorLog::ToStdStr(signum));
}

void PluginImplementer::CheckModel() {

  if(!Connected) {
    TLMErrorLog::FatalError("Check model cannot be called before the TLM client is connected to manager");
  }

  Message.Header.MessageType =  TLMMessageTypeConst::TLM_CHECK_MODEL;

  TLMCommUtil::SendMessage(Message);
  TLMCommUtil::ReceiveMessage(Message);

  if(! Message.Header.TLMInterfaceID) {
    TLMErrorLog::Log("Error detected on TLM manager while checking meta model");
    TLMErrorLog::FatalError("Header id is " + TLMErrorLog::ToStdStr(int(Message.Header.TLMInterfaceID)));
  }

  ModelChecked = true;
}


// Init method. Should be called after the default constructor. It will
// initialize the object and connect to TLMManager. Will return true
// on success, false otherwize. Note that the method can be called
// only once.
bool PluginImplementer::Init( std::string model,
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

  if((Message.SocketHandle = ClientComm.ConnectManager(host, port)) < 0) {
    TLMErrorLog::Warning("Init failed: could not connect to TLM manager");
    return false;
  }

  TLMErrorLog::Log("Sending Component registration request");

  ClientComm.CreateComponentRegMessage(model, Message);
  TLMCommUtil::SendMessage(Message);
  TLMCommUtil::ReceiveMessage(Message);

  TLMErrorLog::Log(string("Got component ID: ") +
                   TLMErrorLog::ToStdStr(Message.Header.TLMInterfaceID));

  StartTime = timeStart;
  EndTime = timeEnd;
  MaxStep = maxStep;

  Connected = true;
  SetInitialized();

  return true;
}


// Register TLM interface sends a registration request to TLMManager
// and returns the ID for the interface. '-1' is returned if
// the interface is not connected in the MetaModel.
int  PluginImplementer::RegisteTLMInterface( std::string name , std::string type ) {
    TLMErrorLog::Log(string("Register Interface (kanin) ") + name );

    TLMInterface *ifc;
    if(type=="3D") {
        TLMErrorLog::Log("Registers TLM interface of type 3D");
        ifc = new TLMInterface3D( ClientComm, name, StartTime );
    }
    else if(type == "1D") {
        TLMErrorLog::Log("Registers TLM interface of type 1D");
        ifc = new TLMInterface1D( ClientComm, name, StartTime );
    }
    else if(type == "SignalInput") {
        TLMErrorLog::Log("Registers TLM interface of type SignalInput");
        ifc = new TLMInterfaceSignal( ClientComm, name, StartTime, Input );
    }
    else if(type == "SignalOutput") {
        TLMErrorLog::Log("Registers TLM interface of type SignalOutput");
        ifc = new TLMInterfaceSignal( ClientComm, name, StartTime, Output );
    }
    else {
        TLMErrorLog::FatalError("Unknown interface type.");
    }

    int id = ifc->GetInterfaceID();

    TLMErrorLog::Log(string("Got interface ID: ") + TLMErrorLog::ToStdStr(id));

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


// ReceiveTimeData receives time-stamped data from coupled simulations.
// Since the order of messages can vary the specified interfaceID
// is used only to detect the last message expected when the function
// is called. Any number of messages to other interfaces can arrive
// before the desired message is received.
// Input:
//   interfaceID - ID of a TLM interface that triggered the request
void PluginImplementer::ReceiveTimeData(TLMInterface* reqIfc, double time)  {
  while(time > reqIfc->GetNextRecvTime()) { // while data is needed

    // Receive data untill there is info for this interface
    string mess("Interface ");
    TLMErrorLog::Log(mess + reqIfc->GetName() +
                     " needs data for time= " + TLMErrorLog::ToStdStr(time));

    double allowedMaxTime = reqIfc->GetLastSendTime() + reqIfc->GetConnParams().Delay;

    if(allowedMaxTime < time && reqIfc->GetType() != "Signal") {
      string mess("WARNING: Interface ");
      TLMErrorLog::Log(mess + reqIfc->GetName() +
                       " is NOT ALLOWED to ask data after time= " + TLMErrorLog::ToStdStr(allowedMaxTime) +
                       ". The error is: "+TLMErrorLog::ToStdStr(time - allowedMaxTime));
      break;
    }

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

    TLMErrorLog::Log(string("Got data until time=") +
                     TLMErrorLog::ToStdStr(ifc->GetNextRecvTime()) );
  }
}

void PluginImplementer::GetValueSignal(int interfaceID, double time, double *value) {
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterfaceSignal* ifc = dynamic_cast<TLMInterfaceSignal*>(Interfaces[idx]);

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
    ifc->SetTimeData(time, *value);  //We need to write something as well
}

void PluginImplementer::GetForce1D(int interfaceID, double time, double position, double speed, double *force) {
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
  ifc->GetForce(time, position, speed, force);
}


void PluginImplementer::GetForce3D(int interfaceID,
                                   double time,
                                   double position[],
                                   double orientation[],
                                   double speed[],
                                   double ang_speed[],
                                   double* force)  {

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
  ReceiveTimeData( ifc, time);

  // evaluate the reaction force from the TLM connection
  ifc->GetForce(time, position, orientation, speed, ang_speed, force);
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

  if( !ifc->waitForShutdown() ){
    // Store the data into the interface object
    TLMErrorLog::Log(string("calling SetTimeData()"));
    ifc->SetTimeData(time, position, orientation,speed,ang_speed);
  }
  else {
    // Check if all interfaces wait for shutdown
    std::vector<TLMInterface*>::iterator iter;
    for( iter=Interfaces.begin() ; iter!=Interfaces.end() ; iter++ ){
      if( ! (*iter)->waitForShutdown() ) return;
    }
#ifdef _MSC_VER
    WSACleanup(); // BZ306 fixed here
#else
    // needed anything ?
#endif

    TLMErrorLog::Log(string("Takedown due to finished interface data request."));

    // If we got here, we have a shutdown request from all interfaces
    //abort(); // Some systems don't handle exit() very well, let's try abort();
    // abort creates core dump!
    exit(0);
  }
}


void PluginImplementer::SetValueSignal(int valueID,
                                       double time,
                                       double value) {
    if(!ModelChecked) CheckModel();
    if(valueID < 0) return;
    // Find the interface object by its ID
    int idx = GetInterfaceIndex(valueID);
    TLMInterfaceSignal* ifc = dynamic_cast<TLMInterfaceSignal*>(Interfaces[idx]);
    assert(ifc -> GetInterfaceID() == valueID);

    if( !ifc->waitForShutdown() ){
      // Store the data into the interface object
      TLMErrorLog::Log(string("calling SetTimeData()"));
      ifc->SetTimeData(time, value);
    }
    else {
      // Check if all interfaces wait for shutdown
      std::vector<TLMInterface*>::iterator iter;
      for( iter=Interfaces.begin() ; iter!=Interfaces.end() ; iter++ ){
        if( ! (*iter)->waitForShutdown() ) return;
      }
  #ifdef _MSC_VER
      WSACleanup(); // BZ306 fixed here
  #else
      // needed anything ?
  #endif

      TLMErrorLog::Log(string("Takedown due to finished interface data request."));

      // If we got here, we have a shutdown request from all interfaces
      //abort(); // Some systems don't handle exit() very well, let's try abort();
      // abort creates core dump!
      exit(0);
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

  if( !ifc->waitForShutdown() ){
    // Store the data into the interface object
    TLMErrorLog::Log(string("calling SetTimeData()"));
    ifc->SetTimeData(time, position, speed);
  }
  else {
    // Check if all interfaces wait for shutdown
    std::vector<TLMInterface*>::iterator iter;
    for( iter=Interfaces.begin() ; iter!=Interfaces.end() ; iter++ ){
      if( ! (*iter)->waitForShutdown() ) return;
    }
#ifdef _MSC_VER     
    WSACleanup(); // BZ306 fixed here
#else
    // needed anything ?
#endif     

    TLMErrorLog::Log(string("Takedown due to finished interface data request."));

    // If we got here, we have a shutdown request from all interfaces
    //abort(); // Some systems don't handle exit() very well, let's try abort();
    // abort creates core dump!
    exit(0);
  }
}

// GetConnectionParams returnes the ConnectionParams for
// the specified interface ID. Interface must be registered
// first.
void PluginImplementer::GetConnectionParams(int interfaceID, TLMConnectionParams& ParamsOut) {

  // Use the ID to get to the right interface object
  int idx = GetInterfaceIndex(interfaceID);
  TLMInterface* ifc = Interfaces[idx];
  assert(ifc -> GetInterfaceID() == interfaceID);

  ParamsOut = ifc->GetConnParams();
}

void PluginImplementer::GetTimeDataSignal(int interfaceID, double time, TLMTimeDataSignal &DataOut) {
    if(!ModelChecked) CheckModel();

    // Use the ID to get to the right interface object
    int idx = GetInterfaceIndex(interfaceID);
    TLMInterfaceSignal* ifc = dynamic_cast<TLMInterfaceSignal*>(Interfaces[idx]);
    assert(ifc -> GetInterfaceID() == interfaceID);

    // Check if the interface expects more data from the coupled simulation
    // Receive if necessary .Note that potentially more that one receive is possible
    ReceiveTimeData( ifc, time);

    DataOut.time = time - ifc->GetConnParams().Delay;

    ifc->GetTimeData(DataOut);
}

void PluginImplementer::GetTimeData1D(int interfaceID, double time, TLMTimeData1D &DataOut) {
  if(!ModelChecked) CheckModel();

  // Use the ID to get to the right interface object
  int idx = GetInterfaceIndex(interfaceID);
  TLMInterface1D* ifc = dynamic_cast<TLMInterface1D*>(Interfaces[idx]);
  assert(ifc -> GetInterfaceID() == interfaceID);

  // Check if the interface expects more data from the coupled simulation
  // Receive if necessary .Note that potentially more that one receive is possible
  ReceiveTimeData( ifc, time);

  DataOut.time = time - ifc->GetConnParams().Delay;

  ifc->GetTimeData(DataOut);
}

// GetTimeData returnes the necessary time stamped information needed
// for the calculation of the reaction force at a given time.
// The function might result in a request sent to TLM manager.
void PluginImplementer::GetTimeData3D(int interfaceID, double time, TLMTimeData3D& DataOut){
  if(!ModelChecked) CheckModel();

  // Use the ID to get to the right interface object
  int idx = GetInterfaceIndex(interfaceID);
  TLMInterface3D* ifc = dynamic_cast<TLMInterface3D*>(Interfaces[idx]);
  assert(ifc -> GetInterfaceID() == interfaceID);

  // Check if the interface expects more data from the coupled simulation
  // Receive if necessary .Note that potentially more that one receive is possible
  ReceiveTimeData( ifc, time);

  DataOut.time = time - ifc->GetConnParams().Delay;

  ifc->GetTimeData(DataOut);

}

