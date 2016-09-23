//
// File: ManagerMain.cc
// 
// TLM manager main program file.

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "TLMErrorLog.h"
#include "MetaModel.h"
#include "MetaModelReader.h"
#include "ManagerCommHandler.h"
#include "double3Vec.h"
#include "double33Mat.h"

#ifdef _MSC_VER
#include "mygetopt.h"
#else
#include <getopt.h>
#endif

using namespace tlmMisc;
using std::string;

void usage(){
    string usageStr =
            "Usage: tlmmananger [-d] [-m <monitor-port>] [-p <server-port>] [-r] <metamodel>, where metamodel is a name of XML file.\n"
            "-d                 : enable debug mode\n"
            "-m <monitor-port>  : set the port for monitoring connections\n"
            "-p <server-port>   : set the server network port for communication with the simulation tools\n"
            "-r                 : run manager in interface request mode, get information about interface locations";
    TLMErrorLog::SetDebugOut(true);
    TLMErrorLog::Log(usageStr);
    std::cout << usageStr << std::endl;
    exit(1);
}

// Print all interfaces position and orientation
void print_interface_information(MetaModel& theModel)
{
  std::ofstream interfacefile ("interfaceData.xml");
  if (interfacefile.is_open()) {
    interfacefile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    interfacefile << "<Interfaces>\n";
    if( theModel.GetInterfacesNum() > 0 ) {
      for(size_t idx=0 ; idx<theModel.GetInterfacesNum() ; idx++ ){
        TLMInterfaceProxy& intProx = theModel.GetTLMInterfaceProxy(idx);
        TLMComponentProxy& comProx = theModel.GetTLMComponentProxy(intProx.GetComponentID());
        TLMTimeData& tlmData = intProx.getTime0Data();

        double3Vec R(tlmData.Position[0], tlmData.Position[1], tlmData.Position[2]);
        double33Mat A( tlmData.RotMatrix[0], tlmData.RotMatrix[1], tlmData.RotMatrix[2],
                    tlmData.RotMatrix[3], tlmData.RotMatrix[4], tlmData.RotMatrix[5],
                    tlmData.RotMatrix[6], tlmData.RotMatrix[7], tlmData.RotMatrix[8]);
        double3Vec phi = ATophi321(A);
        
        interfacefile << "\t<Interface model=\"" + comProx.GetName() + "\" Name=\"" + intProx.GetName() + "\"\n";
        interfacefile << "\t\tPosition=\"" << R(1) << "," << R(2) << "," << R(3) << "\"\n";
        interfacefile << "\t\tAngle321=\"" << phi(1) << "," << phi(2) << "," << phi(3) << "\"/>\n"; 
      }
    }
    interfacefile << "</Interfaces>\n";
    interfacefile.close();
  } else {
    std::cout << "Error opening interfaceData.xml file." << std::endl;
    
    if( theModel.GetInterfacesNum() == 0 ){
        std::cout << "No TLM interfaces found." << std::endl;
        return;
    }
    
    std::cout << "Positions and orientations:" << std::endl;

    for(size_t idx=0 ; idx<theModel.GetInterfacesNum() ; idx++ ){
        TLMInterfaceProxy& intProx = theModel.GetTLMInterfaceProxy(idx);
        TLMComponentProxy& comProx = theModel.GetTLMComponentProxy(intProx.GetComponentID());
        TLMTimeData& tlmData = intProx.getTime0Data();

        double3Vec R(tlmData.Position[0], tlmData.Position[1], tlmData.Position[2]);
        double33Mat A( tlmData.RotMatrix[0], tlmData.RotMatrix[1], tlmData.RotMatrix[2],
                    tlmData.RotMatrix[3], tlmData.RotMatrix[4], tlmData.RotMatrix[5],
                    tlmData.RotMatrix[6], tlmData.RotMatrix[7], tlmData.RotMatrix[8]);

        std::cout << comProx.GetName() << "." << intProx.GetName() << ":" << std::endl;
        std::cout << "R  : " << R << std::endl;
        std::cout << "phi: " << ATophi321(A) << std::endl;
    }
  }
}

int main(int argc, char* argv[]) {
#ifndef USE_THREADS
#warning TLM manager requires pthreads to be compiled in. Use -DUSE_THREADS in the Makefile.head if neeeded.    
	TLMErrorLog::Error("tlmmanger was compiled without threads and is not usable.");
	exit(1);    
#endif
    bool debugFlg = false;
    int serverPort = 0;
    int monitorPort = 0;
    ManagerCommHandler::CommunicationMode comMode=ManagerCommHandler::CoSimulationMode;

    char c;
    while ((c = getopt (argc, argv, "dp:m:r")) != -1){
        switch (c) {
        case 'd':
            debugFlg = true;
            break;
        case 'p':
            serverPort = atoi(optarg);
            break;
        case 'm':
            monitorPort = atoi(optarg);
            break;
        case 'r':
            comMode = ManagerCommHandler::InterfaceRequestMode;
            break;
        default:
            usage();
            break;
        }
    }

    // Check for existing model (XML) file argument.
    if( optind >= argc ){
        usage();
    }

    // Debug on?
    if(debugFlg) {
        TLMErrorLog::SetDebugOut(true);
    }
    
    // Create the meta model object
    MetaModel theModel;

    {
        // Create model reader for the model
        MetaModelReader modelReader(theModel);

        std::string inFile(argv[optind]);

        // read the XML file and build the model
        // Note: Skip loading connections in interface request mode in case an interface no longer exists
        modelReader.ReadModel(inFile,comMode == ManagerCommHandler::InterfaceRequestMode);
    }
    
    // Set preferred network port
    if( serverPort > 0 ){
        theModel.GetSimParams().SetPort(serverPort);
    }

    // Set preferred network port for monitoring
    if( monitorPort > 0 ){
        theModel.GetSimParams().SetMonitorPort(monitorPort);
    }

    // Create manager object
    ManagerCommHandler manager(theModel);

    // Run the simulation
    manager.Run(comMode);

    // Print interface information if needed.
    if( comMode == ManagerCommHandler::InterfaceRequestMode ){
        print_interface_information(theModel);
        return 0;
    }

    return 0;
}

