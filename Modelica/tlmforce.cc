#include "tlmforce.h"
#include "TLMPlugin.h"
#include <string>
#include <strstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string.h>
#include <cmath>

using std::map;
using std::ifstream;
using std::string;
using std::strstream;
using std::endl;
//using std::cerr;

// The wrapper expect TLM parameters in this file.
// Alternative implementation might use .acf file to set
// some extra variables that are read by the wrapper.
static const char* TLM_CONFIG_FILE_NAME = "tlm.config";

//To debug the TLM interface please enable the debug flag in TLM Modelica Interface Library.
//All the messages will then be written to tlmmodelica.log.
//To add debug message to the above mentioned file,
//please use the following syntax: debugOutFile<< "debug message" <<endl;

MarkerMotionData::MarkerMotionData() {
    Time = -1e50;
    int i = 3;
    while(i > 0) {
        --i;
        Position[i] = 0;
        Speed[i] = 0;
        Ang_speed[i] =  0;
    }
    i = 9;
    while(i > 0) {
        --i;
        Orientation[i] = 0;
    }
}

void MarkerMotionData::SetData( double time,
                                double position[],
                                double orientation[],
                                double speed[],
                                double ang_speed[]) {
    Time = time;
    int i = 3;
    while(i > 0) {
        --i;
        Position[i] = position[i];
        Speed[i] = speed[i];
        Ang_speed[i] =  ang_speed[i];
    }
    i = 9;
    while(i > 0) {
        --i;
        Orientation[i] = orientation[i];
    }
}


const MarkerMotionData& MarkerMotionData::operator=(const MarkerMotionData& src)
{
    memcpy(this, &src, sizeof(MarkerMotionData));

    return *this;
}

TLM_force* TLM_force::instance_= 0;
static int init_calls = 0;

TLM_force::TLM_force(bool debugFlg):
    // instance_ // static variable, used as singleton
    MarkerIDmap(),
    LastMarkerMotion(0),
    NumMarkers(0),
    Mode(0),
    Plugin(TLMPlugin::CreateInstance()),
    DebugOutFlg(debugFlg),
    timeStart(0.0),
    timeEnd(0.0),
    maxStep(0.0),
    lastSetMotionTime(0.0),
    debugOutFile("tlmmodelica.log")
{

    if(debugFlg){
        debugOutFile<< "Debug on" <<endl;
    }else{
        debugOutFile<< "Debug off" <<endl;
    }
    if(instance_ != 0) {
        debugOutFile<< "Singleton pattern violated in TLM_force constructor" <<endl;
        exit(1);
    }


    if(debugFlg) SetDebugOut(debugFlg);

    instance_ = this;

    // Read parameters from a file

    ifstream tlmConfigFile(TLM_CONFIG_FILE_NAME);

    std::string model;
    std::string serverName;

    tlmConfigFile >> model;
    tlmConfigFile >> serverName;
    tlmConfigFile >> timeStart;
    tlmConfigFile >> timeEnd;
    tlmConfigFile >> maxStep;

    if(!tlmConfigFile.good()) {
        debugOutFile<<"Error reading TLM configuration data from tlm.config" <<endl;
        exit(1);
    }

    if(! Plugin->Init( model,
                       timeStart,
                       timeEnd,
                       maxStep,
                       serverName)) {
        debugOutFile<< "Error initializing the TLM plugin" <<endl;
        exit(1);
    }

}

TLM_force::~TLM_force() {
    delete [] LastMarkerMotion;
}

void TLM_force::SetDebugOut(bool flg){
    Plugin->SetDebugOut(flg);
}

TLM_force* TLM_force::GetInstance(bool debugFlg)
{
    if(!instance_) {
        instance_ = new TLM_force( debugFlg );
    }

    return instance_;
}

void TLM_force::GetForce( std::string markerID, 
                          //double lastConvergedTime,
                          MarkerMotionData& param,
                          double* force)
{
    MarkerID& mID = MarkerIDmap[markerID];
    int interfaceID = mID.ID; // interface force ID in TLM manager

    // All interfaces initialized?
    if(getMode() == 0){
        // Go to running mode.
        setMode();
    }
    
    if( interfaceID >= 0 ){
        // Call the plugin to get reaction force
        Plugin->GetForce(interfaceID,
                         param.Time,
                         param.Position,
                         param.Orientation,
                         param.Speed,
                         param.Ang_speed,
                         force);
    }
    else {
        /* Not connected */
        for( int i=0 ; i<6 ; i++ ) {
            force[i] = 0.0;
        }
    }
}

void TLM_force::SetMotion( std::string markerID, 
                           MarkerMotionData& param)
{
    MarkerID& mID = MarkerIDmap[markerID];
    int interfaceID = mID.ID; // interface force ID in TLM manager

    
    if( interfaceID >= 0 ){
        Plugin->SetMotion(interfaceID,          // Send data to the Plugin
                          param.Time,
                          param.Position,
                          param.Orientation,
                          param.Speed,
                          param.Ang_speed);

        lastSetMotionTime = param.Time;
    }
}


//! Get simulation and TLM related parameters
void TLM_force::GetTLMParameters( std::string markerID,
                                  double& TLMDelay)
{  

    //int interfaceID = Plugin->RegisteTLMInterface(markerID);
    MarkerID& mID = MarkerIDmap[markerID];
    int interfaceID = mID.ID; // interface force ID in TLM manager

    TLMConnectionParams ParamsOut;
    Plugin->GetConnectionParams(interfaceID, ParamsOut);

    TLMDelay = ParamsOut.Delay;
}

//! Get simulation and TLM related parameters
void TLM_force::GetSimParameters( std::string markerID,
                                  double& startTime,
                                  double& endTime)
{  
    startTime = timeStart;
    endTime = timeEnd;
}

void TLM_force::RegisterMarker(const std::string markerID) 
{    
    int interfaceID = Plugin->RegisteTLMInterface(markerID);

    MarkerID& mID = MarkerIDmap[markerID];
    mID.ID = interfaceID;
    mID.index = NumMarkers;
    
    NumMarkers++;
}

bool TLM_force::IsRegisteredMarker(const std::string markerID) 
{
    return MarkerIDmap.find(markerID) != MarkerIDmap.end();
}

void TLM_force::SwitchToRunMode(){
    LastMarkerMotion = new MarkerMotionData[NumMarkers];
    for(int i = 0; i < NumMarkers; i++) {
        LastMarkerMotion[i].Time = -1e50;
    }
}


#ifdef __cplusplus
extern "C" {
#endif

int DebugFlg = 1;

void set_debug_mode(int debugFlgIn)
{
    DebugFlg = debugFlgIn;

    if( DebugFlg ){
        fprintf(stderr, "Debug on\n");

    }
    else {
        fprintf(stderr, "Debug off\n");
    }
    TLM_force* inst = TLM_force::GetInstance(DebugFlg);
    inst->SetDebugOut(DebugFlg);

}

double get_tlm_delay(const char* markerID)
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

    fprintf(stderr, "%s: get_tlm_delay for interface %s (%f)\n", model.c_str(), markerID, res);

    return res;
}

void get_sim_parameters(const char* markerID,
                        double* start_time,
                        double* end_time)
{
    TLM_force* inst = TLM_force::GetInstance(false);
    inst->GetSimParameters(markerID, *start_time, *end_time);
}

void set_tlm_motion(const char* markerID,   // The calling marker ID
                    double time,    // Current simulation time
                    const double position[], // Marker position data
                    const double orientation[], // Marker rotation matrix
                    const double speed[],      // Marker translational velocity
                    const double ang_speed[])
{
    TLM_force* inst = TLM_force::GetInstance(DebugFlg);

    if(inst->IsRegisteredMarker(markerID)){

        MarkerMotionData param;
        param.SetData (time,
                       (double*)position,
                       (double*)orientation,
                       (double*)speed,
                       (double*)ang_speed);

        inst->SetMotion(markerID,
                        param);
    }
    else {
        fprintf(stderr, "ERROR in set_tlm_motion(...), called for non initialized interface %s\n", markerID );

    }
}

// The calc_tlm_force function is called directly from the Modelica interface function
// It needs special declaration
void calc_tlm_force(const char* markerID,   // The calling marker ID
                    double time,    // Current simulation time
                    //double lastConvergedTime, // Last converged time
                    const double position[], // Marker position data
                    const double orientation[], // Marker rotation matrix
                    const double speed[],      // Marker translational velocity
                    const double ang_speed[],
                    double force[],   // Output 3-component force
                    double torque[])  // Output 3-component torque
{
    double forceOut[6];
    int f, t;

    TLM_force* inst = TLM_force::GetInstance(DebugFlg);

    if(!inst->IsRegisteredMarker(markerID)) {
        // Register marker and return 0 force.

        inst->RegisterMarker(markerID);
        for(f=0; f<3 ; f++ ) force[f] = 0.0;
        for(t=0; t<3 ; t++ ) torque[t] = 0.0;

        return;
    }

    MarkerMotionData param;
    param.SetData (time,
                   (double*)position,
                   (double*)orientation,
                   (double*)speed,
                   (double*)ang_speed);

    inst->GetForce(markerID,
                   //lastConvergedTime,
                   param,
                   forceOut);



    // Copy results
    for( f=0 ; f<3 ; f++ ) force[f] = -forceOut[f];
    for( t=0 ; t<3 ; t++ ) torque[t] = -forceOut[t+3];
    
}
#ifdef __cplusplus
}
#endif
