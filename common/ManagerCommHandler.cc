#include "ManagerCommHandler.h"
#include "strConv.h"
#include <iostream>
#ifdef USE_THREADS
#include <pthread.h>
#endif
#include <cassert>

#include <cstdlib>
#include "timing.h"

#ifndef _MSC_VER
#include <unistd.h> 
#endif

using std::string;
using std::cerr;
using std::endl;
using std::multimap;

// Run method executes all the protocols in the right order:
// Startup, Check then Simulate
void ManagerCommHandler::Run(CommunicationMode CommMode_In) {
    CommMode = CommMode_In;

#ifdef USE_THREADS
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr,  PTHREAD_SCOPE_SYSTEM);
    pthread_t reader, writer;

#if 1
    pthread_t monitor;
    if( CommMode == CoSimulationMode ){
        pthread_create( &monitor, &attr, thread_MonitorThreadRun, (void*)this);
    }
#endif

    // start the reader & writer threads
    pthread_create( &reader, &attr, thread_ReaderThreadRun, (void*)this);

    pthread_create( &writer, &attr, thread_WriterThreadRun, (void*)this);

#if 1
    if( CommMode == CoSimulationMode ){
        pthread_join(monitor, NULL);
    }
#endif
    pthread_join(reader, NULL);
    pthread_join(writer, NULL);
#endif

    if(exceptionMsg.size() > 0){
        throw(exceptionMsg);
    }

}

//! Thread exception handler.
void ManagerCommHandler::threadException(const std::string &msg)
{
    exceptionLock.lock();

    exceptionMsg += msg + "\n";

    // Terminate message queue, this will unblock the writer thread if needed.
    MessageQueue.Terminate();

    // We close all sockets on exception.
    Comm.closeAll();

    exceptionLock.unlock();
}

bool ManagerCommHandler::gotException(std::string &msg)
{
    msg = exceptionMsg;
    return (msg.size() > 0);
}

// RunStartupProtocol implements startup protocol that
// enables client registration at the manager
void ManagerCommHandler::RunStartupProtocol() {
    // Number of components that are expected to register
    int numToRegister = TheModel.GetComponentsNum();
    // Number of components waiting for check model reply
    int numCheckModel = 0;

    // Server socket is used to accept connections
    int acceptSocket = Comm.CreateServerSocket();
    
    // Update the meta-model with the selected server port.
    TheModel.GetSimParams().SetPort(Comm.GetServerPort());    
    
    // Start the external components forming "coupled simulation"
    TheModel.StartComponents();

    TLMErrorLog::Log("Components were started, waiting for registration requests");
    Comm.AddActiveSocket(acceptSocket);
    
    // Setup timer
    tTM_Info tInfo;
    TM_Init(&tInfo);
    TM_Start(&tInfo);

    while( (numToRegister > 0) || ( numCheckModel < TheModel.GetComponentsNum())) {
        Comm.SelectReadSocket();
        
        // Check for timeout.
        TM_Stop(&tInfo);
        if( tInfo.total.tv_sec > TheModel.GetSimParams().GetTimeout() ){
            Comm.closeAll();
            TLMErrorLog::FatalError("Timeout - failed to start all components, give up! (" 
                                    + TLMErrorLog::ToStdStr(tInfo.total.tv_sec) 
                                    + " > " + TLMErrorLog::ToStdStr(TheModel.GetSimParams().GetTimeout()) 
                                    + ")" );            
            break;
        }
        // Restart is needed for correct time accumulation.
        TM_Start(&tInfo);
        
        TLMErrorLog::Log("Communicating with clients...");
        
        Comm.ClearActiveSockets();
        // Check all the components for interface registration messages
        for(int iSock =  TheModel.GetComponentsNum() - 1; iSock >= 0 ; --iSock) {
            TLMComponentProxy& comp =  TheModel.GetTLMComponentProxy(iSock);
            int hdl = comp.GetSocketHandle();
            // The component needs to be connected to a socket
            if(hdl < 0) continue;
            // The component needs to be is startup mode, not waiting for check mode
            if(comp.GetReadyToSim()) continue;

            // There is data waiting
            if(!Comm.HasData(hdl)) {
                Comm.AddActiveSocket(hdl); // expect more messages
                continue;
            }

            TLMMessage* message = MessageQueue.GetReadSlot();
            message->SocketHandle = hdl;
            TLMCommUtil::ReceiveMessage(*message);

            if(message->Header.MessageType ==  TLMMessageTypeConst::TLM_CHECK_MODEL) {
                // This component is done with registration. It's will wait for others
                TLMErrorLog::Log(string("Component ") + comp.GetName() + " is ready to simulation");;

                comp.SetReadyToSim();
                numCheckModel++;
            }
            else {
                TLMErrorLog::Log(string("Component ") + comp.GetName() + " registers interface");;

                Comm.AddActiveSocket(hdl); // expect more messages
                ProcessRegInterfaceMessage(iSock, *message);
                MessageQueue.PutWriteSlot(message);
            }
        }

        // Check if a new connection is waiting to be accepted.
        if((numToRegister > 0) && Comm.HasData(acceptSocket)) {
            int hdl = Comm.AcceptComponentConnections();
            // WARNING!!! This is potentially a problematic case
            // since I immediately try to receive a message from just accepted connection
            // and might block in 'read' while other clients wait.
            // Alternatively I could put the sockets that are not associated
            // with any component to a separate place and "select" on them once more.
            // Might be necessary to fix later.

            TLMMessage* message = MessageQueue.GetReadSlot();
            message->SocketHandle = hdl;
            if( !TLMCommUtil::ReceiveMessage(*message) ){
                TLMErrorLog::FatalError("Failed to get message, exiting");
                abort();
            }

            ProcessRegComponentMessage(*message);

            MessageQueue.PutWriteSlot(message);
            numToRegister --;
            if(numToRegister == 0)
                TLMErrorLog::Log("All expected components are registered");

            Comm.AddActiveSocket(hdl);
        }

        if(numToRegister)  // still more connections expected
            Comm.AddActiveSocket(acceptSocket);
        
    }
}

// ProcessRegComponentMessage processes the first message after "accept"
// It is expected to be a component registration message.
// The functions associates the socket handle with the component in the MetaModel.
// It then prepares the reply by setting TLMInterfaceID to component ID.
void ManagerCommHandler::ProcessRegComponentMessage( TLMMessage& mess) {

    if(mess.Header.MessageType !=  TLMMessageTypeConst::TLM_REG_COMPONENT) {
	TLMErrorLog::FatalError("Component registration message expected");
    }

    string aName ((const char*)(& mess.Data[0]), mess.Header.DataSize);

    int CompID = TheModel.GetTLMComponentID(aName);

    if( CompID < 0 || CompID >= TheModel.GetComponentsNum() ){
	TLMErrorLog::FatalError("Component registration for " + aName + " failed!");
        return;
    }

    TLMComponentProxy& comp = TheModel.GetTLMComponentProxy(CompID);

    comp.SetSocketHandle(mess.SocketHandle);

    mess.Header.DataSize = 0;

    mess.Header.TLMInterfaceID = CompID;
    
    TLMErrorLog::Log(string("Component ") + aName + " is connected");
	
}

// ProcessRegInterfaceMessage processes a TLMInterface registration message from a client.
// It finds the appropriate proxy, sets its status to "connected"
// and prepares a reply message with interface ID and connection parameters.
// Note, that it's ok to try to register an interface not defined
// in the model. It'll just mean that no information will be sent to/from it.
void ManagerCommHandler::ProcessRegInterfaceMessage(int compID, TLMMessage& mess) {
    if(mess.Header.MessageType != TLMMessageTypeConst::TLM_REG_INTERFACE) {
        //std::cerr << "wrong message is: " <<  mess.Header.MessageType << endl;
        //std::cerr << "wrong message is: " <<  int(mess.Header.MessageType) << endl;
	TLMErrorLog::FatalError("Interface registration message expected");	
    }

    // First, find the interface in the meta model
    string aName ((const char*)(& mess.Data[0]), mess.Header.DataSize);

    int IfcID = TheModel.GetTLMInterfaceID(compID, aName);    

    mess.Header.TLMInterfaceID = IfcID;

    mess.Header.SourceIsBigEndianSystem = TLMMessageHeader::IsBigEndianSystem;
    mess.Header.DataSize = 0;

    if(IfcID < 0 && CommMode == InterfaceRequestMode) {
        // interface not found, create it
        TheModel.RegisterTLMInterfaceProxy(compID, aName);
        IfcID = TheModel.GetTLMInterfaceID(compID, aName);        
    }

    if(IfcID < 0) {
        // interface not found
        TLMErrorLog::Warning(string("Interface ") + 
                             TheModel.GetTLMComponentProxy(compID).GetName() + '.'
                             + aName + " not defined in metamodel. Ignored.");
        return;
    }

    if( CommMode == CoSimulationMode ){
        SetupInterfaceConnectionMessage(IfcID, aName, mess);
    }
    else if( CommMode == InterfaceRequestMode ){
        
        TLMErrorLog::Log(string("Register TLM interface ") + 
                         TheModel.GetTLMComponentProxy(compID).GetName() + '.' + aName);
        
        mess.Header.TLMInterfaceID = IfcID;
        
        TLMInterfaceProxy& ifc = TheModel.GetTLMInterfaceProxy(IfcID);
        ifc.SetConnected();

        SetupInterfaceRequestMessage(mess);        
    }
    else {
        TLMErrorLog::Warning("Wrong coomunication mode in ManagerCommHandler::ProcessRegInterfaceMessage(...)");
        return;
    }
}

void ManagerCommHandler::SetupInterfaceConnectionMessage(int IfcID, std::string& aName, TLMMessage& mess)
{    
    // set the connected flag in the MetaModel
    TLMInterfaceProxy& ifc = TheModel.GetTLMInterfaceProxy(IfcID);
    ifc.SetConnected();

    // Find the connection object if exists

    int connID = ifc.GetConnectionID();
    if(connID < 0){
	// interface is not connected in the meta-model
        mess.Header.TLMInterfaceID = -1;
        return;
    }

    TLMErrorLog::Log(string("Interface ") + aName + " is connected");

    // Put connection parameters in the reply
    TLMConnection& conn = TheModel.GetTLMConnection (connID);

    TLMConnectionParams& param = conn.GetParams();

    // Apply component transformation for each interface.
    int CompId = ifc.GetComponentID();
    TheModel.GetTLMComponentProxy(CompId).GetInertialTranformation(param.cX_R_cG_cG, param.cX_A_cG);

    // Send initial interface position
    TLMTimeData& td = ifc.getTime0Data();
    for(int i=0 ; i<3 ; i++ ) param.Nom_cI_R_cX_cX[i] = td.Position[i];
    for(int i=0 ; i<9 ; i++ ) param.Nom_cI_A_cX[i] = td.RotMatrix[i];

    mess.Header.DataSize = sizeof (TLMConnectionParams);

    mess.Data.resize(sizeof (TLMConnectionParams));

    memcpy(& mess.Data[0], &param, mess.Header.DataSize);

}

void ManagerCommHandler::SetupInterfaceRequestMessage(TLMMessage& mess)
{
        
    TLMConnectionParams param;
    param.Delay = 0.1;
    param.mode = 1;

    mess.Header.DataSize = sizeof (TLMConnectionParams);
    mess.Data.resize(sizeof (TLMConnectionParams));
    memcpy(& mess.Data[0], &param, mess.Header.DataSize);
    
}


// ReaderThreadRun processes incomming messages and creates
// messages to be sent.
void ManagerCommHandler::ReaderThreadRun() {

    // Handle start-up
    RunStartupProtocol();

    // Check that startup completed correctly
    int StartupOK = TheModel.CheckProxyComm();
    
    // Send the status result to all components
    for(int iSock =  TheModel.GetComponentsNum() - 1; iSock >= 0 ; --iSock) {
        int hdl = TheModel.GetTLMComponentProxy(iSock).GetSocketHandle();
        TLMMessage* message = MessageQueue.GetReadSlot();
        message->SocketHandle = hdl;
        message->Header.MessageType = TLMMessageTypeConst::TLM_CHECK_MODEL;
        message->Header.DataSize = 0;
        message->Header.TLMInterfaceID = StartupOK;
        MessageQueue.PutWriteSlot(message);
    }

    if(!StartupOK) {
        MessageQueue.Terminate();
        return;
    }

    TLMErrorLog::Log("------------------  Starting time data exchange   ------------------");
    
    Comm.SwitchToRunningMode();
    runningMode = RunMode;

    int nClosedSock = 0;

    while(nClosedSock != TheModel.GetComponentsNum()) {
        Comm.SelectReadSocket(); // wait for a change

        for(int iSock =  TheModel.GetComponentsNum() - 1; iSock >= 0 ; --iSock) {
            TLMComponentProxy& comp = TheModel.GetTLMComponentProxy(iSock);
            int hdl = comp.GetSocketHandle();

            if((hdl != -1 ) && Comm.HasData(hdl)) { // there is data to be received on the socket

                TLMMessage* message = MessageQueue.GetReadSlot();
                message->SocketHandle = hdl;
                if(TLMCommUtil::ReceiveMessage(*message)) {
                    if( CommMode == CoSimulationMode ){
                        MarshalMessage(*message);

                        // Forward message for monitoring.
                        ForwardToMonitor(*message);

                        // Place in send buffer
                        MessageQueue.PutWriteSlot(message);
                    }
                    else {
                        // CommMode == InterfaceRequestMode
                        UnpackAndStoreTimeData(*message);
                    }
                }
                else {
                    TLMErrorLog::Log(string("Connection to component ") + comp.GetName() + " is closed");
                    Comm.DropActiveSocket(hdl);
                    comp.SetSocketHandle(-1);
                    nClosedSock ++;
                }
            }
        }
    }
    TLMErrorLog::Log("All sockets are closed - exiting");
    runningMode = ShutdownMode;
    MessageQueue.Terminate();

    Comm.closeAll();
}

void ManagerCommHandler::WriterThreadRun() {

    TLMMessage* tlm_mess = 0;
    TLMErrorLog::Log(string("TLM manager is ready to send messages"));

    while(( tlm_mess = MessageQueue.GetWriteSlot()) != NULL) {
        TLMCommUtil::SendMessage(*tlm_mess);
        //TLMMessage &mm = *tlm_mess;
        //TLMCommUtil::SendMessage(mm);
        MessageQueue.ReleaseSlot(tlm_mess);
    }
    
}


void ManagerCommHandler::MarshalMessage(TLMMessage& message) {

    if(message.Header.MessageType !=   TLMMessageTypeConst::TLM_TIME_DATA) {
        TLMErrorLog::FatalError("Unexpected message received " + tlmMisc::ToStr(message.Header.MessageType));
    };

    // forward the time data
    TLMInterfaceProxy& src = TheModel.GetTLMInterfaceProxy(message.Header.TLMInterfaceID);
    int destID = src.GetLinkedID();

    if(destID < 0) {
        TLMErrorLog::Warning("Received time data for an unconnected interface. Ignored.");
        message.SocketHandle = -1;
        message.Header.TLMInterfaceID = -1;
    }
    else {
        TLMInterfaceProxy& dest = TheModel.GetTLMInterfaceProxy(destID);
        TLMComponentProxy& destComp = TheModel.GetTLMComponentProxy(dest.GetComponentID());
        message.SocketHandle = destComp.GetSocketHandle();
        message.Header.TLMInterfaceID = destID;

        if(TLMErrorLog::LogEnabled()){
            TLMErrorLog::Log(string("Forwarding from " +
                                    TheModel.GetTLMComponentProxy(src.GetComponentID()).GetName() + '.'+
                                    src.GetName()
                                    + " to " + destComp.GetName() + '.' + dest.GetName()));
        }
    }
}

void ManagerCommHandler::UnpackAndStoreTimeData(TLMMessage& message) 
{
    if(message.Header.MessageType !=   TLMMessageTypeConst::TLM_TIME_DATA) {
	TLMErrorLog::FatalError("Unexpected message received in ManagerCommHandler::UnpackAndStoreTimeData(...)");
    };

    // since mess.Data is continious we can just convert the pointer
    TLMTimeData* Next = (TLMTimeData*)(&message.Data[0]);

    // check if we have byte order missmatch in the message and perform
    // swapping if necessary
    bool switch_byte_order = 
	(TLMMessageHeader::IsBigEndianSystem != message.Header.SourceIsBigEndianSystem);
    if (switch_byte_order) 
	TLMCommUtil::ByteSwap(Next, sizeof(double),  message.Header.DataSize/sizeof(double));

    // forward the time data
    TLMInterfaceProxy& src = TheModel.GetTLMInterfaceProxy(message.Header.TLMInterfaceID);
    TLMTimeData& data = src.getTime0Data();

    TLMErrorLog::Log("Unpack and store time data for " + src.GetName() ); 
    
    data = *Next;    
}


int ManagerCommHandler::ProcessInterfaceMonitoringMessage(TLMMessage& message)
{
    if(message.Header.MessageType != TLMMessageTypeConst::TLM_REG_INTERFACE) {
        TLMErrorLog::FatalError("Interface monitoring registration message expected");
    }
    
    // First, find the interface in the meta model
    string aName ((const char*)(& message.Data[0]), message.Header.DataSize);            
    
    TLMErrorLog::Log( "Request for monitoring " + aName ); 

    // Here the full name, i.e., component.interface, is requered
    int IfcID = TheModel.GetTLMInterfaceID(aName);

    message.Header.TLMInterfaceID = IfcID;
    message.Header.SourceIsBigEndianSystem = TLMMessageHeader::IsBigEndianSystem;
    message.Header.DataSize = 0;
    
    if( IfcID < 0 ){
        TLMErrorLog::Warning( "In monitoring, interface " + aName + " is not connected." ); 
        return -1;
    }
    
    // Wait until interface registration is completet.
    TLMInterfaceProxy& ifc = TheModel.GetTLMInterfaceProxy(IfcID);
    while( !ifc.GetConnected() ){
#ifndef _MSC_VER
        usleep(10000); // micro seconds
#else
        Sleep(10); // milli seconds
#endif
    }



    string::size_type DotPos = aName.find('.');  // Component name is the part before '.'
    string IfcName = aName.substr(DotPos+1);    
    
    SetupInterfaceConnectionMessage(IfcID, IfcName, message);

    return IfcID;
}

void ManagerCommHandler::ForwardToMonitor(TLMMessage& message){
    monitorMapLock.lock();

    // We forward to the sender!
    TLMInterfaceProxy& ifc = TheModel.GetTLMInterfaceProxy(message.Header.TLMInterfaceID);
    int TLMInterfaceID = ifc.GetLinkedID();
    
    if( monitorInterfaceMap.count(TLMInterfaceID) > 0 ){
        
        if(message.Header.MessageType != TLMMessageTypeConst::TLM_TIME_DATA) {
            TLMErrorLog::FatalError("Unexpected message received in forward to monitor");
        };

        // Forward to all connected monitoring ports
        multimap<int,int>::iterator pos;
        for( pos = monitorInterfaceMap.lower_bound(TLMInterfaceID) ;
             pos != monitorInterfaceMap.upper_bound(TLMInterfaceID) ; 
             pos++ ){
            
            TLMErrorLog::Log( "Forwarding to monitor, interface " + TLMErrorLog::ToStdStr(TLMInterfaceID) 
                              + " on socket " + TLMErrorLog::ToStdStr(pos->second) ); 
            
            int hdl = pos->second;
            
            TLMMessage* newMessage = MessageQueue.GetReadSlot();

            newMessage->SocketHandle = hdl;
            memcpy(&newMessage->Header, &message.Header, sizeof(TLMMessageHeader));
            newMessage->Header.TLMInterfaceID = TLMInterfaceID;

            newMessage->Header.DataSize = message.Header.DataSize;
            newMessage->Data.resize(newMessage->Header.DataSize);	    
            
            memcpy(&newMessage->Data[0], &message.Data[0], newMessage->Header.DataSize);

            MessageQueue.PutWriteSlot(newMessage);
        }
    }
    else {
        TLMErrorLog::Log( "Nothing to forward for monitor interface " + TLMErrorLog::ToStdStr(TLMInterfaceID));
    }
    monitorMapLock.unlock();
}


void ManagerCommHandler::MonitorThreadRun()
{
    TLMErrorLog::Log("In monitoring");
    
    if( TheModel.GetSimParams().GetMonitorPort() <= 0 ){
        TLMErrorLog::Log("Monitoring disabled!");
        return;
    }
    
    TLMErrorLog::Log("Initialize monitoring port");

    // Create a connection for max. 10 clients.
    TLMManagerComm monComm(10, TheModel.GetSimParams().GetMonitorPort());

    // Server socket is used to accept connections
    int acceptSocket = monComm.CreateServerSocket();
    if( acceptSocket == -1 ){
        TLMErrorLog::FatalError("Failed to initialize monitoring socket");
        abort();
        //return;
    }

    if( TheModel.GetSimParams().GetMonitorPort() != monComm.GetServerPort() ){
        TLMErrorLog::Warning("Used monitoring port : " + TLMErrorLog::ToStdStr(monComm.GetServerPort()));
    }

    // Update the meta-model with the selected server port.
    TheModel.GetSimParams().SetMonitorPort(monComm.GetServerPort());

    // Never switch to running mode but use active sockets instead.
    monComm.AddActiveSocket(acceptSocket);

    TLMErrorLog::Log("Wait for monitoring connections...");
    
    std::vector<int> socks;

    std::multimap<int,int> localIntMap;

    //assert( runningMode == RunMode );
    while(runningMode != ShutdownMode){
        int hdl = -1;

        monComm.SelectReadSocket();
        
        // Just check if we are in shutdown mode
        if( runningMode == ShutdownMode ) break;

        if(monComm.HasData(acceptSocket)) {
            TLMErrorLog::Log("Got new monitoring connection");
            hdl = monComm.AcceptComponentConnections();
            if( hdl < 0 ){
                TLMErrorLog::FatalError("Failed to accept socket.");
                abort();        
            }
            monComm.AddActiveSocket(hdl);
            MonitorConnected = true;
            socks.push_back(hdl);            
        }
        else {
            for( std::vector<int>::iterator it=socks.begin() ; it != socks.end() ; it++ ){
                if(monComm.HasData(*it)) {
                    TLMErrorLog::Log("Accepted data on monitoring connection" );                    

                    hdl = *it;
                    break;
                }
            }
        }

        // Just check if we are in shutdown mode
        if( runningMode == ShutdownMode ) break;

        if( hdl >= 0 ){
            TLMMessage* message = MessageQueue.GetReadSlot();
            message->SocketHandle = hdl;
            
            if( !TLMCommUtil::ReceiveMessage(*message) ){
                TLMErrorLog::Warning("Failed to get message from monitor, disconected?");
                //abort();
                monComm.DropActiveSocket(hdl);
                continue;
            }
            
            if(message->Header.MessageType ==  TLMMessageTypeConst::TLM_CHECK_MODEL) {
                TLMErrorLog::Warning("Received unexpected check-model on monitoring interface, try to answer..."); 

                message->Header.TLMInterfaceID = 1;
                message->Header.DataSize = 0;

                MessageQueue.PutWriteSlot(message);
            }
            else {
                int IfcID = ProcessInterfaceMonitoringMessage(*message);
                MessageQueue.PutWriteSlot(message);
		
                if( IfcID >= 0 ){

                    TLMErrorLog::Log("Register monitor handle for interface " + tlmMisc::ToStr(IfcID));
#if 0
                    //std::cout << "hdl=" << hdl << ", ifID=" << IfcID << std::endl;
                    localIntMap.insert(std::make_pair(hdl, IfcID));
                    //std::cout << "hdl count=" << localIntMap.count(hdl) << ", if count=" << TheModel.GetInterfacesNum() << std::endl;

                    // NOTE, here we register interfaces first if all interfaces are monitored.
                    if(localIntMap.count(hdl) == TheModel.GetInterfacesNum()){
                        monitorMapLock.lock();
                        std::multimap<int,int>::iterator it;
                        for( it = localIntMap.lower_bound(hdl) ;
                             it != localIntMap.upper_bound(hdl) ;
                             it++ ){
                            monitorInterfaceMap.insert(std::make_pair(it->second, hdl));
                        }
                        monitorMapLock.unlock();
                    }
                    else{
                    }
#else
                    monitorMapLock.lock();
                    monitorInterfaceMap.insert(std::make_pair(IfcID, hdl));
                    monitorMapLock.unlock();
#endif

                }
            }
            //MessageQueue.PutWriteSlot(message);
        }
        else {
            // wait some time.
#ifndef _MSC_VER
            usleep(10000);
#else
            Sleep(10);
#endif
        }

    }

    // Close all sockets
    monComm.closeAll();
}
