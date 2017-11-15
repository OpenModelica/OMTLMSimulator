#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>

#include "OMTLMSimulatorLib/OMTLMSimulatorLib.h"


std::ofstream DebugOutFile;
#define TLM_DEBUG_FILE_NAME "TLMlogfile.log"


struct optionsType {
  bool debug;
  std::string address;
  int manager;
  int monitor;
  bool interfaceRequest;
  std::string singleModel;
  std::string model;
  double timeStep = 0;
  int logSteps = 1000;

  bool addressSet = false;
  bool managerSet = false;
  bool monitorSet = false;
  bool modelSet = false;

  bool splitOption(std::string option, std::string &name, std::string &value) {
    std::stringstream optStream(option);

    if(!std::getline(optStream, name, '=')) return false;
    if(!std::getline(optStream, value, '=')) return false;

    return true;
  }

  void parseArguments(int argc, char *argv[])
  {
//    if(argc != 9) {
//      std::cout << "Wrong number of arguments.\n";
//      std::cout << "Usage:\n";
//      //std::cout << "  OMFMISimulatorWrapper <workingDirectory> <modelFile> <startTime> <stepSize> <stopTime>\n";
//      exit(-1);
//    }

    for(int i=1; i<argc; ++i) {
      std::string name, value;
      if(splitOption(argv[i],name,value)) {
        if(name == "address") {
          address = value;
          addressSet = true;
        }
        else if(name == "manager") {
          manager = stoi(value);
          managerSet = true;
        }
        else if(name == "monitor") {
          monitor = stoi(value);
          monitorSet = true;
        }
        else if(name == "model") {
          model = value;
          modelSet = true;
        }
        else if(name == "logsteps") {
          logSteps = stoi(value);
        }
        else if(name == "singlemodel") {
          singleModel = value;
        }
      }
      else if(argv[i] == "-d") {
        debug = true;
      }
      else if(argv[i] == "-r") {
        interfaceRequest = true;
      }
    }

    if(!addressSet) {
      std::cout << "Error: Address not specified.\n";
      exit(-1);
    }
    if(!managerSet) {
      std::cout << "Error: Manager port not specified.\n";
      exit(-1);
    }
    if(!monitorSet) {
      std::cout << "Error: Monitor port not specified.\n";
      exit(-1);
    }
    if(!modelSet) {
      std::cout << "Error: Model file not specified.\n";
      exit(-1);
    }
  }


  void print() {
    std::cout << "\nOptions: \n";
    std::cout << "   debug            = " << debug << "\n";
    std::cout << "   serverAddress    = " << address << "\n";
    std::cout << "   serverPort       = " << manager << "\n";
    std::cout << "   monitorPort      = " << monitor << "\n";
    std::cout << "   interfaceRequest = " << interfaceRequest << "\n";
    std::cout << "   singleModel      = " << singleModel << "\n";
    std::cout << "   modelFile        = " << model << "\n";
    std::cout << "   timeStep         = " << timeStep << "\n";
    std::cout << "   nLogSteps        = " << logSteps << "\n";

  }
} options;



int main(int argc, char *argv[])
{
  /////////////////////
  // Parse arguments //
  /////////////////////

  std::cout << "Parsing arguments...\n";
  options.parseArguments(argc, argv);
  options.print();

  //////////////////////
  // Start simulation //
  //////////////////////

  void* pModel = OMTLMSimulator::loadModel(options.model.c_str());

  OMTLMSimulator::simulate(pModel,
                           options.debug,
                           options.address,
                           options.manager,
                           options.monitor,
                           options.interfaceRequest,
                           options.singleModel,
                           options.timeStep,
                           options.logSteps);

      std::cout << "Finished!\n";
}


