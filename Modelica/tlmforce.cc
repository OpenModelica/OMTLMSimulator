#include "tlmforce.h"
#include "Plugin/TLMPlugin.h"
#include "Logging/TLMErrorLog.h"
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

// The wrapper expect TLM parameters in this file.
// Alternative implementation might use .acf file to set
// some extra variables that are read by the wrapper.
static const char* TLM_CONFIG_FILE_NAME = "tlm.config";

// To debug the TLM interface please enable the debug flag in TLM Modelica Interface Library.
// All the messages will then be written to tlmmodelica.log.
// To add debug message to the above mentioned file,
// please use the following syntax: debugOutFile<< "debug message" <<endl;
static const char* TLM_DEBUG_FILE_NAME = "tlmmodelica.log";

//! The TLM Plugin instance.
static TLMPlugin* Plugin = 0;

//! Debug enabled or disabled?
static bool debugFlg = false;

//! Write debug messages to tlmmodelica.log
std::ofstream debugOutFile;

//! MarkerIDmap maps the Modelica marker ID to registration index/ TLM force ID
std::map<std::string, int> MarkerIDmap;

#ifdef __cplusplus
extern "C" {
#endif

void initialize_TLM()
{
    // Avoid reinitialization!
    assert( Plugin == 0 );

    // Create the plugin
    Plugin = TLMPlugin::CreateInstance();

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

    if(debugFlg && !debugOutFile.is_open() ){
        debugOutFile.open(TLM_DEBUG_FILE_NAME);

        if( debugOutFile.good()){
            TLMErrorLog::SetOutStream(debugOutFile);
        }

        TLMErrorLog::SetLogLevel(TLMLogLevel::Debug);
    }

    if(!tlmConfigFile.good()) {
        debugOutFile << "Error reading TLM configuration data from tlm.config" << endl;
        exit(1);
    }

    if(! Plugin->Init( model,
                       timeStart,
                       timeEnd,
                       maxStep,
                       serverName)) {
        debugOutFile << "Error initializing the TLM plugin" << endl;
        exit(1);
    }
}


void initialize_interface(const char* markerID)
{
    // Check if interface is registered. If it's not, register it
    if( MarkerIDmap.find(markerID) == MarkerIDmap.end() ){
        if( Plugin == 0 ){
            initialize_TLM();
        }

        MarkerIDmap[markerID] = Plugin->RegisteTLMInterface(markerID);
    }
}


void set_debug_mode(int debugFlgIn)
{
    debugFlg = debugFlgIn;

    if( debugFlg ){
        debugOutFile.open(TLM_DEBUG_FILE_NAME);
        if(debugOutFile.good()){
            cerr << "Debug on" << endl;
            TLMErrorLog::SetOutStream(debugOutFile);
        }
        else{
            cerr << "Unable to open debug log " << TLM_DEBUG_FILE_NAME << endl;
            cerr << "Debug off" << endl;
            debugOutFile.close();
            debugFlg = false;
        }
    }
    else {
        debugOutFile.close();
        cerr << "Debug off" << endl;
    }

    if(debugFlg) {
        TLMErrorLog::SetLogLevel(TLMLogLevel::Debug);
    }
    else {
        TLMErrorLog::SetLogLevel(TLMLogLevel::Warning);
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

    debugOutFile << model << ": get_tlm_delay (" << res << ")" << endl;

    return res;
}

void set_tlm_motion(const char* markerID,   // The calling marker ID
                    double time,    // Current simulation time
                    double position[], // Marker position data
                    double orientation[], // Marker rotation matrix
                    double speed[],      // Marker translational velocity
                    double ang_speed[])
{
    if( MarkerIDmap.find(markerID) != MarkerIDmap.end() ){
        int interfaceID = MarkerIDmap[markerID];

        if( interfaceID >= 0 ){
            Plugin->SetMotion3D(interfaceID,          // Send data to the Plugin
                              time,
                              position,
                              orientation,
                              speed,
                              ang_speed);
        }

    }
    else {
        debugOutFile << "set_tlm_motion(...), called for non initialized interface " << markerID << endl;
    }
}

// The calc_tlm_force function is called directly from the Modelica interface function
// It needs special declaration
void calc_tlm_force(const char* markerID,   // The calling marker ID
                    double time,    // Current simulation time
                    //double lastConvergedTime, // Last converged time
                    double position[], // Marker position data
                    double orientation[], // Marker rotation matrix
                    double speed[],      // Marker translational velocity
                    double ang_speed[],
                    double force[],   // Output 3-component force
                    double torque[])  // Output 3-component torque
{
    double forceOut[6];
    int f, t;

    // Check if interface is registered. If it's not, register it
    if( MarkerIDmap.find(markerID) == MarkerIDmap.end() ){
        if( Plugin == 0 ){
            initialize_TLM();
        }

        MarkerIDmap[markerID] = Plugin->RegisteTLMInterface(markerID);
    }

    // Interface force ID in TLM manager
    int interfaceID = MarkerIDmap[markerID];

    if( interfaceID >= 0 ){
        // Call the plugin to get reaction force
        Plugin->GetForce3D(interfaceID,
                         time,
                         position,
                         orientation,
                         speed,
                         ang_speed,
                         forceOut);
    }
    else {
        /* Not connected */
        for( int i=0 ; i<6 ; i++ ) {
            force[i] = 0.0;
        }
    }


    // Copy results
    // NOTE, Modelica wants, for some reason, inverted forces???????
    //      (This might be a bug in the Modelica TLM implementation as well)
    for( f=0 ; f<3 ; f++ ) force[f] = -forceOut[f];
    for( t=0 ; t<3 ; t++ ) torque[t] = -forceOut[t+3];
    
}
#ifdef __cplusplus
}
#endif
