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

int main(int argc, char* argv[]) {
#ifndef USE_THREADS
#warning TLM manager requires pthreads to be compiled in. Use -DUSE_THREADS in the Makefile.head if neeeded.    
	TLMErrorLog::Error("tlmmanger was compiled without threads and is not usable.");
	exit(1);    
#endif


    if(  (argc != 2) &&
       !((argc == 3) && (strcmp(argv[1],"-d") == 0))
	 ) {
	TLMErrorLog::SetDebugOut(true);
	TLMErrorLog::Log("Usage: tlmmananger [-d] <metamodel>, where metamodel is a name of XML file.");
	exit(1);
    }

    char* fn = argv[argc-1];
    if(argc == 3) {
	TLMErrorLog::SetDebugOut(true);
    }
    

    // Create the meta model object
    MetaModel theModel;

    {
	// Create model reader for the model
	MetaModelReader modelReader(theModel);

	std::string inFile(fn);
	// read the XML file and build the model
	modelReader.ReadModel(inFile);
    }
    
    // Create manager object
    ManagerCommHandler manager(theModel);

    // Run the simulation
    manager.Run();
    
    return 0;
}
