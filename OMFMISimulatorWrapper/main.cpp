#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>

#include "OMSimulator.h"
#include "Types.h"

// TLMPlugin includes
#include "Plugin/TLMPlugin.h"
#include "Logging/TLMErrorLog.h"
#include "common.h"

std::ofstream DebugOutFile;
#define TLM_DEBUG_FILE_NAME "TLMlogfile.log"

struct interfaceType {
  std::string name;
  oms_causality_t causality;
  std::string variable;
  int id;
};

struct optionsType {
  std::string workingDirectory = "";
  std::string fileName = "";
  double startTime = -1;
  double stepSize = -1;
  double stopTime = -1;
  std::vector<interfaceType> interfaces;
  std::string modelName;
  std::string tlmServer;

  void parseArguments(int argc, char *argv[])
  {
    if(argc != 3) {
      std::cout << "Wrong number of arguments.\n";
      std::cout << "Usage:\n";
      std::cout << "  OMFMISimulatorWrapper <workingDirectory> <modelFile> <startTime> <stepSize> <stopTime>\n";
      exit(-1);
    }

    workingDirectory = argv[1];
    fileName = argv[2];
  }

  void readTlmConfigFile()
  {
    std::ifstream tlmConfigFile(workingDirectory+"/tlm.config");

    tlmConfigFile >> modelName;
    tlmConfigFile >> tlmServer;
    tlmConfigFile >> startTime;
    tlmConfigFile >> stopTime;
    tlmConfigFile >> stepSize;

    if(!tlmConfigFile.good()) {
      std::cout << "Error reading TLM configuration data from tlm.config\n";
      exit(1);
    }
  }

  void print() {
    std::cout << "\nOptions: \n";
    std::cout << "   modelName        = " << modelName << "\n";
    std::cout << "   tlmServer        = " << tlmServer << "\n";
    std::cout << "   workingDirectory = " << workingDirectory << "\n";
    std::cout << "   fileName         = " << fileName << "\n";
    std::cout << "   startTime        = " << startTime << "\n";
    std::cout << "   stepSize         = " << stepSize << "\n";
    std::cout << "   stopTime         = " << stopTime << "\n";
    std::cout << "   interfaces       = ";
    for(size_t i=0; i<interfaces.size(); ++i) {
      std::cout << interfaces.at(i).causality << ": " << interfaces.at(i).name
                << "(" << interfaces.at(i).variable << ")\n";
      if(i != interfaces.size()-1)
        std::cout << "                      ";
    }
    std::cout << "\n";
  }
} options;



int main(int argc, char *argv[])
{
  /////////////////////
  // Parse arguments //
  /////////////////////

  std::cout << "Parsing arguments...\n";
  options.parseArguments(argc, argv);
  options.readTlmConfigFile();

  ////////////////
  // Load model //
  ////////////////

  std::cout << "Loading model...\n";
  void* pModel = NULL;
  oms_setWorkingDirectory(options.workingDirectory.c_str());
  oms_setTempDirectory(options.workingDirectory.c_str());
  pModel = oms_loadModel(options.fileName.c_str());

  //////////////////////////////////
  // Fetch input/output variables //
  //////////////////////////////////

  std::cout << "Fetching variable interfaces...\n";
  int nInterfaces = oms_getNumberOfInterfaces(pModel);
  for(int i=0; i<nInterfaces; ++i) {
    options.interfaces.push_back(interfaceType());
    options.interfaces.back().name = oms_getInterfaceName(pModel, i);
    options.interfaces.back().causality = oms_getInterfaceCausality(pModel, i);
    options.interfaces.back().variable = oms_getInterfaceVariable(pModel, i);
  }

  options.print();

  ///////////////////////////////
  // Instantiate the TLMPlugin //
  ///////////////////////////////

  std::cout << "Instantiating TLMPlugin...\n";
  TLMPlugin* plugin = TLMPlugin::CreateInstance();

  ////////////////////
  // Initialization //
  ////////////////////

  std::cout << "Initializing TLMPlugin...\n";
  if(!plugin->Init(options.modelName,
                   options.startTime,
                   options.stopTime,
                   options.stepSize,
                   options.tlmServer)) {
    TLMErrorLog::FatalError("Error initializing the TLM plugin.");
    exit(1);
  }

  std::cout << "Enabling debug output...\n";
  bool debug=true;
  if(debug) {
    DebugOutFile.open(TLM_DEBUG_FILE_NAME);

    if( DebugOutFile.good()){
      TLMErrorLog::SetOutStream(DebugOutFile);
    }

    TLMErrorLog::SetLogLevel(TLMLogLevel::Debug);
  }

  for(size_t i=0; i<options.interfaces.size(); ++i) {
    std::cout << "Registering interface...\n";
    if(options.interfaces.at(i).causality == oms_causality_parameter) {
      options.interfaces.at(i).id = plugin->RegisterComponentParameter(options.interfaces.at(i).name.c_str(),"0");
    }
    else if(options.interfaces.at(i).causality == oms_causality_input) {
      options.interfaces.at(i).id = plugin->RegisteTLMInterface(options.interfaces.at(i).name.c_str(),
                                                              1,
                                                              "Input",
                                                              "Signal");
    }
    else if(options.interfaces.at(i).causality == oms_causality_output) {
      options.interfaces.at(i).id = plugin->RegisteTLMInterface(options.interfaces.at(i).name.c_str(),
                                                              1,
                                                              "Output",
                                                              "Signal");
    }
  }

  std::cout << "Initializing OMFMISimulator...\n";
  oms_setResultFile(pModel, "logdata.csv");
  oms_setStartTime(pModel, options.startTime);
  oms_setCommunicationInterval(pModel, options.stepSize);
  oms_setStopTime(pModel, options.stopTime);
  oms_initialize(pModel);

  //Set parameters
  for(size_t i=0; i<options.interfaces.size(); ++i) {
    if(options.interfaces.at(i).causality == oms_causality_parameter) {
      std::string name;
      std::string value;
      plugin->GetParameterValue(options.interfaces.at(i).id, name, value);
      double value_real = atof(value.c_str());
      oms_setReal(pModel, options.interfaces.at(i).variable.c_str(), value_real);
    }
  }

  /////////////////////
  // Simulation loop //
  /////////////////////

  std::cout << "Simulating...\n";
  double time = options.startTime;
  while(time < options.stopTime) {
    for(size_t i=0; i<options.interfaces.size(); ++i) {
      if(options.interfaces.at(i).causality == oms_causality_input) {
        double value;
        plugin->GetValueSignal(options.interfaces.at(i).id, time, &value);
        oms_setReal(pModel, options.interfaces.at(i).variable.c_str(), value);
      }
    }

    time = time+options.stepSize;
    oms_stepUntil(pModel, time);

    for(size_t i=0; i<options.interfaces.size(); ++i) {
      if(options.interfaces.at(i).causality == oms_causality_output) {
        double value = oms_getReal(pModel, options.interfaces.at(i).variable.c_str());
        plugin->SetValueSignal(options.interfaces.at(i).id, time, value);
      }
    }
  }

  //////////////
  // Finalize //
  //////////////

  oms_terminate(pModel);

  plugin->AwaitClosePermission();

  std::cout << "Finished!\n";
}


