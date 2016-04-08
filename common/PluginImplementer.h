//! 
//! File: PluginImplementer.h
//! Provides the implementation class for the TLMPlugin interface.
//!
#ifndef PluginImplementer_h_
#define  PluginImplementer_h_

#include <vector>
#include <map>
#include "TLMClientComm.h"
#include "TLMInterface.h"
#include "TLMPlugin.h"


//! PluginImplemneter class implements the  TLMPlugin interface.
class PluginImplementer: public TLMPlugin {

 public:
    //! Constructor
    PluginImplementer();

    //! Destructor
    ~PluginImplementer();

    //! Catch signals
    void signalHandler(int signum);

protected:

    //! SetDebugOut function enables/disables debug information
    //! output from TLM plugin. 
    //! Input: if Enable is true - output is on, othewise - off.
    virtual void SetDebugOut(bool Enable) { TLMErrorLog::SetDebugOut( Enable); }

    //! SetWarningOut function enables/disables warning information
    //! output from TLM plugin.
    //! Input: if Enable is true - output is on, othewise - off.
    virtual void SetWarningOut(bool Enable) { TLMErrorLog::SetWarningOut(Enable); }

    //! Connected flag tells if the connection to TLM manager is established.
    bool Connected;

    //! Check if connection to TLM manager is established (after Init success)
    bool IsConnected() const { return Connected; }

    //! Checked flag tells if the manager confirmed start of a simulation
    bool ModelChecked;

    //! CheckModel method results in CheckModel request sent to TLM manager.
    //! The successful return indicates that the simulation is ready to run.
    void CheckModel();

    //! Registered interfaces
    std::vector<TLMInterface*> Interfaces;
    
    //! The communication object
    TLMClientComm ClientComm;

    //! The message object used as a buffer
    TLMMessage Message;

    //! MapID2Ind provides a mapping between the ID of interfaces
    //!  and their index in the Interfaces vector
    std::map<int, int> MapID2Ind;

    int GetInterfaceIndex(int ID) const { return MapID2Ind.find(ID)->second; }

    //! Init method. Should be called after the default constructor. It will
    //! initialize the object and connect to TLMManager. Will return true
    //! on success, false otherwize. Note that the method can be called
    //! only once.
    virtual bool Init( std::string model,
                       double timeStart,
                       double timeEnd,
                       double maxStep,
                       std::string ServerName);


    //! Register TLM interface sends a registration request to TLMManager
    //! and returns the ID for the interface. '-1' is returned if
    //! the interface is not connected in the MetaModel.
    int RegisteTLMInterface( std::string name);

    //! ReceiveTimeData receives time-stamped data from coupled simulations
    //! if the specified interface needs more data for the given time..
    //! Since the order of messages can vary the specified interface
    //! is used only to detect the last message expected when the function
    //! is called. Any number of messages to other interfaces can arrive
    //! before the desired message is received.
    //! Input:
    //!   reqIfc - TLM interface that triggered the request;
    //!   time - time needed
    virtual void ReceiveTimeData(TLMInterface* reqIfc, double time);

    //! Evaluate the reaction force from the TLM connection
    //! for a specified interface. Might need to receive messages from the 
    //! TLM manager with TimeData.
    //! Input:
    //!  \param interfaceID - ID of an interface registered with RegisteTLMInterface
    //!  \param  time - current simulation time. Must be between timeStart & timeEnd
    //!   \param position - arrays of 3 doubles, giving position of the interface point
    //!   \param orientation - 3x3 array giving rotation matrix for the interface point
    //!   \param speed - arrays of 3 doubles, giving linear speed of the interface point
    //!   \param ang_speed - arrays of 3 doubles, giving rotational speed of the interface
    //! Output:
    //!    \param force - returns 6 doubles giving force & torque at the interface.
    //!  \note Global coordinate system common for the whole meta model is assumed
    //!   for all vectors.
    void GetForce(int interfaceID,
		  double time,
		  double position[],
		  double orientation[],
		  double speed[],
		  double ang_speed[],
		  double* force) ;

    //! Set the motion of a TLM interface after a successful integration step.
    //! The information is eventually sent to the TLM manager and forwarded
    //! to the coupled simulation.
    //! Input:
    //!  See comments to GetForce method
    void SetMotion(int forceID,
		   double time,
		   double position[],
		   double orientation[],
		   double speed[],
		   double ang_speed[]);

    //! GetConnectionParams returnes the ConnectionParams for
    //! the specified interface ID. Interface must be registered
    //! first.
    virtual void GetConnectionParams(int interfaceID, TLMConnectionParams& ParamsOut);

    //! GetTimeData returnes the necessary time stamped information needed
    //! for the calculation of the reaction force at a given time.
    //! The function might result in a request sent to TLM manager.
    virtual void GetTimeData(int interfaceID, double time, TLMTimeData& DataOut);

 protected:

    //! StartTime - start time for the simulation
    double StartTime;
    
    //! End time for the simulation
    double EndTime;

    //! Maximum solver time step
    double MaxStep;

};

#endif
