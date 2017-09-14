#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include "OMSimulator.h"



struct argumentsType {
  std::string workingDirectory = "";
  std::string fileName = "";
  double startTime = -1;
  double stepSize = -1;
  double stopTime = -1;

  void parse(int argc, char *argv[])
  {
    if(argc != 6) {
      std::cout << "Wrong number of arguments.\n";
      std::cout << "Usage:\n";
      std::cout << "  OMFMISimulatorWrapper <workingDirectory> <modelFile> <startTime> <stepSize> <stopTime>\n";
      exit(-1);
    }

    workingDirectory = argv[1];
    fileName = argv[2];
    startTime = atof(argv[3]);
    stepSize = atof(argv[4]);
    stopTime = atof(argv[5]);
  }

  void print() {
    std::cout << "\nArguments: \n";
    std::cout << "----------\n";
    std::cout << "workingDirectory = " << workingDirectory << "\n";
    std::cout << "fileName         = " << fileName << "\n";
    std::cout << "startTime        = " << startTime << "\n";
    std::cout << "stepSize         = " << stepSize << "\n";
    std::cout << "stopTime         = " << stopTime << "\n\n";
  }
} arguments;



int main(int argc, char *argv[])
{
  arguments.parse(argc, argv);
  arguments.print();



  oms_setWorkingDirectory(arguments.workingDirectory.c_str());
  oms_setTempDirectory(arguments.workingDirectory.c_str());

  void* pModel = NULL;
  pModel = oms_loadModel(arguments.fileName.c_str());

  oms_setResultFile(pModel, "logdata.csv");
  oms_setStartTime(pModel, arguments.startTime);
  oms_setCommunicationInterval(pModel, arguments.stepSize);
  oms_setStopTime(pModel, arguments.stopTime);

  oms_initialize(pModel);
  double time = arguments.startTime;
  while(time < arguments.stopTime) {
    oms_doSteps(pModel, 1);
    oms_getCurrentTime(pModel, &time);
  }
  oms_terminate(pModel);
}


