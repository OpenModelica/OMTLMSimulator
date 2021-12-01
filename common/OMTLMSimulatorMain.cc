#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#ifdef _WIN32
#ifndef NOMINMAX
  #define NOMINMAX
#endif
#include <windows.h>
#endif

#include "OMTLMSimulatorLib/OMTLMSimulatorLib.h"


std::ofstream DebugOutFile;
#define TLM_DEBUG_FILE_NAME "TLMlogfile.log"


struct optionsType {
  int logLevel = 0;
  std::string address = "127.0.1.1";
  int manager = 11111;
  int monitor = 12111;
  bool interfaceRequest;
  std::string singleModel;
  std::string model = "";
  double logStepSize = 0;
  int numLogSteps = 1000;

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
#ifdef _WIN32
          //Note: Not yet tested on Windows!
          char real_path[4096] = "";
          DWORD ret_val = 0;
          ret_val = GetFullPathName(value.c_str(), 4096, real_path, NULL);
          model = std::string(real_path);
#else
          char *real_path = realpath(value.c_str(), NULL);
          if(real_path) {
            model = std::string(real_path);
          }
#endif
          modelSet = true;
        }
        else if(name == "logsteps") {
          numLogSteps = stoi(value);
        }
        else if(name == "singlemodel") {
          singleModel = value;
        }
        else if(name == "loglevel") {
          logLevel = stoi(value);
        }
      }
      else if(std::string(argv[i]) == "-r") {
        interfaceRequest = true;
      }
      else {
        std::cout << "Unknown argument: " << argv[i] << "\n";
      }
    }

    if(!addressSet) {
      std::cout << "Warning: Address not specified. Using 127.0.1.1 (default).\n";
      //exit(-1);
    }
    if(!managerSet) {
      std::cout << "Warning: Manager port not specified. Using 11111 (default).\n";
      //exit(-1);
    }
    if(!monitorSet) {
      std::cout << "Warning: Monitor port not specified. Using 12111 (default).\n";
      //exit(-1);
    }
    if(!modelSet) {
      std::cout << "Error: Model file not specified.\n";
      exit(-1);
    }

    FILE *modelFile = fopen(model.c_str(), "r");
    if (!modelFile) {
      std::cout << "Error: Model file does not exist or is not accessible: " << model << "\n";
      exit(-1);
    }
    fclose(modelFile);
  }


  void print() {
    std::cout << "\nOptions: \n";
    std::cout << "   logLevel         = " << logLevel << "\n";
    std::cout << "   serverAddress    = " << address << "\n";
    std::cout << "   serverPort       = " << manager << "\n";
    std::cout << "   monitorPort      = " << monitor << "\n";
    std::cout << "   interfaceRequest = " << interfaceRequest << "\n";
    std::cout << "   singleModel      = " << singleModel << "\n";
    std::cout << "   modelFile        = " << model << "\n";
    std::cout << "   timeStep         = " << logStepSize << "\n";
    std::cout << "   nLogSteps        = " << numLogSteps << "\n";

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

  void* pModel = omtlm_loadModel(options.model.c_str());
  omtlm_setLogLevel(pModel, options.logLevel);
/*
  void *pModel = omtlm_newModel("FmiTest");
  omtlm_addSubModel(pModel, "adder","/home/robbr48/Documents/Git/OMTLMSimulator/CompositeModels/FmiTestLinux/cs_adder1fmu1/cs_adder1.fmu", "StartTLMFmiWrapper");
  omtlm_addSubModel(pModel, "source1","/home/robbr48/Documents/Git/OMTLMSimulator/CompositeModels/FmiTestLinux/cs_source1fmu1/cs_source1.fmu", "StartTLMFmiWrapper");
  omtlm_addSubModel(pModel, "source2","/home/robbr48/Documents/Git/OMTLMSimulator/CompositeModels/FmiTestLinux/cs_source1fmu1/cs_source1.fmu", "StartTLMFmiWrapper");
  omtlm_addSubModel(pModel, "gain","/home/robbr48/Documents/Git/OMTLMSimulator/CompositeModels/FmiTestLinux/gainfmu1/gain.fmu", "StartTLMFmiWrapper");

  omtlm_addInterface(pModel,"adder","x1",1,"input","signal");
  omtlm_addInterface(pModel,"adder","x2",1,"input","signal");
  omtlm_addInterface(pModel,"adder","y",1,"output","signal");
  omtlm_addInterface(pModel,"source1","y",1,"output","signal");
  omtlm_addInterface(pModel,"source2","y",1,"output","signal");
  omtlm_addInterface(pModel,"gain","y",1,"input","signal");
  omtlm_addInterface(pModel,"gain","u",1,"output","signal");

  omtlm_addConnection(pModel, "adder.x1", "source2.y",0);
  omtlm_addConnection(pModel, "adder.x2", "source1.y",0);
  omtlm_addConnection(pModel, "gain.y", "adder.y",0);

  omtlm_addParameter(pModel,"source1","A","2");
  omtlm_addParameter(pModel,"source1","omega","3");
  omtlm_addParameter(pModel,"source2","A","1");
  omtlm_addParameter(pModel,"source2","omega","10");
  omtlm_addParameter(pModel,"gain","Kp","0.01");
  omtlm_addParameter(pModel,"gain","Ki","0.001");

  omtlm_setStartTime(   pModel, 0);
  omtlm_setStopTime(    pModel, 1);
  omtlm_setLogLevel(    pModel, options.logLevel);
  omtlm_setAddress(     pModel, options.address);
  omtlm_setManagerPort( pModel, options.manager);
  omtlm_setMonitorPort( pModel, options.monitor);
  omtlm_setNumLogStep(  pModel, options.numLogSteps);
  omtlm_setLogStepSize( pModel, options.logStepSize);
*/
  std::cout << "Starting simulation.\n";

  omtlm_simulate(pModel);

  std::cout << "Finished!\n";
}


