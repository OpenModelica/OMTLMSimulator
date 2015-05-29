//!
//! \file TLMPlugin.h
//! Provides the API for client applications working with 
//! TLM manager.
//!
//! \author   Iakov Nakhimovski
//!
#ifndef TLMPlugin_h_
#define TLMPlugin_h_

#include "TLMCalcData.h"

#include <string>

#ifndef MSC_VER
// This is because there are too many virtual functions that have trivial body
// and we wanted their parameters be nicely displayed.
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

//!
//! \class TLMPlugin 
//! This class provides an abstract interface for the client
//! applications participating in TLM based co-simulation
//!
class TLMPlugin {


 public: 

    //! CreateInstance static "factory" method returns 
    //! a pointer to the object implementing the interface.
    //! See PluginImplementer.cc for definition.
    static TLMPlugin* CreateInstance();
    
    //! Destructor. Close TCP/IP connection and release memory. 
    virtual ~TLMPlugin() {};

    //! SetDebugOut function enables/disables debug information
    //! output from TLM plugin. 
    //! Input: if Enable is true - output is on, othewise - off.
    virtual void SetDebugOut(bool Enable) = 0;

    //! Init method. Should be called after the default constructor. It will
    //! initialize the object and connect to TLMManager. Will return true
    //! on success, false otherwize. \note that the method can be called
    //! only once.
    //! \param model name of the component
    //! \param timeStart start time for the simulation
    //! \param timeEnd end time for the simulation 
    //! \param maxStep maximum step of the solver
    //! \param serverName IP address and port of the computer running TLM manager
    //!        separated by colon (e.g., 198.111.123.2:1111)
    virtual bool Init( std::string model,
	       double timeStart,
	       double timeEnd,
	       double maxStep,
	       std::string serverName) = 0;

    //! Register TLM interface sends a registration request to TLMManager
    //! and returns the ID for the interface. '-1' is returned if
    //! the interface is not connected in the MetaModel.
    //! \param name Name of the TLM interface
    virtual int RegisteTLMInterface( std::string name) = 0;

    //! Evaluate the reaction force from the TLM connection
    //! for a specified interface. This function might result in a request sent
    //! to the TLM manager.
    //! Input:
    //!  \param interfaceID - ID of an interface registered with RegisteTLMInterface
    //!  \param  time - current simulation time. Must be between timeStart & timeEnd
    //! \param   position - arrays of 3 doubles, giving position of the interface point
    //!   \param orientation - 3x3 array giving rotation matrix for the interface point
    //!   \param speed - arrays of 3 doubles, giving linear speed of the interface point
    //!   \param ang_speed - arrays of 3 doubles, giving rotational speed of the interface
    //! Output:
    //!    \param force - returns 6 doubles giving force & torque at the interface.
    //!  \note Global coordinate system common for the whole meta model is assumed
    //!   for all vectors.
    virtual void GetForce(int interfaceID,
			  double time,
			  double position[],
			  double orientation[],
			  double speed[],
			  double ang_speed[],
			  double* force)  = 0;

    //! Set the motion of a TLM interface after a successful integration step.
    //! The information is eventually sent to the TLM manager and forwarded
    //! to the coupled simulation.
    //! Input:
    //!  See comments to GetForce method
    virtual void SetMotion(int forceID,
			   double time,
			   double position[],
			   double orientation[],
			   double speed[],
			   double ang_speed[]) = 0;

    //! Check if the object is initialized (Init was called).
    bool IsInitialized() const { return Initialized; }

    //! \par Support functions for distributed applications (like Beast_MPP)
    //!
    //! The following functions are to be used by distributed
    //! applications where TLMPlugin is constructed on one
    //! host (Master) and the force calculations are performed 
    //! on another host. 
    //! In such a setup the GetConnectionParams & GetTimeData
    //! should be called on the Master host and TLMConnectionParams
    //! together with  TLMTimeData should be send out to
    //! other hosts. The static GetForce function defined in TLMCalcData 
    //! class should be used to calculate the resulting force.
    
    //! GetConnectionParams returnes the ConnectionParams for
    //! the specified interface ID. Interface must be registered
    //! first.
    virtual void GetConnectionParams(int interfaceID, TLMConnectionParams& ParamsOut) = 0;

    //! GetTimeData returnes the necessary time stamped information needed
    //! for the calculation of the reaction force at a given time.
    //! The function might result in a request sent to TLM manager.
    virtual void GetTimeData(int interfaceID, double time, TLMTimeData& DataOut) = 0;

    //! The static GetForce function is a pure function that uses
    //! parameters as defined for the GetForce function above.
    //! Additional parameters are obtained with GetConnectionParams & GetTimeData
    static void GetForce(double position[],
			 double orientation[],
			 double speed[],
			 double ang_speed[],
			 TLMTimeData& Data,
			 TLMConnectionParams& Params,
			 double* force);    

    //! TIME_WITHOUT_DATA is a large negative constant used to
    //! indicate lack of data during simulation startup.
    static const int TIME_WITHOUT_DATA = -111111;

 protected:

    //! Set the initialized flag
    void SetInitialized(bool on = true) { Initialized = on; }

 private:

    //! Initialized flag tells if the init method was called
    bool Initialized;

    //! Name of this component in the MetaModel
    std::string CompName;
    
    //! Start time for the simulation
    double TimeStart;

    //! End time for the simulation
    double TimeEnd;

    //! Maximum solver time step
    double MaxStep;

    //! Name of the TLM mananger server (\<server>:\<port>)
    std::string ServerName;

};
#endif
