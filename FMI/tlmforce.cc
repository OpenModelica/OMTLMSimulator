#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TLMPlugin.h"
#include "TLMErrorLog.h"
#include "tlmforce.h"


// The wrapper expect TLM parameters in this file.
// Alternative implementation might use .acf file to set
// some extra variables that are read by the wrapper.
static const char* TLM_CONFIG_FILE_NAME = "tlm.config";

// To debug the TLM interface please enable the debug flag in TLM Modelica Interface Library.
// All the messages will then be written to tlmmodelica.log.
// To add debug message to the above mentioned file,
// please use the following syntax: debugOutFile<< "debug message" <<endl;
static const char* TLM_DEBUG_FILE_NAME = "tlmmodelica.log";

struct MotionData {
    double Time;           //! Time stamp for the data
    double Position[3];    //! Cartesian position
    double Orientation[9]; //! Rotation matrix
    double Speed[3];       //! Translational velocity
    double Ang_speed[3];   //! Angular velocity
};

std::vector<MotionData*> vMotionData;

//extern "C" {

TLMPlugin *pPlugin;

void tlm_instantiate()
{
    pPlugin = TLMPlugin::CreateInstance();

    // Read parameters from a file
    std::ifstream tlmConfigFile(TLM_CONFIG_FILE_NAME);

    std::string model;
    std::string serverName;

    double timeStart;
    double timeEnd;
    double maxStep;

    tlmConfigFile >> model;
    tlmConfigFile >> serverName;
    tlmConfigFile >> timeStart;
    tlmConfigFile >> timeEnd;
    tlmConfigFile >> maxStep;

    if(!tlmConfigFile.good()) {
        //TLMErrorLog::FatalError("Error reading TLM configuration data from tlm.config, exiting...");
        return;
    }

    if(! pPlugin->Init( model,
         timeStart,
         timeEnd,
         maxStep,
         serverName)) {
        //TLMErrorLog::FatalError("Error initializing the TLM plugin, exiting...");
    }
}

//}
