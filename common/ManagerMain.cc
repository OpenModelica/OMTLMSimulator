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

using std::string;

void usage(){
    string usageStr = "Usage: tlmmananger [-d] [-m <monitor-port>] [-p server-port>] <metamodel>, where metamodel is a name of XML file.";
    TLMErrorLog::SetDebugOut(true);
    TLMErrorLog::Log(usageStr);
    cout << usageStr << endl;
    exit(1);
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

    char c;
    while ((c = getopt (argc, argv, "dp:m:")) != -1){
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
        modelReader.ReadModel(inFile);
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
    manager.Run();
    
    return 0;
}
