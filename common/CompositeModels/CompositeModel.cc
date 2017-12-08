/**
 * File: CompositeModel.cc
 *
 * Implementation of methods for classes defined in CompositeModel.h
 */
#include <string>
#include <set>
#include <sstream>
#include <vector>
#include "CompositeModels/CompositeModel.h"
#include "Communication/TLMCommUtil.h"
//#include "portability.h"
#include <cstdlib>

#ifndef WIN32
#include <unistd.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#else
#include <process.h>
#include <winsock2.h>
#endif

using std::string;

// Get server name & port number in the form <server>:<port>
string SimulationParams::GetServerName() const {
#define MAXHOSTNAME 1024

    char Buf[MAXHOSTNAME + 50];
    gethostname(Buf, MAXHOSTNAME); // this sometimes return unreliable (short) names
    
    // getting IP
    struct hostent *hp;
    hp = gethostbyname(Buf);

    if(hp==NULL) {
        TLMErrorLog::FatalError("GetServerName: Failed to get my host IP");
        return string();
    }

    char* localIP;
    localIP = inet_ntoa (*(struct in_addr *)*hp->h_addr_list);

    ////////

    sprintf(Buf,"%s:%d", localIP, Port);
    return string(Buf);
}

// Constructor. 
// Input:
// aCompID - comonent ID of the owner
// IfcID - ID of this interface
// aName - name of this interface
// aType - type of this interface (3D, 1D, SignalInput or SignalOutput)
// aDomain - physical domain of this interface
TLMInterfaceProxy::TLMInterfaceProxy(int CompID, int IfcID, string& aName, int aDimensions,
                                     std::string aCausality, std::string aDomain) :
    InterfaceID(IfcID),
    ComponentID(CompID),
    ConnectionID(-1),
    LinkedID(-1),
    Name(aName),
    Dimensions(aDimensions),
    Causality(aCausality),
    Domain(aDomain),
    Connected(false),
    time0Data3D() {}


// Set the connection object attached to this interface.
void TLMInterfaceProxy::SetConnection(TLMConnection& conn) {
    ConnectionID = conn.GetID();
    LinkedID = (conn.GetFromID() == GetID()) ?
                conn.GetToID():conn.GetFromID();
}


//! Set position and orientation of the component inertial system relative the 
//! meta-models inertial system.
void TLMComponentProxy::SetInertialTranformation(double pos[], double orientation[]) {
    cX_R_cG_cG[0] = pos[0];
    cX_R_cG_cG[1] = pos[1];
    cX_R_cG_cG[2] = pos[2];
    
    cX_A_cG[0] = orientation[0];
    cX_A_cG[1] = orientation[1];
    cX_A_cG[2] = orientation[2];
    
    cX_A_cG[3] = orientation[3];
    cX_A_cG[4] = orientation[4];
    cX_A_cG[5] = orientation[5];
    
    cX_A_cG[6] = orientation[6];
    cX_A_cG[7] = orientation[7];
    cX_A_cG[8] = orientation[8];
}

//! Get position and orientation of the component inertial system relative the 
//! meta-models inertial system.
void TLMComponentProxy::GetInertialTranformation(double pos[3], double orientation[9]) {
    pos[0] = cX_R_cG_cG[0];
    pos[1] = cX_R_cG_cG[1];
    pos[2] = cX_R_cG_cG[2];
    
    orientation[0] = cX_A_cG[0];
    orientation[1] = cX_A_cG[1];
    orientation[2] = cX_A_cG[2];
    
    orientation[3] = cX_A_cG[3];
    orientation[4] = cX_A_cG[4];
    orientation[5] = cX_A_cG[5];
    
    orientation[6] = cX_A_cG[6];
    orientation[7] = cX_A_cG[7];
    orientation[8] = cX_A_cG[8];
}

#ifdef WIN32
// Constructor
CompositeModel::CompositeModel() {}
#else
void child_signal_handler(int s) {
    int pid, status;
    while(1)
    {
        // Catch all SIGCHLD signals
        pid = waitpid (WAIT_ANY, &status, WNOHANG);

        if(pid <= 0) {
            // No child found, we simply break.
            break;
        }

        if(status != 0) {
            // Here we get the actual error, typically the command could not be executed.
            TLMErrorLog::FatalError("Execution failed, please verify command (script), execution path, and check TLM logfile.");
        }
    }
}

// Constructor
CompositeModel::CompositeModel() {
    signal(SIGCHLD, child_signal_handler);
}
#endif

// Destructor
CompositeModel::~CompositeModel() {
    // Clean-up memory allocated by arrays
    {
        for(ComponentsVector::iterator i = Components.begin();
            i != Components.end(); ++i) {
            delete *i;
        }
    }
    {
        for(TLMInterfacesVector::iterator i = Interfaces.begin();
            i != Interfaces.end(); ++i) {
            delete *i;
        }
    }
    {
        for(ConnectionsVector::iterator i = Connections.begin();
            i != Connections.end(); ++i) {
            delete *i;
        }
  }
}

bool CompositeModel::CheckTheModel()
{
    TLMErrorLog::Info("Checking model...");

    bool abort=false;

    std::multiset<int> connectedInterfaceIDs;
    int numConnectedInterfaces = 0;

    int vecSize = Connections.size();

    for(int i=0; i<vecSize; ++i) {
        int from = Connections.at(i)->GetFromID();
        int to = Connections.at(i)->GetToID();

        TLMInterfaceProxy fromProxy = GetTLMInterfaceProxy(from);
        std::string fromInterfaceName = fromProxy.GetName();
        int fromComponent = fromProxy.GetComponentID();
        std::string fromComponentName = GetTLMComponentProxy(fromComponent).GetName();
        std::string fromName = fromComponentName+"."+fromInterfaceName;

        TLMInterfaceProxy toProxy = GetTLMInterfaceProxy(to);
        std::string toInterfaceName = toProxy.GetName();
        int toComponent = fromProxy.GetComponentID();
        std::string toComponentName = GetTLMComponentProxy(toComponent).GetName();
        std::string toName = toComponentName+"."+toInterfaceName;

        if(fromProxy.GetDomain() != toProxy.GetDomain()) {
           TLMErrorLog::Warning(fromName+" and "+toName+
                                " are connected but have different domains!");
        }

        connectedInterfaceIDs.insert(from);
        connectedInterfaceIDs.insert(to);

        numConnectedInterfaces += 2;

        if((fromProxy.GetCausality() == "Bidirectional" && toProxy.GetCausality() != "Bidirectional") ||
          (fromProxy.GetCausality() != "Bidirectional" && toProxy.GetCausality() == "Bidirectional") ||
          (fromProxy.GetCausality() == "Input" && toProxy.GetCausality() != "Output") ||
          (fromProxy.GetCausality() == "Output" && toProxy.GetCausality() != "Input")) {
          TLMErrorLog::Warning(fromName+" is connected to "+toName+
                               " with wrong causalities!");
          abort=true;
        }
    }

    for(int i=0; i<Interfaces.size(); ++i) {
        int id = Interfaces[i]->GetID();

        if(connectedInterfaceIDs.count(id) == 0) {
            int compId = Interfaces[i]->GetComponentID();
            std::string compName = GetTLMComponentProxy(compId).GetName();
            TLMErrorLog::Warning("Interface "+compName+"."+Interfaces[i]->GetName()+" is not connected!");
        }
        else if(connectedInterfaceIDs.count(id) > 1) {
            int compId = Interfaces[i]->GetComponentID();
            std::string compName = GetTLMComponentProxy(compId).GetName();
            TLMErrorLog::Warning("Interface "+compName+"."+Interfaces[i]->GetName()+" has multiple connections!");
            abort=true;
        }
    }

    if(abort) {
      TLMErrorLog::FatalError("Model contains errors and cannot be simulated.");
    }
}


// Add ComponentProxy to the model and return its ID.
int CompositeModel::RegisterTLMComponentProxy(const string& Name,
                                         const string& StartCommand,
                                         const string& ModelName,
                                         int SolverMode,
                                         const string& GeometryFile) {
    TLMComponentProxy* comp = new TLMComponentProxy(Name, StartCommand, ModelName, SolverMode, GeometryFile);
    Components.insert(Components.end(), comp);
    return Components.size() - 1;
}

// Find a Component by its name and return the ID
// Return -1 if not component was found.. 
int CompositeModel::GetTLMComponentID(const string& Name) {
    for(int i = Components.size() - 1; i >= 0; --i) {
        if(Components[i]->GetName() == Name) {
            return i;
        }
    }
    return -1;
}

int CompositeModel::GetTLMInterfaceID(string& FullName) {

    string::size_type DotPos = FullName.find('.');  // Component name is the part before '.'
    string ComponentName = FullName.substr(0, DotPos);
    
    int CompID = GetTLMComponentID(ComponentName);
    if(CompID < 0) return -1;

    string IfcName = FullName.substr(DotPos+1);
    return GetTLMInterfaceID(CompID, IfcName);
    
}

// Add TLM interface proxy with a given name to the Model, return its ID.
int CompositeModel::RegisterTLMInterfaceProxy(const int ComponentID, string& Name, int Dimensions,
                                         std::string Causality, std::string Domain) {
    TLMInterfaceProxy* ifc =
            new TLMInterfaceProxy(ComponentID, Interfaces.size(), Name, Dimensions, Causality, Domain);

    TLMErrorLog::Info("Registering interface proxy."
                     " Id = "+TLMErrorLog::ToStdStr(int(Interfaces.size()))+
                     ", ComponentId = "+TLMErrorLog::ToStdStr(ComponentID)+
                     ", Name = " + Name+
                     ", Dimensions = " + TLMErrorLog::ToStdStr(Dimensions)+
                     ", Causality = " + Causality+
                     ", Domain = " + Domain);

    Interfaces.insert(Interfaces.end(), ifc);
    return Interfaces.size()-1;
}

int CompositeModel::RegisterComponentParameterProxy(const int ComponentID, string& Name, string& DefaultValue) {
    ComponentParameterProxy* par = new ComponentParameterProxy(ComponentID, ComponentParameters.size(), Name, DefaultValue);

    TLMErrorLog::Info("Registering parameter proxy."
                     " Id = " + TLMErrorLog::ToStdStr(int(ComponentParameters.size()))+
                     ", ComponentId = "+TLMErrorLog::ToStdStr(ComponentID)+
                     ", Name = " + Name+
                     ", DefaultValue = " + DefaultValue);

    ComponentParameters.insert(ComponentParameters.end(), par);
    return ComponentParameters.size()-1;
}


// Find TLMInterface belonging to a given component (ID)
// with a specified name and return its ID.
int CompositeModel::GetTLMInterfaceID(const int ComponentID, string& Name) {
    for(int i = Interfaces.size() - 1; i >= 0; i--) {
        TLMInterfaceProxy& ifc =  GetTLMInterfaceProxy(i);
        if((ifc.GetComponentID() == ComponentID)
                && (ifc.GetName() == Name)) {
            return i;
        }
    }
    return -1;
}

int CompositeModel::GetComponentParameterID(const int ComponentID, std::string &Name) {
    for(int i = ComponentParameters.size() - 1; i >= 0; i--) {
        ComponentParameterProxy& ifc =  GetComponentParameterProxy(i);
        if((ifc.GetComponentID() == ComponentID)
                && (ifc.GetName() == Name)) {
            return i;
        }
    }
    return -1;
}


// Add a TLMConnection to the model.
// Input:
// ifc1, ifc2 - ID of the TLM interfaces the connection is attaching to.
// param - parameters of the Connection
int CompositeModel::RegisterTLMConnection(int ifc1, int ifc2, TLMConnectionParams& param) {
    TLMConnection* conn = new TLMConnection(Connections.size(), ifc1, ifc2, param);
    Connections.insert(Connections.end(), conn);
    return Connections.size() - 1;
}

// Start components
void CompositeModel::StartComponents() {
    for(unsigned i = 0; i < Components.size(); i++) {
        TLMErrorLog::Info(string("-----  Starting External Tool  ----- "));
        TLMErrorLog::Info("Name: "+Components[i]->GetName());
        double maxStep = 1e150;
        for(unsigned j = 0; j < Interfaces.size(); j++) {
            // check that the interface belongs to this component
            if((unsigned)Interfaces[j]->GetComponentID() != i) {
                TLMErrorLog::Info("Wrong component ID.");
                continue;
            }
            // check that interface is connected
            int conID = Interfaces[j]->GetConnectionID();
            if(conID < 0) {
                TLMErrorLog::Info("Interface not connected.");
                continue;
            }

            TLMErrorLog::Info("Found interface: "+Interfaces[j]->GetName());

            TLMConnection& conn = GetTLMConnection(conID);

            if(maxStep > conn.GetParams().Delay) {
                maxStep = conn.GetParams().Delay;
            }

            TLMErrorLog::Info("Connection delay = "+TLMErrorLog::ToStdStr(conn.GetParams().Delay));
        }
        if(1e150 == maxStep) maxStep = 0;
        if(maxStep <= 0) {
            maxStep = 1e-4;
            TLMErrorLog::Warning(string("Too smal max time step for ") +
                                 Components[i]->GetName() + ", set default " +
                                 TLMErrorLog::ToStdStr(maxStep));
        }
        if(!Components[i]->GetSolverMode()) maxStep /= 2;


        TLMErrorLog::Info(string("Choosing the max time step for ")+
                         Components[i]->GetName() + " " +
                         TLMErrorLog::ToStdStr(maxStep));

        Components[i]->StartComponent(SimParams, maxStep);
    }
}

bool CompositeModel::CheckProxyComm() {
    for(ComponentsVector::iterator it = Components.begin(); it!=Components.end(); ++it) {
        if(((*it)->GetSocketHandle() < 0) || !(*it)->GetReadyToSim()) {
            TLMErrorLog::Info(string("Component ") + (*it)->GetName() + " is not ready for simulation");
            return false;
        }
    }
    for(TLMInterfacesVector::iterator it = Interfaces.begin();
        it != Interfaces.end(); it++) {
        if(!(*it)->GetConnected()) {
            TLMErrorLog::Info("TLM interface " + GetTLMComponentProxy((*it)->GetComponentID()).GetName() + '.'
                             + (*it)->GetName() + " is not registered by the component.");
            return false;
        }
    }
    
    TLMErrorLog::Info("Meta model checking completed successfully");

    return true;
}


//! Print meta-model to ostream.
void CompositeModel::Print(std::ostream &os) {
    os << "Components:" << std::endl;
    for(ComponentsVector::iterator it = Components.begin(); it!=Components.end(); ++it) {
        os << (*it)->GetName() << std::endl;
    }

    os << "Interfaces:" << std::endl;
    for(TLMInterfacesVector::iterator it = Interfaces.begin(); it != Interfaces.end(); it++) {
        os << (*it)->GetName() << " : " << (*it)->GetID() << std::endl;
    }

    os << "Connections:" << std::endl;
    for(ConnectionsVector::iterator it = Connections.begin(); it != Connections.end(); it++) {
        os << (*it)->GetID() << " : " << "from=" << (*it)->GetFromID() << "to=" << (*it)->GetToID() << std::endl;
    }
}

#if defined(WIN32)
// Create a string with last error message
std::string GetLastErrorStdStr() {
    DWORD error = GetLastError();
    if(error)
    {
        LPVOID lpMsgBuf;
        DWORD bufLen = FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    error,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR) &lpMsgBuf,
                    0, NULL);
        if(bufLen)
        {
            LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
            std::string result(lpMsgStr, lpMsgStr+bufLen);

            LocalFree(lpMsgBuf);

            return result;
        }
    }
    return std::string();
}
#endif
// Start the component executable
void TLMComponentProxy::StartComponent(SimulationParams& SimParams, double MaxStep) {
    TLMErrorLog::Info(string("Starting ") + StartCommand);

    // In the special case where start-command is explicitely set to "none"
    // we skip startup. This is useful for integrated simulation/tlm-manager.
    if(StartCommand != "none") {
        string startTime = SimParams.GetStartTimeStr();
        string endTime = SimParams.GetEndTimeStr();
        string strMaxStep = std::to_string(MaxStep);
        string serverName = SimParams.GetServerName();

#if defined(WIN32)
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        const char* comspec = std::getenv("COMSPEC");
        if(strcmp(comspec, "") == 0) {
            comspec = "cmd";
        }

        // Start the child process.
        std::stringstream command;
        command << comspec << " /c " << StartCommand.c_str() << ".bat";
        command << " " << Name.c_str();
        command << " " << startTime.c_str();
        command << " " << endTime.c_str();
        command << " " << strMaxStep.c_str();
        command << " " << serverName.c_str();
        command << " " << ModelName.c_str();
        TLMErrorLog::Info(string("Starting ") + command.str());
        if(!CreateProcessA(NULL, (char *)command.str().c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            TLMErrorLog::FatalError("StartComponent: Failed to start the component " + Name + " with command " + StartCommand + "."
                                                                                                                                "Error is " + GetLastErrorStdStr());
            exit(-1);
        } else {
            TLMErrorLog::Info(string("CreateProcessA Success"));
        }

        // Close process and thread handles.
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        //	_spawnlp(_P_NOWAIT, StartCommand.c_str(), StartCommand.c_str(),
        //		 Name.c_str(),
        //		 startTime.c_str(),
        //		 endTime.c_str(),
        //		 strMaxStep.c_str(),
        //		 serverName.c_str(),
        //		 ModelName.c_str(),
        //		 NULL
        //		);


#elif defined(__CYGWIN__)
        spawnlp(_P_NOWAIT, StartCommand.c_str(), StartCommand.c_str(),
                Name.c_str(),
                startTime.c_str(),
                endTime.c_str(),
                strMaxStep.c_str(),
                serverName.c_str(),
                ModelName.c_str(),
                NULL);
#else
        // We create a child that runs the simulation program.
        pid_t child;
        switch(child = fork()) {
        case -1:  // failed!!!!
            TLMErrorLog::FatalError("StartComponent: Failed to start a component");
            break;
        case 0:   // I'm a child. I'll execute the program
            execlp(StartCommand.c_str(), StartCommand.c_str(),
                    Name.c_str(),
                    startTime.c_str(),
                    endTime.c_str(),
                    strMaxStep.c_str(),
                    serverName.c_str(),
                    ModelName.c_str(),
                    NULL);

            // We add a try, catch around fatal error in order to force a exit-value other than 0!
            try {
            // If we get here, something went wrong.
            TLMErrorLog::FatalError("StartComponent: Failed to start the component " + Name + " with command " + StartCommand);
        }
            catch(...) {
        }
            exit(-1);
            break;
        default:  // I'm the parent, so just continue.
            break;
        }
#endif    
    }
    else {
        TLMErrorLog::Info("Start command \"none\" nothing started!");
    }
}

ComponentParameterProxy::ComponentParameterProxy(int CompID, int ParID, std::string &aName, std::string &aDefaultValue) :
    ParameterID(ParID),
    ComponentID(CompID),
    Name(aName),
    Value(aDefaultValue) {}
