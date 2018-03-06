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

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/**
 * \brief Creates an empty composite model.
 *
 * @param name Composite model name.
 * @return model instance as opaque pointer.
 */
#ifdef __cplusplus
extern "C"
{
#endif
DLLEXPORT void* omtlm_newModel(const char *name);

/**
 * \brief Loads a composite model from xml representation.
 *
 * @param filename Full path to the composite model xml representation.
 * @return model instance as opaque pointer.
 */
DLLEXPORT void* omtlm_loadModel(const char* filename);


/**
 * \brief Unloads a composite model.
 *
 * @param pModel Model as opaque pointer.
 */
DLLEXPORT void omtlm_unloadModel(void* pModel);


/**
 * \brief Adds a sub-model to a composite model.
 *
 * @param pModel Model as opaque pointer.
 * @param interfaceName1 Name of first interface ("submodel.interface").
 * @param interfaceName2 Name of second interface ("submodel.interface").
 */
DLLEXPORT void omtlm_addSubModel(void *pModel,
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
DLLEXPORT void omtlm_addInterface(void* pModel,
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
DLLEXPORT void omtlm_addConnection(void *pModel,
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
DLLEXPORT void omtlm_addParameter(void *pModel,
                  const char *subModelName,
                  const char* parameterName,
                  const char* defaultValue);

/**
 * \brief Sets start time of simulation.
 *
 * @param pModel Model as opaque pointer.
 * @param startTime Start time of simulation.
 */
DLLEXPORT void omtlm_setStartTime(void *pModel, double startTime);

/**
 * \brief Sets stop time of simulation.
 *
 * @param pModel Model as opaque pointer.
 * @param startTime Stop time of simulation.
 */
DLLEXPORT void omtlm_setStopTime(void *pModel, double stopTime);

/**
 * \brief Enables or disables debug logging.
 *
 * @param pModel Model as opaque pointer.
 * @param debug Tells whether or not to use debug logging.
 */

DLLEXPORT void omtlm_setLogLevel(void *pModel, int logLevel);

/**
 * \brief Sets the address for the TLM Manager server
 *
 * @param pModel Model as opaque pointer.
 * @param address IP address to where manger process is running.
 */
DLLEXPORT void omtlm_setAddress(void *pModel, std::string address);

/**
 * \brief Sets manager port.
 *
 * @param pModel Model as opaque pointer.
 * @param port Manager port.
 */
DLLEXPORT void omtlm_setManagerPort(void *pModel, int port);

/**
 * \brief Sets monitorport.
 *
 * @param pModel Model as opaque pointer.
 * @param port Monitor port.
 */
DLLEXPORT void omtlm_setMonitorPort(void *pModel, int port);

/**
 * \brief Sets step size for logging.
 *
 * @param pModel Model as opaque pointer.
 * @param port Logging step size.
 */
DLLEXPORT void omtlm_setLogStepSize(void *pModel, double stepSize);

/**
 * \brief Sets number of log samples.
 *
 * @param pModel Model as opaque pointer.
 * @param port Number of log samples.
 */
DLLEXPORT void omtlm_setNumLogStep(void *pModel, int steps);

/**
 * \brief Simulates the model.
 *
 * @param pModel Model as opaque pointer.
 */
DLLEXPORT void omtlm_simulate(void* model);

/**
 * \brief Prints model structure to cout
 *
 * @param pModel Model as opaque pointer.
 */
DLLEXPORT void omtlm_printModelStructure(void* pModel);

#ifdef __cplusplus
}
#endif


#endif
