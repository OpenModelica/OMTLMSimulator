#ifndef HOPSANCOMMON_H
#define HOPSANCOMMON_H

#include <sstream>
#include "Logging/TLMErrorLog.h"

#define TLM_CONFIG_FILE_NAME "tlm.config"
#define TLM_DEBUG_FILE_NAME "TLMHopsan.log"

// TLM config data
struct tlmConfig_t {
    std::string model;
    std::string server;
    double tstart;
    double tend;
    double hmax;
};

inline tlmConfig_t readTlmConfigFile(std::string path)
{
    tlmConfig_t tlmConfig;
    std::ifstream tlmConfigFile(path.c_str());

    tlmConfigFile >> tlmConfig.model;
    tlmConfigFile >> tlmConfig.server;
    tlmConfigFile >> tlmConfig.tstart;
    tlmConfigFile >> tlmConfig.tend;
    tlmConfigFile >> tlmConfig.hmax;

    if(!tlmConfigFile.good()) {
      TLMErrorLog::FatalError("Error reading TLM configuration data from tlm.config");
      //exit(1);
    }

    //Print results to log file
    TLMErrorLog::Log("---"+std::string(TLM_CONFIG_FILE_NAME)+"---");
    TLMErrorLog::Log("model: "+tlmConfig.model);
    TLMErrorLog::Log("server: "+tlmConfig.server);
    std::stringstream ss1;
    ss1 << "tstart: " << tlmConfig.tstart;
    TLMErrorLog::Log(ss1.str());
    std::stringstream ss2;
    ss2 << "tend: " << tlmConfig.tend;
    TLMErrorLog::Log(ss2.str());
    std::stringstream ss3;
    ss3 << "hmax: " << tlmConfig.hmax;
    TLMErrorLog::Log(ss3.str());

    return tlmConfig;
}


#endif // HOPSANCOMMON_H
