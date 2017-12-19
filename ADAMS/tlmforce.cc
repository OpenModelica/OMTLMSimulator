#include "tlmforce.h"
#include "Plugin/TLMPlugin.h"
#include <string>
#include <strstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

using std::map;
using std::ifstream;
using std::string;
using std::strstream;
using std::endl;
using std::cerr;

// The wrapper expect TLM parameters in this file.
// Alternative implementation might use .acf file to set
// some extra variables that are read by the wrapper.
static const char* TLM_CONFIG_FILE_NAME = "tlm.config";

// Debug messages from within the plugin will go into this file
static const char* TLM_ADAMS_LOG_FILE_NAME = "tlmadams.log";

// File stream for the log
static std::ofstream* MyMess = 0;


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

TLM_force::TLM_force(bool debugFlg):
    // instance_ // static variable, used as singleton
    MarkerIDmap(),
    LastMarkerMotion(0),
    NumMarkers(0),
    Mode(0),
    Plugin(TLMPlugin::CreateInstance()),
    DebugOutFlg(debugFlg)
{
    if(instance_ != 0) {
	cerr << "Singleton pattern violated in TLM_force constructor" << endl;
	exit(1);
    }

    if(debugFlg) SetDebugOut();

    instance_ = this;

    // Read parameters from a file

    ifstream tlmConfigFile(TLM_CONFIG_FILE_NAME);

    std::string model;
    double timeStart;
    double timeEnd;
    double maxStep;
    std::string serverName;

    tlmConfigFile >> model;
    tlmConfigFile >> timeStart;
    tlmConfigFile >> timeEnd;
    tlmConfigFile >> maxStep;
    tlmConfigFile >> serverName;

    if(!tlmConfigFile.good()) {
	std::cerr << "Error reading TLM configuration data from tlm.config" << std::endl;
	exit(1);
    }

    std::cerr << "TLM plugin was initialized" << std::endl;

    if(! Plugin->Init( model,
		       timeStart,
		       timeEnd,
		       maxStep,
		       serverName)) {
	std::cerr << "Error initializing the TLM plugin" << std::endl;
	exit(1);
    }
    std::cerr << "TLM plugin was initialized" << std::endl;   
}

TLM_force::~TLM_force() {
    delete [] LastMarkerMotion;
}

void TLM_force::SetDebugOut(){
    TLMErrorLog::SetLogLevel(TLMLogLevel::Debug);
}

TLM_force* TLM_force::GetInstance(bool debugFlg)
{
    if(!instance_) {
	instance_ = new TLM_force( debugFlg);
    }
    return instance_;
}

void TLM_force::GetForce(bool derCalc, int markerID, 
			 double lastConvergedTime,
			 MarkerMotionData& param,
			 double* force) {
    if(getMode() == 0) {
	// init has been called for all
	setMode();
	SwitchToRunMode();
    }

    MarkerID& mID = MarkerIDmap[markerID];

    int interfaceID = mID.ID; // interface force ID in TLM manager

    MarkerMotionData& lastParam = LastMarkerMotion[mID.index];

    if(DebugOutFlg) {
	cerr << "Checking for interface " << interfaceID 
	     << " time " << param.Time << endl
	     << " converged time " << lastConvergedTime << endl
	     << " last time " << lastParam.Time << endl;
    }

    if(!derCalc) { // if it's a normal call (not Jacobian)
	if( (lastParam.Time >= 0 ) // there's data
	    && (lastParam.Time != param.Time ) //not a repeated call
	    ) { 
	    
	    if(lastConvergedTime == lastParam.Time) { // that was a converged step 
		map<int, MarkerID>::iterator it;
		for(it = MarkerIDmap.begin(); it != MarkerIDmap.end();++it) {
		    int curID = it->second.ID;
		    int index = it->second.index;
		    MarkerMotionData& toSend = LastMarkerMotion[index];
		    Plugin->SetMotion(curID,          // Send data to the Plugin
				  toSend.Time,
				  toSend.Position,
				  toSend.Orientation,
				  toSend.Speed,
				  toSend.Ang_speed);
		    // invalidate time to avoid resend
		    toSend.Time = param.Time;
		}
	    }
	}

	lastParam = param; // store the current motion data
    }

    // Call the plugin to get reaction force
    Plugin->GetForce3D(interfaceID,
		     param.Time,
		     param.Position,
		     param.Orientation,
		     param.Speed,
		     param.Ang_speed,
		     force);
}

void TLM_force::RegisterMarker(int markerID) {
    
    // No way to get the real marker name from the solver - using "M<ID>"
    strstream convert;
    convert << 'M' << markerID << '\0';
    string markerName(convert.str());

    cerr << "Trying to register interface " << markerName << endl;

    int interfaceID = Plugin->RegisteTLMInterface(markerName);

    MarkerID& mID = MarkerIDmap[markerID];
    mID.ID = interfaceID;
    mID.index = NumMarkers;
    
    NumMarkers++;
}

void TLM_force::SwitchToRunMode(){
    LastMarkerMotion = new MarkerMotionData[NumMarkers];
    for(int i = 0; i < NumMarkers; i++) {
	LastMarkerMotion[i].Time = 1e-50;
    }
}


PUBLIC VOID_FUNCTION calc_tlm_force(const int initFlg, // If set, it is an initialization call. Register marker.
				    const int derFlg,  // If set, it is derivatives calculation.
				                    // Time stands still. 
				    const int dbgOutFlg,
				    int markerID,   // The calling marker ID
				    double time,    // Current simulation time
				    double lastConvergedTime, // Last converged time 
				    double position[], // Marker position data
				    double orientation[], // Marker rotation matrix
				    double speed[],      // Marker translational velocity
				    double ang_speed[],  // Marker angular velocity
				    double* force)      // Output 6-component force
{
  
    if(dbgOutFlg && (MyMess == 0)) {
	MyMess = new std::ofstream(TLM_ADAMS_LOG_FILE_NAME);
	cerr.rdbuf(MyMess->rdbuf()); // note that Plugin is actually using cerr
    }

    TLM_force* inst = TLM_force::GetInstance(dbgOutFlg);

    if(initFlg) {
	inst->RegisterMarker(markerID);
	return ;
    }
    
    MarkerMotionData param;
    param.SetData (time, position, orientation, speed, ang_speed);

    inst->GetForce(derFlg, markerID, 
		   lastConvergedTime,
		   param,
		   force);

    return ;
}

