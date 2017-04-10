//!
//!  \file ManagerCommHandler.h
//!
//!  ManagerCommHandler classes definition (TLMManager communication logic)
//!
//!

#ifndef ManagerCommHandler_h_
#define ManagerCommHandler_h_

#include <string>
#include <map>
// note: <map> must be above all, because of a VC2005 bug (on _Wherenode)

#include "Communication/TLMCommUtil.h"
#include "Communication/TLMManagerComm.h"
#include "Communication/TLMMessageQueue.h"
#include "CompositeModels/CompositeModel.h"

#include "TLMThreadSynch.h"

#ifndef _MSC_VER
#include <unistd.h>
#endif

//! \class ManagerCommHandler
//! ManagerCommHandler class implements the communication protocol 
//! It uses the classes defined in TLMManagerComm.h
class ManagerCommHandler {
private:
    //! send message queue
    TLMMessageQueue MessageQueue;

    //! Communication object
    TLMManagerComm Comm;

    //! Meta-model
    CompositeModel& TheModel;

    bool MonitorConnected;

public:
    //! The communication protocol modes, i.e., real co-simulation or interface information request.
    enum CommunicationMode { CoSimulationMode, InterfaceRequestMode };

private:
    //! The mode of communication either real co-simulation or interface information request.
    CommunicationMode CommMode;

    //! The multimap to store monitoring interface sockets.
    std::multimap<int,int> monitorInterfaceMap;

    //! The multimap mutex for synchronisation of "monitorInterfaceMap" access
    SimpleLock monitorMapLock;

public:
    //! The current running mode. Mainly used for monitoring.
    enum RunningMode{ StartUpMode, RunMode, ShutdownMode };

private:
    //! The current running mode.
    RunningMode runningMode;

    //! Exception message
    std::string exceptionMsg;

    //! Lock for setting exception message.
    SimpleLock exceptionLock;

public:
    //! Constructor.
    ManagerCommHandler(CompositeModel& Model):
        MessageQueue(),
        Comm(Model.GetComponentsNum(), Model.GetSimParams().GetPort()),
        TheModel(Model),
        MonitorConnected(false),
        CommMode(CoSimulationMode),
        monitorInterfaceMap(),
        monitorMapLock(),
        runningMode(StartUpMode),
        exceptionMsg(""),
        exceptionLock()
    {
    }

    //! Run method executes all the protocols in the right order:
    //! Startup, Check then Simulate
    void Run(CommunicationMode CommMode_In = CoSimulationMode);


    //! Forward start to the particular object
    static void* thread_ReaderThreadRun(void * arg) {
        ManagerCommHandler* con = (ManagerCommHandler*)arg;

        if(con->TheModel.GetSimParams().GetMonitorPort() > 0) {
            while(!con->MonitorConnected) {
#ifndef _MSC_VER
                usleep(10000); // micro seconds
#else
                Sleep(10); // milli seconds
#endif
                TLMErrorLog::Log("Waiting for monitor to connect");
            }
        }

        try {
            con->ReaderThreadRun();
        }
        catch(std::string& msg) {
            con->HandleThreadException(msg);
        }
        catch(...) {
            con->HandleThreadException("Manager run thread caught exception");
        }
        return NULL;
    }

    //! RunStartupProtocol implements startup protocol that
    //! enables client registration at the manager
    void RunStartupProtocol();
    

    //! ProcessRegComponentMessage processes the first message after "accept"
    //! It is expected to be a component registration message.
    //! The functions associates the socket handle with the component in the CompositeModel.
    //! It then prepares the reply by setting TLMInterfaceID to component ID.
    void ProcessRegComponentMessage(TLMMessage& mess);

    //! ProcessRegInterfaceMessage processes a TLMInterface registration message from a client.
    //! It finds the appropriate proxy, sets its status to "connected"
    //! and prepares a reply message with interface ID and connection parameters.
    //! Note, that it's ok to try to register an interface not defined
    //! in the model. It'll just mean that no information will be sent to/from it.
    void ProcessRegInterfaceMessage(int compID, TLMMessage& mess);

    void ProcessRegParameterMessage(int compID, TLMMessage& mess);

    //! ReaderThreadRun processes incomming messages and creates
    //! messages to be sent.
    void ReaderThreadRun();

    //! Forward start to the particular object
    static void* thread_WriterThreadRun(void * arg) {
        ManagerCommHandler* con = (ManagerCommHandler*)arg;

        if(con->TheModel.GetSimParams().GetMonitorPort() > 0) {
            while(!con->MonitorConnected) {
#ifndef _MSC_VER
                usleep(10000); // micro seconds
#else
                Sleep(10); // milli seconds
#endif
                TLMErrorLog::Log("Waiting for monitor to connect");
            }
        }

        try {
            con->WriterThreadRun();
        }
        catch(std::string& msg) {
            con->HandleThreadException(msg);
        }
        catch(...) {
            con->HandleThreadException("Manager writer thread caught exception");
        }
        return NULL;
    }

    //! Send out messages in a separate thread
    void WriterThreadRun();

    //! Marshal time stamped message to the right client
    void MarshalMessage(TLMMessage& message);


    //! Forward start to the particular object
    static void* thread_MonitorThreadRun(void * arg) {
        ManagerCommHandler* con = (ManagerCommHandler*)arg;
        try {
            con->MonitorThreadRun();
        }
        catch(std::string& msg) {
            con->HandleThreadException(msg);
        }
        catch(...) {
            con->HandleThreadException("Manager monitor thread caught exception");
        }

        return NULL;
    }

    //! Initialize and run the monitoring thread.
    void MonitorThreadRun();

    //! Get the current running state.
    RunningMode getRunState() { return runningMode; }

    //! Check if we got an exception and return exception message.
    //! \param[out] msg The exception message, or empty string if no exception occured.
    //! \return True if an exception occured, false otherwise.
    bool GotException(std::string &msg);

private:
    //! Setup interface connection message.
    //! Used in ProcessRegInterfaceMessage(...).
    void SetupInterfaceConnectionMessage(int IfcID, std::string& aName, TLMMessage& mess);

    //! Setup interface connection message for data request mode.
    //! Used in ProcessRegInterfaceMessage(...).
    void SetupInterfaceRequestMessage(TLMMessage& mess);

    //! Extracts the time data from message and stores it in the
    //! corresponding meta-model interface proxy.
    void UnpackAndStoreTimeData(TLMMessage& mess);

    //! Process interface monitoring requests.
    //! Each TLM interface might be monitored by one or several
    //! external processes.
    int ProcessInterfaceMonitoringMessage(TLMMessage& message);

    //! Forwards message to monitoring ports if necessary.
    void ForwardToMonitor(TLMMessage& message);

    //! Thread exception handler.
    //! Shuts down all communications and sets the exception message.
    //! Invoked by threads.
    void HandleThreadException(const std::string& msg);
};

#endif
