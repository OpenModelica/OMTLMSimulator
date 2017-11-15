//!
//! \file OMTLMSimulatorLib.h
//!
//! Provides an API for OMTLMSimulator
//!
//!
//! \author Robert Braun
//!

#ifndef OMTLMSIMULATORLIB_H
#define OMTLMSIMULATORLIB_H

namespace OMTLMSimulator {

void* loadModel(const char* filename, bool interfaceRequest=false, const char* singleModel="");

bool simulate(void* model,
              bool debug,
              std::string serverAddress,
              int serverPort,
              int monitorPort,
              bool interfaceRequest,
              std::string singleModel,
              double timeStep,
              int nLogSTeps);

}

#endif
