#ifndef TLMFORCE_H
#define TLMFORCE_H
//! \file tlmforce.h
//!
//! The file provides declarations for the Modelica specific wrapper
//! function and classes to interface and TLMPlugin

#ifdef __cplusplus
// This part is compiled in C++ mode only

#include <map>
#include <string>
#include <fstream>
#include <iostream>
//! MarkerMotionData stores the motion data for a marker during
//! a call to force calculation. This is used for storing
//! the data and checking for converged steps.
struct MarkerMotionData {
    double Time;           //! Time stamp for the data
    double Position[3];    //! Cartesian position
    double Orientation[9]; //! Rotation matrix
    double Speed[3];       //! Translational velocity
    double Ang_speed[3];   //! Angular velocity

    //! SetData function sets all the attributes of the class
    void SetData( double time,          //! time stamp
                  double position[],    //! Cartesian postition, relative ground
                  double orientation[], //! rotation matrix 9 elements
                  double speed[],       //! translational speed global coordinate system
                  double ang_speed[]);  //! rotational speed local coordinate system

    //! Assignment operator
    const MarkerMotionData& operator=(const MarkerMotionData& src);

    //! Default constructor. Time is initialized to -1e50
    MarkerMotionData();

private:
    //! Copy constructor is not allowed
    MarkerMotionData( const MarkerMotionData&);

};

class TLMPlugin;


//! TLM_force is a wrapper class interfacing TLMPlugin from Modelica
//! There is just one instance of the class.
class TLM_force {

    //! Singleton instance
    static TLM_force* instance_;

    //! MarkerID containes information used to name the Marker
    //! within this class and in calls to TLMPlugin
    struct MarkerID {
        int ID;         //! Force ID as used by TLM Plugin
        int index;      //! Registration index within this class
    };

    //! MarkerIDmap maps the Modelica marker ID to registration index/ TLM force ID
    std::map<std::string, MarkerID> MarkerIDmap;
    
    //!  LastMarkerMotion is an array of saved MarkerMotion data. This is used
    //!  for checking converged steps and sending information to TLM plugin
    MarkerMotionData* LastMarkerMotion;

    //! Number of registered markers (TLM interfaces)
    int NumMarkers;

    //! Mode attribute tells if the initialization of the plugin is over
    //! and simulation is going on.
    //! It is initialized to 0 and then set to 1 to indicate running state.
    int Mode;

    //! getMode() returns the current value of Mode attribute
    int getMode() const { return Mode; }

    //! setMode() sets the Mode to 1, indicating the end of initialization stage.
    void setMode() { Mode = 1; }

    //! Plugin is the reference to TLM plugin instance
    TLMPlugin* Plugin;

    //! DebugOutFlg if true enables debug output
    bool DebugOutFlg;

    //! Simulation start time
    double timeStart;

    //! Simulation end time
    double timeEnd;

    //! Simulation max time step
    double maxStep;

    //! Time when TLM data was last set
    double lastSetMotionTime;
public:

    //! Write debug messages to debugOutFile.log
    std::ofstream debugOutFile;

    //! Constructor takes debugFlg as input which is used to set DebugOutFlg
    TLM_force(bool debugFlg);

    //! Destructor handles memory deallocation and graceful exit
    ~TLM_force();

    //! GetInstance creates this class instance if necessary and enables
    //! debug output when  debugFlg is true.
    static TLM_force* GetInstance(bool debugFlg);

    //! RegisterMarker registers an TLM interface with name M<markerID>
    //! markerID is the Modelica marker ID on the action part.
    //! Adds marker to internal map of ID's
    void RegisterMarker(const std::string markerID);

    //! Checks for already registered markers
    bool IsRegisteredMarker(const std::string markerID);

    //! SwitchToRunMode indicate that no more markers need to be registered
    //! and simulation is starting
    void SwitchToRunMode( );

    // Set DebugOutFlg to true
    void SetDebugOut(bool flg);

    //! Get the reaction force from TLM plugin
    void GetForce( std::string markerID, // Modelica marker ID on the action part
                   //double lastConvergedTime, // Last successful time step, used for storing data
                   MarkerMotionData& param,  // Current simulation time motion information
                   double* force);           // Output 6-component force

    //! Set point motion in TLM plugin
    void SetMotion( std::string markerID,
                    MarkerMotionData& param);

    //! Get TLM related parameters
    void GetTLMParameters(std::string markerID,
                          double& TLMDelay);

    //! Get simulation related parameters
    void GetSimParameters(std::string markerID,
                          double& startTime,
                          double& endTime);

    double getLastSetMotionTime(){ return lastSetMotionTime; }
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

void set_debug_mode(int debugFlgIn);

double get_tlm_delay(const char* markerID);

void get_sim_parameters(const char* markerID,
                        double* start_time,
                        double* end_time);

void set_tlm_motion(const char* markerID,   // The calling marker ID
                    double time,    // Current simulation time
                    const double position[], // Marker position data
                    const double orientation[], // Marker rotation matrix
                    const double speed[],      // Marker translational velocity
                    const double ang_speed[]);

// The calc_tlm_force function is called directly from the Modelica interface function
// It needs special declaration
// Output 6-component force
void calc_tlm_force(const char* markerID,   // The calling marker ID
                    double time,    // Current simulation time
                    //double lastConvergedTime, // Last converged time
                    const double position[], // Marker position data
                    const double orientation[], // Marker rotation matrix
                    const double speed[],      // Marker translational velocity
                    const double ang_speed[],
                    double force[],      // Output 3-component force
                    double torque[]);    // Output 3-component torque
#ifdef __cplusplus
}
#endif


#endif
