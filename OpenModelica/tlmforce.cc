#include "tlmforce.h"
#include "TLMPlugin.h"
#include "TLMErrorLog.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string.h>
#include <map>

using std::map;
using std::ifstream;
using std::string;
using std::endl;
using std::cerr;

typedef struct {
  TLMPlugin* Plugin;
  int referenceCount;
} TLMPluginStruct;

TLMPluginStruct* TLMPluginStructObj = 0;

// The wrapper expect TLM parameters in this file.
// Alternative implementation might use .acf file to set
// some extra variables that are read by the wrapper.
static const char* TLM_CONFIG_FILE_NAME = "tlm.config";

// To debug the TLM interface please enable the debug flag in TLM Modelica Interface Library.
// All the messages will then be written to tlmmodelica.log.
// To add debug message to the above mentioned file,
// please use the following syntax: debugOutFile<< "debug message" <<endl;
static const char* TLM_DEBUG_FILE_NAME = "tlmmodelica.log";

//! Debug enabled or disabled?
static bool debugFlg = false;

//! Write debug messages to tlmmodelica.log
std::ofstream debugOutFile;

//! MarkerIDmap maps the Modelica marker ID to registration index/ TLM force ID
std::map<std::string, int> MarkerIDmap;

//#define ALWAYSCALLSETMOTION
#ifdef ALWAYSCALLSETMOTION
//! Keep track of last time setMotion was called
double lastSetMotionTime = 0.0;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void* initialize_TLM()
{
    if (TLMPluginStructObj) {
	TLMPluginStructObj->referenceCount += 1;
	return (void*)TLMPluginStructObj;
    }
    set_debug_mode(debugFlg);

    TLMPluginStructObj = (TLMPluginStruct*)malloc(sizeof(TLMPluginStruct));
    // Create the plugin
    TLMPluginStructObj->Plugin = TLMPlugin::CreateInstance();
    TLMPluginStructObj->referenceCount = 1;

    // Read parameters from a file
    ifstream tlmConfigFile(TLM_CONFIG_FILE_NAME);

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
        TLMErrorLog::FatalError("Error reading TLM configuration data from tlm.config, exiting...");
        return 0;
    }

    if(! TLMPluginStructObj->Plugin->Init( model,
					   timeStart,
					   timeEnd,
					   maxStep,
					   serverName)) {
        TLMErrorLog::FatalError("Error initializing the TLM plugin, exiting...");
	return 0;
    }

    return (void*)TLMPluginStructObj;
}

void deinitialize_TLM(void* in_TLMPluginStructObj)
{
  TLMPluginStruct* TLMPluginStructObj = (TLMPluginStruct*)in_TLMPluginStructObj;
  if (TLMPluginStructObj->referenceCount == 1) {
      free(TLMPluginStructObj);
      TLMPluginStructObj = 0;
  } else {
      TLMPluginStructObj->referenceCount -= 1;
  }
}

void set_debug_mode(int debugFlgIn)
{
    debugFlg = debugFlgIn;

    TLMErrorLog::SetDebugOut(debugFlg);

    if( debugFlg ){
        if( !debugOutFile.is_open() ){
	    debugOutFile.open(TLM_DEBUG_FILE_NAME);
	    if( !debugOutFile.good() ){
	      TLMErrorLog::SetOutStream(debugOutFile);
	      TLMErrorLog::Log("Debug on");
	    }
	    else{
	      TLMErrorLog::Warning(std::string("Unable to open debug log ") + TLM_DEBUG_FILE_NAME );
	      TLMErrorLog::Warning("Debug off");
	      debugOutFile.close();
	      debugFlg = false;
	    }
	}
    }
    else {
        TLMErrorLog::Log("Debug off");
        debugOutFile.close();
    }
}

double get_tlm_delay()
{
    ifstream tlmConfigFile(TLM_CONFIG_FILE_NAME);

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

    double res = maxStep;
    tlmConfigFile.close();

    TLMErrorLog::Log(model + ": get_tlm_delay (" + TLMErrorLog::ToStdStr(res) + ")");

    return res;
}

void set_tlm_motion(void* in_TLMPluginStructObj,
		    const char* interfaceID,   // The calling marker ID
                    double simTime,    // Current simulation time
                    double position[], // Marker position data
                    double orientation[], // Marker rotation matrix
                    double speed[],      // Marker translational velocity
                    double ang_speed[])
{
    TLMPluginStruct* TLMPluginStructObj = (TLMPluginStruct*)in_TLMPluginStructObj;
    if( MarkerIDmap.find(interfaceID) != MarkerIDmap.end() ){
        int id = MarkerIDmap[interfaceID];

        if( id >= 0 ){
            TLMPluginStructObj->Plugin->SetMotion3D(id,          // Send data to the Plugin
						  simTime,
						  position,
						  orientation,
						  speed,
						  ang_speed);
        }

    }
    else {
        TLMErrorLog::Warning( "set_tlm_motion(...), called for non initialized interface " + std::string(interfaceID));
    }
}


void set_tlm_motion_1d(void* in_TLMPluginStructObj,
            const char* interfaceID,   // The calling marker ID
                    double simTime,    // Current simulation time
                    double position, // Marker position data
                    double speed)      // Marker translational velocity
{
    TLMPluginStruct* TLMPluginStructObj = (TLMPluginStruct*)in_TLMPluginStructObj;
    if( MarkerIDmap.find(interfaceID) != MarkerIDmap.end() ){
        int id = MarkerIDmap[interfaceID];

        if( id >= 0 ){
            TLMPluginStructObj->Plugin->SetMotion1D(id,          // Send data to the Plugin
                          simTime,
                          position,
                          speed);
        }

    }
    else {
        TLMErrorLog::Warning( "set_tlm_motion_1d(...), called for non initialized interface " + std::string(interfaceID));
    }
}


// The calc_tlm_force function is called directly from the Modelica interface function
// It needs special declaration
void calc_tlm_force(void* in_TLMPluginStructObj,
		    const char* interfaceID,   // The calling marker ID
                    double simTime,    // Current simulation time
                    //double lastConvergedTime, // Last converged time
                    double position[], // Marker position data
                    double orientation[], // Marker rotation matrix
                    double speed[],      // Marker translational velocity
                    double ang_speed[],
                    double force[],   // Output 3-component force
                    double torque[])  // Output 3-component torque
{
    TLMPluginStruct* TLMPluginStructObj = (TLMPluginStruct*)in_TLMPluginStructObj;
    double forceOut[6];
    int f, t;

    // defined in OpenModelica dassl.c
    extern int RHSFinalFlag;
    static bool firstFinalStepReached = false;

    if( RHSFinalFlag ){
      set_tlm_motion(TLMPluginStructObj, interfaceID, simTime, position, orientation, speed, ang_speed);
      firstFinalStepReached = true;
    }

    // Check if interface is registered. If it's not, register it
    if( MarkerIDmap.find(interfaceID) == MarkerIDmap.end() ){
        MarkerIDmap[interfaceID] = TLMPluginStructObj->Plugin->RegisteTLMInterface(interfaceID, "3D");
    }

    // Interface force ID in TLM manager
    int id = MarkerIDmap[interfaceID];

    // Note, we make sure that we do not use the interface before the first final RHS
    // so that all interfaces are registered before we start exchanging data. Also
    // This gives a well defined start condition where all forces and moments are 0.0
    if( id >= 0 && firstFinalStepReached ){
        // Call the plugin to get reaction force
	TLMPluginStructObj->Plugin->GetForce3D(id,
					     simTime,
					     position,
					     orientation,
					     speed,
					     ang_speed,
					     forceOut);
    }
    else {
        /* Not connected */
        for( int i=0 ; i<6 ; i++ ) {
            forceOut[i] = 0.0;
        }
    }


    // Copy results
    // NOTE, Modelica wants, for some reason, inverted forces???????
    //      (This might be a bug in the Modelica TLM implementation as well)
    for( f=0 ; f<3 ; f++ ) force[f] = -forceOut[f];
    for( t=0 ; t<3 ; t++ ) torque[t] = -forceOut[t+3];
}


// The calc_tlm_force function is called directly from the Modelica interface function
// It needs special declaration
void calc_tlm_force_1d(void* in_TLMPluginStructObj,
            const char* interfaceID,   // The calling marker ID
                    double simTime,    // Current simulation time
                    //double lastConvergedTime, // Last converged time
                    double position, // Marker position data
                    double speed,      // Marker translational velocity
                    double force[])   // Output force
{
    TLMPluginStruct* TLMPluginStructObj = (TLMPluginStruct*)in_TLMPluginStructObj;
    double forceOut[1];

    // defined in OpenModelica dassl.c
    extern int RHSFinalFlag;
    static bool firstFinalStepReached = false;

    if( RHSFinalFlag ){
      set_tlm_motion_1d(TLMPluginStructObj, interfaceID, simTime, position, speed);
      firstFinalStepReached = true;
    }

    // Check if interface is registered. If it's not, register it
    if( MarkerIDmap.find(interfaceID) == MarkerIDmap.end() ){
        MarkerIDmap[interfaceID] = TLMPluginStructObj->Plugin->RegisteTLMInterface(interfaceID, "1D");
    }

    // Interface force ID in TLM manager
    int id = MarkerIDmap[interfaceID];

    // Note, we make sure that we do not use the interface before the first final RHS
    // so that all interfaces are registered before we start exchanging data. Also
    // This gives a well defined start condition where all forces and moments are 0.0
    if( id >= 0 && firstFinalStepReached ){
        // Call the plugin to get reaction force
    TLMPluginStructObj->Plugin->GetForce1D(id,
                         simTime,
                         position,
                         speed,
                         forceOut);
    }
    else {
        /* Not connected */
        forceOut[0] = 0.0;

    }


    // Copy results
    // NOTE, Modelica wants, for some reason, inverted forces???????
    //      (This might be a bug in the Modelica TLM implementation as well)
   force[0] = -forceOut[0];
}


#ifdef __cplusplus
}
#endif
