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

#include <map>

namespace OMTLMSimulator {

/**
 * \brief Creates an empty composite model.
 *
 * @param name Composite model name.
 * @return model instance as opaque pointer.
 */
void* newModel(const char *name);


/**
 * \brief Loads a composite model from xml representation.
 *
 * @param filename Full path to the composite model xml representation.
 * @return model instance as opaque pointer.
 */
void* loadModel(const char* filename);


/**
 * \brief Unloads a composite model.
 *
 * @param pModel Model as opaque pointer.
 */
void unloadModel(void* pModel);


/**
 * \brief Adds a sub-model to a composite model.
 *
 * @param pModel Model as opaque pointer.
 * @param interfaceName1 Name of first interface ("submodel.interface").
 * @param interfaceName2 Name of second interface ("submodel.interface").
 */
void addSubModel(void *pModel,
                 const char* name,
                 const char* file,
                 const char* startCommand);


/**
 * \brief Adds an interface to a sub-model.
 *
 * @param pModel Model as opaque pointer.
 * @param subModelName Name of sub-model.
 * @param name Name of second interface ("submodel.interface").
 */
void addInterface(void* pModel,
                  const char *subModelName,
                  const char *name,
                  int dimensions,
                  const char *causality,
                  const char *domain);

/**
 * \brief Adds a connection between two interfaces.
 *
 * @param pModel Model as opaque pointer.
 * @param interfaceName1 Name of first interface ("submodel.interface").
 * @param interfaceName2 Name of second interface ("submodel.interface").
 */
void addConnection(void *pModel,
                   const char *interfaceName1,
                   const char* interfaceName2,
                   double delay,
                   double Zf = 0.0,
                   double Zfr = 0.0,
                   double alpha = 0.0);

/**
 * \brief Adds (sets) a parameter to a sub-model.
 *
 * @param pModel Model as opaque pointer.
 * @param subModelName Name of sub-model.
 * @param name Name of parameter.
 * @param defaultValue Value of parameter.
 */
void addParameter(void *pModel,
                  const char *subModelName,
                  const char* parameterName,
                  const char* defaultValue);

/**
 * \brief Sets start time of simulation.
 *
 * @param pModel Model as opaque pointer.
 * @param startTime Start time of simulation.
 */
void setStartTime(void *pModel, double startTime);

/**
 * \brief Sets stop time of simulation.
 *
 * @param pModel Model as opaque pointer.
 * @param startTime Stop time of simulation.
 */
void setStopTime(void *pModel, double stopTime);

/**
 * \brief Enables or disables debug logging.
 *
 * @param pModel Model as opaque pointer.
 * @param debug Tells whether or not to use debug logging.
 */

void setLogLevel(void *pModel, int logLevel);

/**
 * \brief Sets the address for the TLM Manager server
 *
 * @param pModel Model as opaque pointer.
 * @param address IP address to where manger process is running.
 */
void setAddress(void *pModel, std::string address);

/**
 * \brief Sets manager port.
 *
 * @param pModel Model as opaque pointer.
 * @param port Manager port.
 */
void setManagerPort(void *pModel, int port);

/**
 * \brief Sets monitorport.
 *
 * @param pModel Model as opaque pointer.
 * @param port Monitor port.
 */
void setMonitorPort(void *pModel, int port);

/**
 * \brief Sets step size for logging.
 *
 * @param pModel Model as opaque pointer.
 * @param port Logging step size.
 */
void setLogStepSize(void *pModel, double stepSize);

/**
 * \brief Sets number of log samples.
 *
 * @param pModel Model as opaque pointer.
 * @param port Number of log samples.
 */
void setNumLogStep(void *pModel, int steps);

/**
 * \brief Simulates the model.
 *
 * @param pModel Model as opaque pointer.
 */
void simulate(void* model);

}

#endif
