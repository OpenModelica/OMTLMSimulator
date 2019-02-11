//
// File: ManagerMain.cc
//
// TLM manager main program file.

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <thread>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <math.h>

#include "Logging/TLMErrorLog.h"
#include "CompositeModels/CompositeModel.h"
#include "CompositeModels/CompositeModelReader.h"
#include "Communication/ManagerCommHandler.h"
#include "Plugin/MonitoringPluginImplementer.h"
#include "Logging/TLMErrorLog.h"
#include "CompositeModels/CompositeModel.h"
#include "CompositeModels/CompositeModelReader.h"
#include "Communication/ManagerCommHandler.h"
#include "Plugin/MonitoringPluginImplementer.h"
#include "OMTLMSimulatorLib.h"

#ifndef _WIN32
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BCloseSocket close
#else
#include <winsock2.h>
#define NOMINMAX
#include <windows.h>
#include <cassert>
#include <io.h>
#define BCloseSocket closesocket
#endif

#include "double3.h"
#include "double33.h"
#ifndef NO_RTIME
#include "timing.h"
#else
#include "SurrogateTimer.h"
#endif //NO_RTIME
#include "coordTransform.h"
#include "double3.h"
#include "double33.h"

#if defined(__unix__)
#include <signal.h>
#endif

#ifdef _MSC_VER
#include "mygetopt.h"
#else
#include <getopt.h>
#endif

using std::string;

static std::map<std::string,int> subModelMap;
static std::map<std::string,int> interfaceMap;


struct Color {
  double r;
  double g;
  double b;
  Color(double rr, double gg, double bb) {
    r = rr;
    g = gg;
    b = bb;
  }
};


class CompositeModelProxy {
public:
  omtlm_CompositeModel *mpCompositeModel = 0;
  double startTime = 0;
  double stopTime = 1;
  int logLevel = 0;
  std::string serverAddress = "127.0.1.1";
  int managerPort = 11111;
  int monitorPort = 12111;
  double logStepSize = 1e-4;
  int numLogSteps = 1000;

};


TLMPlugin* InitializeTLMConnection(omtlm_CompositeModel& model, std::string& serverName) {
  TLMPlugin* TLMlink = MonitoringPluginImplementer::CreateInstance();

#if defined(__unix__)
  signal(SIGPIPE, SIG_IGN); // Handle return value of send instead of crashing on Linux
#endif

  TLMErrorLog::Info("Trying to register TLM monitor on host " + serverName);

  if(! TLMlink->Init("monitor",
                     model.GetSimParams().GetStartTime(),
                     model.GetSimParams().GetEndTime(),
                     model.GetSimParams().GetWriteTimeStep(),
                     serverName))
  {
    TLMErrorLog::FatalError("Cannot initialize MonitoringPluginImplementer.");
    std::cout << "Cannot initialize MonitoringPluginImplementer.\n";
    return 0;
  }

  int nTLMInterfaces = model.GetInterfacesNum();
  for(int i=0; i<nTLMInterfaces; i++) {
    TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
    TLMComponentProxy& component = model.GetTLMComponentProxy(interfaceProxy.GetComponentID());

    TLMErrorLog::Info("Trying to register monitoring interface " + interfaceProxy.GetName());
    int TLMInterfaceID = TLMlink->RegisteTLMInterface(component.GetName() + "." + interfaceProxy.GetName(),
                                                      interfaceProxy.GetDimensions(), interfaceProxy.GetCausality(),
                                                      interfaceProxy.GetDomain());

    if(TLMInterfaceID >= 0) {
      TLMErrorLog::Info("Registration was successful");
      std::string compName = model.GetTLMComponentProxy(interfaceProxy.GetComponentID()).GetName();
    }
    else {
      TLMErrorLog::Info("Interface not connected in Meta-Model: " + component.GetName() + "." + interfaceProxy.GetName());
    }
  }

  return TLMlink;
}

//! Evaluate the data needed for the current time step.
void MonitorTimeStep(TLMPlugin* TLMlink,
                     omtlm_CompositeModel& model,
                     double SimTime,
                     std::map<int, TLMTimeDataSignal>& dataStorageSignal,
                     std::map<int, TLMTimeData1D>& dataStorage1D,
                     std::map<int, TLMTimeData3D>& dataStorage3D) {
  if(TLMlink != 0) {
    // Get data from TLM-Manager here!
    int nTLMInterfaces = model.GetInterfacesNum();

    for(int i=0; i<nTLMInterfaces; i++) {
      TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
      int interfaceID = interfaceProxy.GetID();
      int connectionID = interfaceProxy.GetConnectionID();
      int dimensions = interfaceProxy.GetDimensions();
      string causality = interfaceProxy.GetCausality();

      if(TLMErrorLog::GetLogLevel() >= TLMLogLevel::Info) {
          TLMErrorLog::Info("Data request for " + interfaceProxy.GetName() + " for time " + ToStr(SimTime) + ", id: " + ToStr(interfaceID));
      }

      if(connectionID >= 0) {
#define LOGGEDFORCEFIX
#ifdef  LOGGEDFORCEFIX
        if(dimensions == 6) {
          TLMTimeData3D& PrevTimeData = dataStorage3D[interfaceID];
          TLMTimeData3D& CurTimeData = dataStorage3D[interfaceID];

          TLMlink->GetTimeData3D(interfaceID, SimTime, CurTimeData);

          double delay = model.GetTLMConnection(interfaceProxy.GetConnectionID()).GetParams().Delay;
          double alpha = model.GetTLMConnection(interfaceProxy.GetConnectionID()).GetParams().alpha;
          TLMlink->GetTimeData3D(interfaceID, SimTime-delay, PrevTimeData);

          //Apply damping factor, since this can not be done in GetTimeData (DampedTimeData is not available for monitor)
          for(int i = 0; i < 6; i++) {
            CurTimeData.GenForce[i] =
                CurTimeData.GenForce[i] * (1 - alpha)
                + PrevTimeData.GenForce[i] * alpha;
          }
        }
        else if(dimensions == 1 && causality == "bidirectional") {
          TLMTimeData1D& PrevTimeData = dataStorage1D[interfaceID];
          TLMTimeData1D& CurTimeData = dataStorage1D[interfaceID];

          TLMlink->GetTimeData1D(interfaceID, SimTime, CurTimeData);

          double delay = model.GetTLMConnection(interfaceProxy.GetConnectionID()).GetParams().Delay;
          double alpha = model.GetTLMConnection(interfaceProxy.GetConnectionID()).GetParams().alpha;
          TLMlink->GetTimeData1D(interfaceID, SimTime-delay, PrevTimeData);

          //Apply damping factor, since this can not be done in GetTimeData (DampedTimeData is not available for monitor)
          CurTimeData.GenForce = CurTimeData.GenForce*(1-alpha) + PrevTimeData.GenForce*alpha;
        }
        else if(dimensions == 1 && causality == "output") {
          TLMTimeDataSignal& CurTimeData = dataStorageSignal[interfaceID];
          int linkedID = interfaceProxy.GetLinkedID();
          TLMlink->GetTimeDataSignal(interfaceID, SimTime, CurTimeData, true);
        }
        //No need to check for erroneous interface type, we can simply log nothing instead /robbr
#else
        TLMTimeData& CurTimeData = dataStorage[interfaceID];
        TLMlink->GetTimeData(interfaceID, SimTime, CurTimeData);
#endif
      }
    }
  }
}

void WriteVisualXMLFile(omtlm_CompositeModel& model, std::string &baseFileName, std::string &path) {
  // Get data from TLM-Manager here!
  bool canWriteVisualXMLFile = false;
  int nTLMComponents = model.GetComponentsNum();
  for (int i = 0; i < nTLMComponents; i++) {
    TLMComponentProxy& component = model.GetTLMComponentProxy(i);
    if(!component.GetGeometryFile().empty()) {
      canWriteVisualXMLFile = true;
    }
  }
  // write the visual xml file
  if(canWriteVisualXMLFile) {
    std::ofstream visualFile((baseFileName + "_visual.xml").c_str());
    if(!visualFile.good()) {
      TLMErrorLog::FatalError("Failed to open outfile " + baseFileName + "_visual.xml, give up.");
      return;
    }
    visualFile << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    visualFile << "<visualization>\n";

    visualFile << "  <shape>\n";
    visualFile << "    <ident>x-axis</ident>\n";
    visualFile << "    <type>cylinder</type>\n";
    visualFile << "    <T>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "    </T>\n";
    visualFile << "    <r>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </r>\n";
    visualFile << "    <r_shape>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </r_shape>\n";
    visualFile << "    <lengthDir>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </lengthDir>\n";
    visualFile << "    <widthDir>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </widthDir>\n";
    visualFile << "    <length><exp>0.4375</exp></length>\n";
    visualFile << "    <width><exp>0.0025</exp></width>\n";
    visualFile << "    <height><exp>0.0025</exp></height>\n";
    visualFile << "    <extra><exp>0.0</exp></extra>\n";
    visualFile << "    <color>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>255.0</exp>\n";
    visualFile << "    </color>\n";
    visualFile << "    <specCoeff><exp>0.7</exp></specCoeff>\n";
    visualFile << "  </shape>\n";

    visualFile << "  <shape>\n";
    visualFile << "    <ident>y-axis</ident>\n";
    visualFile << "    <type>cylinder</type>\n";
    visualFile << "    <T>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "    </T>\n";
    visualFile << "    <r>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </r>\n";
    visualFile << "    <r_shape>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </r_shape>\n";
    visualFile << "    <lengthDir>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </lengthDir>\n";
    visualFile << "    <widthDir>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </widthDir>\n";
    visualFile << "    <length><exp>0.4375</exp></length>\n";
    visualFile << "    <width><exp>0.0025</exp></width>\n";
    visualFile << "    <height><exp>0.0025</exp></height>\n";
    visualFile << "    <extra><exp>0.0</exp></extra>\n";
    visualFile << "    <color>\n";
    visualFile << "      <exp>255.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </color>\n";
    visualFile << "    <specCoeff><exp>0.7</exp></specCoeff>\n";
    visualFile << "  </shape>\n";

    visualFile << "  <shape>\n";
    visualFile << "    <ident>z-axis</ident>\n";
    visualFile << "    <type>cylinder</type>\n";
    visualFile << "    <T>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "    </T>\n";
    visualFile << "    <r>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </r>\n";
    visualFile << "    <r_shape>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </r_shape>\n";
    visualFile << "    <lengthDir>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "    </lengthDir>\n";
    visualFile << "    <widthDir>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>1.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </widthDir>\n";
    visualFile << "    <length><exp>0.4375</exp></length>\n";
    visualFile << "    <width><exp>0.0025</exp></width>\n";
    visualFile << "    <height><exp>0.0025</exp></height>\n";
    visualFile << "    <extra><exp>0.0</exp></extra>\n";
    visualFile << "    <color>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "      <exp>255.0</exp>\n";
    visualFile << "      <exp>0.0</exp>\n";
    visualFile << "    </color>\n";
    visualFile << "    <specCoeff><exp>0.7</exp></specCoeff>\n";
    visualFile << "  </shape>\n";

    // colors vector
    std::vector<Color> colors;
    Color color = Color(128.0, 128.0, 128.0); // Gray
    colors.push_back(color);
    color = Color(255.0, 255.0, 0.0); // Yellow
    colors.push_back(color);
    color = Color(0.0, 255.0, 255.0); // Cyan
    colors.push_back(color);
    color = Color(255.0, 0.0, 255.0); // Magenta / Fuchsia
    colors.push_back(color);
    color = Color(128.0, 0.0, 0.0); // Maroon
    colors.push_back(color);

    // components vector
    std::vector<std::string> components;
    int nTLMInterfaces = model.GetInterfacesNum();
    for (int i = 0; i < nTLMInterfaces; i++) {
      TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
      TLMComponentProxy& component = model.GetTLMComponentProxy(interfaceProxy.GetComponentID());
      if(interfaceProxy.GetConnectionID() >= 0) {
        if(std::find(components.begin(), components.end(), component.GetName()) != components.end()) {
          continue;
        }

        double33 T(-interfaceProxy.getTime0Data3D().RotMatrix[0],
            -interfaceProxy.getTime0Data3D().RotMatrix[1],
            -interfaceProxy.getTime0Data3D().RotMatrix[2],
            -interfaceProxy.getTime0Data3D().RotMatrix[3],
            -interfaceProxy.getTime0Data3D().RotMatrix[4],
            -interfaceProxy.getTime0Data3D().RotMatrix[5],
            -interfaceProxy.getTime0Data3D().RotMatrix[6],
            -interfaceProxy.getTime0Data3D().RotMatrix[7],
            -interfaceProxy.getTime0Data3D().RotMatrix[8]);

        double3 r_shape(-interfaceProxy.getTime0Data3D().Position[0],
            -interfaceProxy.getTime0Data3D().Position[1],
            -interfaceProxy.getTime0Data3D().Position[2]);
        r_shape = -T*r_shape;
        double3 lengthDir(1,0,0);
        lengthDir = -T*lengthDir;
        double3 widthDir(0,1,0);
        widthDir = -T*widthDir;

        components.push_back(component.GetName());
        std::string name = component.GetName() + "." + interfaceProxy.GetName();
        visualFile << "  <shape>\n";
        visualFile << "    <ident>" << name << "</ident>\n";
        visualFile << "    <type>file://" << path << "/" << component.GetName() << "/" << component.GetGeometryFile() << "</type>\n";
        visualFile << "    <T>\n";
        visualFile << "      <cref>" << name << ".A(1,1) [-]</cref>\n";
        visualFile << "      <cref>" << name << ".A(1,2) [-]</cref>\n";
        visualFile << "      <cref>" << name << ".A(1,3) [-]</cref>\n";
        visualFile << "      <cref>" << name << ".A(2,1) [-]</cref>\n";
        visualFile << "      <cref>" << name << ".A(2,2) [-]</cref>\n";
        visualFile << "      <cref>" << name << ".A(2,3) [-]</cref>\n";
        visualFile << "      <cref>" << name << ".A(3,1) [-]</cref>\n";
        visualFile << "      <cref>" << name << ".A(3,2) [-]</cref>\n";
        visualFile << "      <cref>" << name << ".A(3,3) [-]</cref>\n";
        visualFile << "    </T>\n";
        visualFile << "    <r>\n";
        visualFile << "      <cref>" << name << ".R[cG][cG](1) [m]</cref>\n";
        visualFile << "      <cref>" << name << ".R[cG][cG](2) [m]</cref>\n";
        visualFile << "      <cref>" << name << ".R[cG][cG](3) [m]</cref>\n";
        visualFile << "    </r>\n";
        visualFile << "    <r_shape>\n";
        visualFile << "      <exp>" << r_shape(1) << "</exp>\n";
        visualFile << "      <exp>" << r_shape(2) << "</exp>\n";
        visualFile << "      <exp>" << r_shape(3) << "</exp>\n";
        visualFile << "    </r_shape>\n";
        visualFile << "    <lengthDir>\n";
        visualFile << "      <exp>" << lengthDir(1) << "</exp>\n";
        visualFile << "      <exp>" << lengthDir(2) << "</exp>\n";
        visualFile << "      <exp>" << lengthDir(3) << "</exp>\n";
        visualFile << "    </lengthDir>\n";
        visualFile << "    <widthDir>\n";
        visualFile << "      <exp>" << widthDir(1) << "</exp>\n";
        visualFile << "      <exp>" << widthDir(2) << "</exp>\n";
        visualFile << "      <exp>" << widthDir(3) << "</exp>\n";
        visualFile << "    </widthDir>\n";
        visualFile << "    <length><exp>0.0</exp></length>\n";
        visualFile << "    <width><exp>0.0</exp></width>\n";
        visualFile << "    <height><exp>0.0</exp></height>\n";
        visualFile << "    <extra><exp>0.0</exp></extra>\n";
        visualFile << "    <color>\n";
        visualFile << "      <exp>" << colors[i % colors.size()].r << "</exp>\n";
        visualFile << "      <exp>" << colors[i % colors.size()].g << "</exp>\n";
        visualFile << "      <exp>" << colors[i % colors.size()].b << "</exp>\n";
        visualFile << "    </color>\n";
        visualFile << "    <specCoeff><exp>0.7</exp></specCoeff>\n";
        visualFile << "  </shape>\n";
      }
    }
    visualFile << "</visualization>";
  }
}

void PrintHeader(omtlm_CompositeModel& model, std::ofstream& dataFile) {
  // Get data from TLM-Manager here!
  int nTLMInterfaces = model.GetInterfacesNum();

  // First variable written is time.
  dataFile << "\"" << "time\",\"wallTime\",";

  int nActiveInterfaces = 0;
  for(int i=0; i<nTLMInterfaces; i++) {
    TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
    TLMComponentProxy& component = model.GetTLMComponentProxy(interfaceProxy.GetComponentID());
    if(interfaceProxy.GetConnectionID() >= 0) {
      if(interfaceProxy.GetDimensions() == 6) {
        // Comma between interfaces
        if(nActiveInterfaces > 0) dataFile << ",";

        // Add all TLM variable names for all active interfaces
        std::string name = component.GetName() + "." + interfaceProxy.GetName();
        dataFile << "\"" << name << ".R[cG][cG](1) [m]\",\"" << name << ".R[cG][cG](2) [m]\",\"" << name << ".R[cG][cG](3) [m]\","; // Position vector
        dataFile << "\"" << name << ".phi[cG](1) [rad]\",\"" << name << ".phi[cG](2) [rad]\",\"" << name << ".phi[cG](3) [rad]\","; // Orientation vector (three angles)
        dataFile << "\"" << name << ".A(1,1) [-]\",\"" << name << ".A(1,2) [-]\",\"" << name << ".A(1,3) [-]\",\""
                 << name << ".A(2,1) [-]\",\"" << name << ".A(2,2) [-]\",\"" << name << ".A(2,3) [-]\",\""
                 << name << ".A(3,1) [-]\",\"" << name << ".A(3,2) [-]\",\"" << name << ".A(3,3) [-]\","; // Transformation matrix
        dataFile << "\"" << name << ".vR[cG][cG,cG](1) [m/s]\",\"" << name << ".vR[cG][cG,cG](2) [m/s]\",\"" << name << ".vR[cG][cG,cG](3) [m/s]\","; // velocity
        dataFile << "\"" << name << ".Omega[cG][cG](1) [rad/s]\",\"" << name << ".Omega[cG][cG](2) [rad/s]\",\"" << name << ".Omega[cG][cG](3) [rad/s]\","; // angular velocity
        dataFile << "\"" << name << ".F_tie[cG](1) [N]\",\"" << name << ".F_tie[cG](2) [N]\",\"" << name << ".F_tie[cG](3) [N]\","; // force vector
        dataFile << "\"" << name << ".M_tie[cG][cG](1) [Nm]\",\"" << name << ".M_tie[cG][cG](2) [Nm]\",\"" << name << ".M_tie[cG][cG](3) [Nm]\""; // torque vector

        nActiveInterfaces++;
      }
      else if(interfaceProxy.GetDimensions() == 1 &&
              interfaceProxy.GetCausality() == "bidirectional") {
        // Comma between interfaces
        if(nActiveInterfaces > 0) dataFile << ",";

        // Add all TLM variable names for all active interfaces
        std::string name = component.GetName() + "." + interfaceProxy.GetName();
        if(interfaceProxy.GetDomain() == "hydraulic") {
          dataFile << "\"" << name << ".q [m^3/s]\","; // Volume flow
          dataFile << "\"" << name << ".p [Pa]\""; // Pressure
        }
        else if(interfaceProxy.GetDomain() == "mechanical") {
          dataFile << "\"" << name << ".x [m]\","; // Position
          dataFile << "\"" << name << ".v [m/s]\","; // Speed
          dataFile << "\"" << name << ".F [N]\""; // Force
        }
        else if(interfaceProxy.GetDomain() == "rotational") {
          dataFile << "\"" << name << ".phi [rad]\","; // Position
          dataFile << "\"" << name << ".w [rad/s]\","; // Speed
          dataFile << "\"" << name << ".T [Nm]\""; // Force
        }
        else if(interfaceProxy.GetDomain() == "electric") {
          dataFile << "\"" << name << ".I [A]\","; // Current
          dataFile << "\"" << name << ".U [V]\""; // Voltage
        }

        nActiveInterfaces++;
      }
      else if(interfaceProxy.GetDimensions() == 1 &&
              interfaceProxy.GetCausality() == "output") {
        // Comma between interfaces
        if(nActiveInterfaces > 0) dataFile << ",";

        // Add variable names for all active interfaces
        std::string name = component.GetName() + "." + interfaceProxy.GetName();
        dataFile << "\"" << name << "\""; // Value

        nActiveInterfaces++;
      }
    }
  }

  dataFile << std::endl;
}

void PrintData(omtlm_CompositeModel& model,
               std::ofstream& dataFile,
               tTM_Info& tInfo,
               std::map<int, TLMTimeDataSignal> &dataStorageSignal,
               std::map<int, TLMTimeData1D>& dataStorage1D,
               std::map<int, TLMTimeData3D> &dataStorage3D) {
  double startTime = model.GetSimParams().GetStartTime();

  double wallTime = tInfo.total.tv_sec + tInfo.total.tv_nsec/1.0e9;

  // Get data from TLM-Manager here!
  int nTLMInterfaces = model.GetInterfacesNum();

  bool printTimeFlg = true;
  int nActiveInterfaces = 0;

  for(int i=0; i<nTLMInterfaces; i++) {
    TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
    if(interfaceProxy.GetConnectionID() >= 0) {
      if(interfaceProxy.GetDimensions() == 6) {
        if(TLMErrorLog::GetLogLevel() >= TLMLogLevel::Info) {
          std::stringstream ss;
          ss << "Printing data for 3D interface " << interfaceProxy.GetID();
          TLMErrorLog::Info(ss.str());
        }

        TLMTimeData3D& timeData = dataStorage3D.at(interfaceProxy.GetID());

        if(timeData.time < startTime) {
          timeData.time = startTime;
        }

        // Print time only once, that is, for the first entry.
        if(printTimeFlg) {
          dataFile << timeData.time << ",";
          dataFile << wallTime << ",";
          printTimeFlg = false;
        }

        // Comma between interfaces
        if(nActiveInterfaces > 0) dataFile << ",";

        // Convert orientation matrix to angles

        // first convert the matrices into double33 format
        double33 A(timeData.RotMatrix[0], timeData.RotMatrix[1], timeData.RotMatrix[2],
            timeData.RotMatrix[3], timeData.RotMatrix[4], timeData.RotMatrix[5],
            timeData.RotMatrix[6], timeData.RotMatrix[7], timeData.RotMatrix[8]);

        // Then convert to angles
        double3 phi = ATophi321(A);

        // Backward calculation of force from TLM wave.
        // This is done because the actual force send is the delayed force.
        // The wave is: C = - Force + Impedance * Velocity -> F = -(C - Imp*Vel)
        double3 force(0.0);
        double3 torque(0.0);
        TLMConnection& connection = model.GetTLMConnection(interfaceProxy.GetConnectionID());
        for(int i = 0; i < 3; i++) {
#if 1
          force(i+1) =  -timeData.GenForce[i] + connection.GetParams().Zf * timeData.Velocity[i];
          torque(i+1) = -timeData.GenForce[i+3] + connection.GetParams().Zfr * timeData.Velocity[i+3];
#else
          force(i+1) =  timeData.GenForce[i];
          torque(i+1) = timeData.GenForce[i+3];
#endif
        }

        dataFile << timeData.Position[0] << "," << timeData.Position[1] << "," << timeData.Position[2] << ",";
        dataFile << phi(1)               << "," << phi(2)               << "," << phi(3)               << ",";
        dataFile << A(1,1)               << "," << A(1,2)               << "," << A(1,3)               << ",";
        dataFile << A(2,1)               << "," << A(2,2)               << "," << A(2,3)               << ",";
        dataFile << A(3,1)               << "," << A(3,2)               << "," << A(3,3)               << ",";
        dataFile << timeData.Velocity[0] << "," << timeData.Velocity[1] << "," << timeData.Velocity[2] << ",";
        dataFile << timeData.Velocity[3] << "," << timeData.Velocity[4] << "," << timeData.Velocity[5] << ",";
        dataFile << force(1)             << "," << force(2)             << "," << force(3)             << ",";
        dataFile << torque(1)            << "," << torque(2)            << "," << torque(3);

        nActiveInterfaces++;
      }
      else if(interfaceProxy.GetDimensions() == 1 &&
              interfaceProxy.GetCausality() == "bidirectional") {
        if(TLMErrorLog::GetLogLevel() >= TLMLogLevel::Info) {
          std::stringstream ss;
          ss << "Printing data for 1D interface " << interfaceProxy.GetID();
          TLMErrorLog::Info(ss.str());
        }

        TLMTimeData1D& timeData = dataStorage1D.at(interfaceProxy.GetID());

        if(timeData.time < startTime) {
          timeData.time = startTime;
        }
        // Print time only once, that is, for the first entry.
        if(printTimeFlg) {
          dataFile << timeData.time << ",";
          dataFile << wallTime << ",";
          printTimeFlg = false;
        }

        // Comma between interfaces
        if(nActiveInterfaces > 0) dataFile << ",";

        // Backward calculation of force from TLM wave.
        // This is done because the actual force send is the delayed force.
        // The wave is: C = - Force + Impedance * Velocity -> F = -(C - Imp*Vel)

        TLMConnection& connection = model.GetTLMConnection(interfaceProxy.GetConnectionID());

        double force;
        if(interfaceProxy.GetDomain() == "hydraulic") {
          force =  timeData.GenForce + connection.GetParams().Zf * timeData.Velocity;
        }
        else {
          force =  -timeData.GenForce + connection.GetParams().Zf * timeData.Velocity;
        }

        if(interfaceProxy.GetDomain() == "hydraulic") {
          dataFile << timeData.Velocity << ",";     //Flow
          dataFile << force;                        //Pressure
        } else if(interfaceProxy.GetDomain() == "mechanical") {
          dataFile << timeData.Position << ",";
          dataFile << timeData.Velocity << ",";
          dataFile << force;
        }
        else if(interfaceProxy.GetDomain() == "rotational") {
          dataFile << timeData.Position << ",";   //Angle
          dataFile << timeData.Velocity << ",";   //Angular velocity
          dataFile << force;                      //Torque
        }
        else if(interfaceProxy.GetDomain() == "electric") {
          dataFile << timeData.Velocity << ",";     //Current
          dataFile << force;                        //Voltage
        }

        nActiveInterfaces++;
      }
      else if(interfaceProxy.GetDimensions() == 1 &&
              interfaceProxy.GetCausality() == "output") {

        if(TLMErrorLog::GetLogLevel() >= TLMLogLevel::Info) {
          std::stringstream ss;
          ss << "Printing data for output interface " << interfaceProxy.GetID();
          TLMErrorLog::Info(ss.str());
        }

        TLMTimeDataSignal& timeData = dataStorageSignal.at(interfaceProxy.GetID());

        if(timeData.time < startTime) {
          timeData.time = startTime;
        }
        // Print time only once, that is, for the first entry.
        if(printTimeFlg) {
          dataFile << timeData.time << ",";
          dataFile << wallTime << ",";
          printTimeFlg = false;
        }

        // Comma between interfaces
        if(nActiveInterfaces > 0) dataFile << ",";

        dataFile << timeData.Value;

        nActiveInterfaces++;
      }
    }
  }
  dataFile << std::endl;
}

void PrintRunStatus(omtlm_CompositeModel& model, std::ofstream& runFile, tTM_Info& tInfo, double SimTime)  {
  double startTime = model.GetSimParams().GetStartTime();
  double endTime = model.GetSimParams().GetEndTime();
  double timeStep = model.GetSimParams().GetWriteTimeStep();
  int numSteps = (endTime-startTime)/timeStep;
  int curStep = (SimTime-startTime)/timeStep;
  double progress = ((curStep*1.0)/(numSteps*1.0))*100.0;
  std::string statusStr = (curStep == numSteps ? "Done" : "Running");

  // Calculate average wall clock time for a single logging time step.
  timespec_t avg;
  TM_Avg(&tInfo, &avg);

  double avgStepTime = avg.tv_sec + (avg.tv_nsec/1.0e9);
  double timeLeft = static_cast<double>(numSteps-curStep)*avgStepTime;
  int hLeft = timeLeft/3600;
  int mLeft = (timeLeft - static_cast<double>(hLeft))/60;
  int sLeft = (timeLeft - static_cast<double>(hLeft) - static_cast<double>(mLeft));

  // Always write from beginning of file, that is, overwrite old data.
  runFile.seekp(0);
  runFile << "Status    : " << statusStr << std::endl;
  runFile << "Sim. time : " << SimTime   << std::endl;
  runFile << "Step      : " << curStep   << " of " << numSteps << std::endl;
  runFile << "Progress  : " << progress   << "%" << std::endl;
  runFile << "            " << std::endl;
  runFile << "Estimated time left: " << hLeft << ":" << mLeft << ":" << sLeft << std::endl;
  //runFile << "average time per step: " << avgStepTime << " sec." << endl;
  //runFile << "time left            : " << timeLeft << " sec." << endl;
  runFile << "                                                              " << std::endl;
}


//Start it threaded!
int startMonitor(double timeStep,
                 double nSteps,
                 std::string server,
                 std::string modelName,
                 omtlm_CompositeModel &model) {

  TLMErrorLog::Info("Starting monitoring...");

#ifndef USE_THREADS
#ifdef _WIN32
#pragma message ( "TLM manager requires pthreads to be compiled in. Use -DUSE_THREADS in the Makefile.head if neeeded." )
#else
#warning TLM manager requires pthreads to be compiled in. Use -DUSE_THREADS in the Makefile.head if neeeded.
#endif
  std::cout << "Exiting (no threads)\n";
  TLMErrorLog::Error("tlmmanger was compiled without threads and is not usable.");
  exit(1);
#endif



  //  // Enable debug?
  //  std::ofstream logfile;
  //  logfile.open("monitor2.log");
  //  TLMErrorLog::SetOutStream(logfile);
  //  if(debug) {
  //    TLMErrorLog::SetDebugOut(true);
  //    TLMErrorLog::SetNormalErrorLogOn(true);
  //    TLMErrorLog::SetWarningOut(true);
  //  }



  // Open file for data logging, that is, storing the co-simulation data.
  std::ofstream outdataFile((modelName + ".csv").c_str());
  if(!outdataFile.good()) {
    TLMErrorLog::FatalError("Failed to open outfile " + modelName + ".csv, give up.");
    exit(1);
  }

  // Open run file for logging of simulation progress.
  std::ofstream runFile((modelName + ".run").c_str());
  if(!runFile.good()) {
    TLMErrorLog::FatalError("Failed to open runfile " + modelName + ".run, give up.");
    exit(1);
  }

  // Initialize TLM
  model.CheckTheModel();
  TLMPlugin* thePlugin = InitializeTLMConnection(model, server);
  if(!thePlugin) {
    TLMErrorLog::FatalError("Failed to initialize TLM interface, give up.");
    exit(1);
  }

  // Setup simulation time for logging.
  double simTime = model.GetSimParams().GetStartTime();
  double endTime  = model.GetSimParams().GetEndTime();

  // Setup time step for output logging according to priority:
  // 1. User input specified in -t option
  // 2. User input specified in -s option
  // 3. Time step from CompositeModel
  if(timeStep == 0.0) {
    if(nSteps > 0) {
      timeStep = (endTime-simTime)/static_cast<double>(nSteps);
    }
    else {
      timeStep = model.GetSimParams().GetWriteTimeStep();
    }
  }

  // Print/log the header information
  PrintHeader(model, outdataFile);

  // Setup timer for run-time estimation.
  tTM_Info tInfo;
  TM_Init(&tInfo);
  TM_Clear(&tInfo);

  do {
    // Adjust to meet end-time step.
    if(simTime > endTime) simTime = endTime;

    // Data structure for data logging
    std::map<int, TLMTimeDataSignal> dataSignal;
    std::map<int, TLMTimeData1D> data1D;
    std::map<int, TLMTimeData3D> data3D;

    // Get data for next time step.
    TM_Start(&tInfo);
    MonitorTimeStep(thePlugin, model, simTime, dataSignal, data1D, data3D);
    TM_Stop(&tInfo);

    // Print data row
    PrintData(model, outdataFile, tInfo, dataSignal, data1D, data3D);

    // Update run status
    PrintRunStatus(model, runFile, tInfo, simTime);

    // Next time step
    simTime += timeStep;
  } while(simTime < endTime);

  TLMErrorLog::Info("Monitor sending close request (simTime = "+std::to_string(simTime)+", endTime = "+std::to_string(endTime)+")");
  thePlugin->SendCloseNotification();

  delete thePlugin;

  return 0;
}



// Print all interfaces position and orientation
void PrintInterfaceInformation(omtlm_CompositeModel& theModel) {
  std::ofstream interfacefile ("interfaceData.xml");
  if(interfacefile.is_open()) {
    interfacefile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    interfacefile << "<ModelData>\n";
    interfacefile << "<Interfaces>\n";
    if(theModel.GetInterfacesNum() > 0) {
      for(size_t idx=0; idx<theModel.GetInterfacesNum(); idx++) {
        TLMInterfaceProxy& intProx = theModel.GetTLMInterfaceProxy(idx);
        TLMComponentProxy& comProx = theModel.GetTLMComponentProxy(intProx.GetComponentID());
        TLMTimeData3D& tlmData = intProx.getTime0Data3D();

        double3 R(tlmData.Position[0], tlmData.Position[1], tlmData.Position[2]);
        double33 A(tlmData.RotMatrix[0], tlmData.RotMatrix[1], tlmData.RotMatrix[2],
            tlmData.RotMatrix[3], tlmData.RotMatrix[4], tlmData.RotMatrix[5],
            tlmData.RotMatrix[6], tlmData.RotMatrix[7], tlmData.RotMatrix[8]);
        double3 phi = ATophi321(A);

        std::stringstream ss;
        ss << intProx.GetDimensions();
        std::string dimStr = ss.str();
        interfacefile << "\t<Interface model=\"" + comProx.GetName() +
                         "\" Name=\"" + intProx.GetName() +
                         "\" Dimensions=\"" + dimStr +
                         "\" Causality=\"" + intProx.GetCausality() +
                         "\" Domain=\"" + intProx.GetDomain() + "\"\n";
        interfacefile << "\t\tPosition=\"" << R(1) << "," << R(2) << "," << R(3) << "\"\n";
        interfacefile << "\t\tAngle321=\"" << phi(1) << "," << phi(2) << "," << phi(3) << "\"/>\n";
      }
    }
    interfacefile << "</Interfaces>\n";

    interfacefile << "<Parameters>\n";
    for(size_t idx=0; idx<theModel.GetComponentParametersNum(); ++idx) {
      ComponentParameterProxy& parProx = theModel.GetComponentParameterProxy(idx);
      TLMComponentProxy& comProx = theModel.GetTLMComponentProxy(parProx.GetComponentID());

      interfacefile << "\t<Parameter model=\"" + comProx.GetName() +
                       "\" Name=\"" + parProx.GetName() +
                       "\" DefaultValue=\"" +parProx.GetValue() + "\"/>\n";
    }
    interfacefile << "</Parameters>\n";
    interfacefile << "</ModelData>\n";
    interfacefile.close();
  } else {
    std::cout << "Error opening interfaceData.xml file." << std::endl;

    if(theModel.GetInterfacesNum() == 0) {
      std::cout << "No TLM interfaces found." << std::endl;
      return;
    }

    std::cout << "Positions and orientations:" << std::endl;

    for(size_t idx=0; idx<theModel.GetInterfacesNum(); idx++) {
      TLMInterfaceProxy& intProx = theModel.GetTLMInterfaceProxy(idx);
      TLMComponentProxy& comProx = theModel.GetTLMComponentProxy(intProx.GetComponentID());
      TLMTimeData3D& tlmData = intProx.getTime0Data3D();

      double3 R(tlmData.Position[0], tlmData.Position[1], tlmData.Position[2]);
      double33 A(tlmData.RotMatrix[0], tlmData.RotMatrix[1], tlmData.RotMatrix[2],
          tlmData.RotMatrix[3], tlmData.RotMatrix[4], tlmData.RotMatrix[5],
          tlmData.RotMatrix[6], tlmData.RotMatrix[7], tlmData.RotMatrix[8]);

      std::cout << comProx.GetName() << "." << intProx.GetName() << ":" << std::endl;
      std::cout << "R  : " << R << std::endl;
      std::cout << "phi: " << ATophi321(A) << std::endl;
    }
  }
}



int startManager(int serverPort,
                 int monitorPort,
                 ManagerCommHandler::CommunicationMode comMode,
                 omtlm_CompositeModel &model) {

  TLMErrorLog::Info("Printing from manager thread.");

  // Set preferred network port
  if(serverPort > 0) {
    model.GetSimParams().SetPort(serverPort);
  }

  // Set preferred network port for monitoring
  if(monitorPort > 0) {
    model.GetSimParams().SetMonitorPort(monitorPort);
  }

  // Create manager object
  ManagerCommHandler manager(model);


  // Run the simulation
  manager.Run(comMode);

  // Print interface information if needed.
  if(comMode == ManagerCommHandler::InterfaceRequestMode) {
    PrintInterfaceInformation(model);
    return 0;
  }



}


void simulateInternal(void *pModel,
                      bool interfaceRequest,
                      std::string singleModel) {


#ifndef USE_THREADS
#ifdef _WIN32
#pragma message ( "TLM manager requires pthreads to be compiled in. Use -DUSE_THREADS in the Makefile.head if neeeded." )
#else
#warning TLM manager requires pthreads to be compiled in. Use -DUSE_THREADS in the Makefile.head if neeeded.
#endif
  TLMErrorLog::Error("OMTLMSimulatorLib was compiled without threads and is not usable.");
  exit(1);
#endif

  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;

  //checkPortAvailability(pModelProxy->managerPort);
  //checkPortAvailability(pModelProxy->monitorPort);

  ManagerCommHandler::CommunicationMode comMode=ManagerCommHandler::CoSimulationMode;
  if(interfaceRequest) {
    comMode = ManagerCommHandler::InterfaceRequestMode;
  }

  //Set log level
  TLMErrorLog::SetLogLevel(TLMLogLevel(pModelProxy->logLevel));
  if(comMode == ManagerCommHandler::InterfaceRequestMode) {       //Always enable debug for interface request /robbr
    TLMErrorLog::SetLogLevel(TLMLogLevel::Info);
  }


  omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;

  pCompositeModel->CheckTheModel();

  std::string modelName = pCompositeModel->GetModelName();

  std::string server = std::string(pModelProxy->serverAddress)+
                       ":"+std::to_string(pModelProxy->monitorPort);

  // Start manager thread
  std::thread managerThread = std::thread(startManager,
                                          pModelProxy->managerPort,
                                          pModelProxy->monitorPort,
                                          comMode,
                                          std::ref(*pCompositeModel));


  // Start monitor thread (if not interface request mode)
  std::thread monitorThread;
  if(comMode != ManagerCommHandler::InterfaceRequestMode) {
    monitorThread = std::thread(startMonitor,
                                pModelProxy->logStepSize,
                                pModelProxy->numLogSteps,
                                server,
                                modelName,
                                std::ref(*pCompositeModel));
  }

  // Wait for threads to finish
  if(comMode != ManagerCommHandler::InterfaceRequestMode) {
    monitorThread.join();
    std::cout << "Monitoring thread finished.\n";
  }
  managerThread.join();
  std::cout << "Manager thread finished.\n";

  TLMErrorLog::Close();

  return;
}


//This internal function is needed in order to
//hide the two last arguments from the API.
void *loadModelInternal(const char *fileName,
                        bool interfaceRequest,
                        const char *singleModel) {
  // Load composite model for manager
  // Note: Skip loading connections in interface request mode in case an interface no longer exists
  omtlm_CompositeModel *pCompositeModel = new omtlm_CompositeModel();
  {
    CompositeModelReader managerModelReader(*pCompositeModel);
    std::string fileNameStr(fileName);
    managerModelReader.ReadModel(fileNameStr,
                                 interfaceRequest,
                                 std::string(singleModel));
  }

  return (void*)pCompositeModel;
}


void *omtlm_loadModel(const char *filename) {
  CompositeModelProxy *pModelProxy = new CompositeModelProxy();
  pModelProxy->mpCompositeModel = (omtlm_CompositeModel*)loadModelInternal(filename, false, "");
  return pModelProxy;
}


void *omtlm_newModel(const char *name) {
  CompositeModelProxy *pModelProxy = new CompositeModelProxy();
  pModelProxy->mpCompositeModel = new omtlm_CompositeModel();
  pModelProxy->mpCompositeModel->SetModelName(name);
  return pModelProxy;
}



//! @returns Sub-Model ID
void omtlm_addSubModel(void *pModel,
                       const char* name,
                       const char* file,
                       const char* startCommand) {

  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
  std::string path, fileName;

  int id = pCompositeModel->RegisterTLMComponentProxy(name,
                                                      startCommand,
                                                      file,
                                                      false,
                                                      "");
  subModelMap.insert(std::pair<std::string,int>(std::string(name),id));
}



//! @returns Interface ID
void omtlm_addInterface(void *pModel,
                                  const char* subModelName,
                                  const char* name,
                                  int dimensions,
                                  const char* causality,
                                  const char* domain) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
  std::string nameStr(name);
  int subModelId= subModelMap.find(std::string(subModelName))->second;
  int id = pCompositeModel->RegisterTLMInterfaceProxy(subModelId,
                                                      nameStr,
                                                      dimensions,
                                                      causality,
                                                      domain);

  std::string fullName = std::string(subModelName)+"."+nameStr;
  interfaceMap.insert(std::pair<std::string,int>(fullName,id));
}


void omtlm_setInitialPositionAndOrientation(void *pModel,
                                            const char* interfaceName,
                                            std::vector<double> position,
                                            std::vector<double> orientation)
{
  std::string ifcName = interfaceName;
  if(ifcName.find(".") == ifcName.npos) {
    //Apply position and orientation to a component
    int compId = subModelMap.find(std::string(ifcName))->second;
    CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
    omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
    TLMComponentProxy& compProxy = pCompositeModel->GetTLMComponentProxy(compId);
    double x[3];
    double A[9];
    std::copy(position.begin(), position.end(), x);
    std::copy(orientation.begin(), orientation.end(), A);
    compProxy.SetInertialTranformation(x,A);
  }
  else {
    //Apply position and orientation to an interface
    int interfaceId = interfaceMap.find(std::string(interfaceName))->second;
    CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
    omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
    TLMInterfaceProxy& ifcProxy = pCompositeModel->GetTLMInterfaceProxy(interfaceId);
    if(ifcProxy.GetDimensions() == 6) {
      if(position.size() != 3 || orientation.size() != 9) {
        TLMErrorLog::FatalError("Wrong size of vectors with initial position and orientation for interface "+ifcProxy.GetName());
      }
      std::copy(position.begin(), position.end(), ifcProxy.getTime0Data3D().Position);
      std::copy(orientation.begin(), orientation.end(), ifcProxy.getTime0Data3D().RotMatrix);
    }
    else {
      TLMErrorLog::FatalError("Can only set initial position and orientation for 3D interfaces.");
    }
  }
}


void omtlm_addConnection(void *pModel,
                         const char *interfaceName1,
                         const char *interfaceName2,
                         double delay,
                         double Zf,
                         double Zfr,
                         double alpha) {
    // Todo: Error checking

  int interfaceId1 = interfaceMap.find(std::string(interfaceName1))->second;
  int interfaceId2 = interfaceMap.find(std::string(interfaceName2))->second;

  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
  TLMConnectionParams params;
  params.Delay = delay;
  params.Zf = Zf;
  params.Zfr = Zfr;
  params.alpha = alpha;

  int connId = pCompositeModel->RegisterTLMConnection(interfaceId1,
                                                      interfaceId2,
                                                      params);

  TLMConnection connection = pCompositeModel->GetTLMConnection(connId);

  pCompositeModel->GetTLMInterfaceProxy(interfaceId1).SetConnected();
  pCompositeModel->GetTLMInterfaceProxy(interfaceId1).SetConnection(connection);

  pCompositeModel->GetTLMInterfaceProxy(interfaceId2).SetConnected();
  pCompositeModel->GetTLMInterfaceProxy(interfaceId2).SetConnection(connection);
}




void omtlm_addParameter(void *pModel,
                                  const char *subModelName,
                                  const char *name,
                                  const char *defaultValue) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
  std::string nameStr(name);
  std::string defaultStr(defaultValue);
  int subModelId = subModelMap.find(std::string(subModelName))->second;
  pCompositeModel->RegisterComponentParameterProxy(subModelId,
                                                   nameStr,
                                                   defaultStr);
}



void omtlm_unloadModel(void *pModel)
{
  if (!pModel)
  {
    //todo: Error message
    return;
  }

  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
  delete pCompositeModel;
  delete pModelProxy;
}

void omtlm_simulate(void *model) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)model;
  std::cout << "Starting TLM simulation.\n";
  simulateInternal(model,
                   false,
                   "");
}

void omtlm_setStartTime(void *pModel, double startTime)
{
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->startTime = startTime;

  double stopTime = pModelProxy->stopTime;
  omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
  pCompositeModel->GetSimParams().Set(11111,startTime,stopTime);

  double writeTimeStep = (stopTime-startTime)/1000.0;
  pCompositeModel->GetSimParams().SetWriteTimeStep(writeTimeStep);
}

void omtlm_setStopTime(void *pModel, double stopTime)
{
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->stopTime = stopTime;

  double startTime = pModelProxy->startTime;
  omtlm_CompositeModel *pCompositeModel = pModelProxy->mpCompositeModel;
  pCompositeModel->GetSimParams().Set(11111,startTime,stopTime);

  double writeTimeStep = (stopTime-startTime)/1000.0;
  pCompositeModel->GetSimParams().SetWriteTimeStep(writeTimeStep);
}

void omtlm_setLogLevel(void *pModel, int logLevel) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->logLevel = logLevel;
  TLMErrorLog::SetLogLevel(TLMLogLevel(pModelProxy->logLevel));
}

void omtlm_checkPortAvailability(int *port) {
      struct sockaddr_in sa;  // My socket addr.

  #ifdef WIN32
      WSADATA ws;
      int d;
      d=WSAStartup(0x0101,&ws);
      assert(d==0);
  #endif

      memset(&sa,0, sizeof(struct sockaddr_in));
      sa.sin_family = AF_INET;

  #ifdef WIN32
      char* localIP;
      localIP = inet_ntoa (*(struct in_addr *)*hp->h_addr_list);
      sa.sin_addr.s_addr = inet_addr(localIP);
  #endif

      sa.sin_port = htons(*port);

      int theSckt;

      if((theSckt =
       #ifdef WIN32
           socket(AF_INET, SOCK_STREAM,IPPROTO_TCP)
       #else
           socket(AF_INET, SOCK_STREAM,0)

       #endif
          ) < 0) {
          TLMErrorLog::FatalError("Create server socket - failed to get a socket handle");

          (*port) = -1;
          return;
      }

      bool val = true;
      setsockopt(theSckt, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(int));

      int bindCount = 0;
      int maxIterations = 1000; // BUG: should be calculated from a max. port range!
      // Bind the socket, first try the predefined port, then increase port number.
      while(bind(theSckt,(struct sockaddr *) &sa, sizeof(struct sockaddr_in)) < 0 && bindCount < maxIterations) {
          (*port)++;
          //std::cout << "Increasing port number to " << port << "\n";
          bindCount++;
          sa.sin_port = htons(*port);
      }

      if(bindCount == maxIterations) {
          BCloseSocket(theSckt);
          TLMErrorLog::FatalError("Create server socket - failed to bind. Check that the port is free.");
          (*port) = -1;
          return;
      }

      close(theSckt);
}

void omtlm_setAddress(void *pModel, std::string address) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->serverAddress = address;
}

void omtlm_setManagerPort(void *pModel, int port) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->managerPort = port;
}

void omtlm_setMonitorPort(void *pModel, int port) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->monitorPort = port;
}

void omtlm_setLogStepSize(void *pModel, double stepSize) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->logStepSize = stepSize;
}

void omtlm_setNumLogStep(void *pModel, int steps) {
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->numLogSteps = steps;
}

void omtlm_printModelStructure(void *pModel)
{
  CompositeModelProxy *pModelProxy = (CompositeModelProxy*)pModel;
  pModelProxy->mpCompositeModel->Print(std::cout);
}
