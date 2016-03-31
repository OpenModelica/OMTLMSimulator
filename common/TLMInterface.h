//!
//! \file TLMInterface.h
//! 
//! Provides a definition for the TLMInterface class
//!
//!
//! \author   Iakov Nakhimovski
//!
#ifndef TLMInterface_h_
#define TLMInterface_h_

#include <queue>
#include <vector>
#include <string>
#include "TLMCommUtil.h"
#include "TLMClientComm.h"

//! 
//! TLMInterface provides the client side functionality for a single TLM interface
//! 
class TLMInterface {

    //!  TimeData is the deque of data received from the coupled simulation.
    //!  The data is "pushed back" when received and "poped front" when the
    //!  time goes forward more than  TLM delay and old data is not needed any longer.
    std::deque<TLMTimeData> TimeData;

    //!  DampedTimeData is the deque of data computed using damping coefficient alfa
    //!  from TimeData.
    //!  The data is "pushed back" when computed and "poped front" when the
    //!  time goes forward more than TLM delay and old data is not needed any longer.
    std::deque<TLMTimeData> DampedTimeData;

    //!  DataToSend stores the motion data from the interface. The data is sent
    //! in packet for a time period of [half] TLM delay [depends on solver type]
    std::vector<TLMTimeData> DataToSend;
    
    //! Last time when the data was sent
    double LastSendTime;

    //! Next time when we don't have data for interpolation and need to wait for
    //! the information from the couple simulation.
    double NextRecvTime;

    //! Message buffer used to transfer information between different methods
    TLMMessage Message;

    //! Parameters of the TLM connection attached to this interface
    TLMConnectionParams Params;

    //! CurrentIntervalIndex is the last offset in TimeData deque used for 
    //! interpolation. It is used to speedup search in deque.
    int CurrentIntervalIndex;

    //! Name of this TLM interface
    std::string Name;

    //! The communication object of the client
    TLMClientComm& Comm;

    //! TLM ID of this interface as received from the TLM manager
    int InterfaceID;

    //! Indecates that the interface is finished and waits for shutdown.
    //! This is use for interface request mode and not simulation mode.
    bool waitForShutdownFlg;

    //! Evaluate the data from deque for the time specified by this Instance
    //! If OnleForce is set, then the position and velocity are not computed.
    void GetTimeData(TLMTimeData& Instance, std::deque<TLMTimeData>&  Data, bool OnlyForce);

 public:

    //! TLMInterface constructor. Sends a registration message to the TLM manager
    //! and prepares the object for simulation.
    TLMInterface(TLMClientComm& theComm, std::string& aName, double StartTime);

    //! Destructor. Sends the rest of the data if necessary.
    ~TLMInterface();

    //! Indecates if the interface is finished and waits for shutdown.
    //! This is use for interface request mode and not simulation mode.
    bool waitForShutdown(){ return waitForShutdownFlg; }

    //! Get name of the interface
    const std::string& GetName() const { return Name; }

    //! Evaluate the data for the time specified by this Instance
    void GetTimeData(TLMTimeData& Instance);

    //! Evaluate the reaction force for the time & motion specified.
    //! Note that communication with TLM Manager should not be necessary
    void GetForce( double time,
		   double position[],
		   double orientation[],
		   double speed[],
		   double ang_speed[],
		   double* force);

    //! Set motion data.
    void SetTimeData(TLMTimeData& Instance);

    //! Set motion data and communicate if necessary.
    void SetTimeData(double time,
		     double position[],
		     double orientation[],
		     double speed[],
		     double ang_speed[]);
    
    //! Send out motion data from the DataToSend vector
    void SendAllData();

    //! Get interface ID of this interface
    int GetInterfaceID() const { return  InterfaceID; }

    //! Unpack time data from a Message
    void UnpackTimeData(TLMMessage& mess);
    
    //! Get the last possible time for interpolation
    double GetNextRecvTime() const { return NextRecvTime; }

    //! Get the last time when the data was sent
    double GetLastSendTime() const { return LastSendTime; }

    //! Get parameters for the TLM connection attached to the interface
    const TLMConnectionParams& GetConnParams() const { return Params; }

 protected:
    
    //! Linear interpolation (can be used for linear extrapolation as well)
    //! returns f(time) = ((time - t[0]) * f[1] - (time - t[1]) * f[0]) /( t[1] - t[0])
    inline static double linear_interpolate(double time, 
					    double t0, double t1, 
					    double f0, double f1){
	return ((time - t0) * f1 - (time - t1) * f0) / ( t1 - t0);
    }

    //! hermite_interpolate is called with a vector containing 4 points
    //! computes the interpolation point with the the polynomial that
    //! interpolates point 2 and 3 and have the derivative in these points
    //! equal to the center difference approximation at these points.
    //! The points are submitted using the iterator 'it' giving the 
    //! first point in the sequence. The desired time is given
    //! by the Instance.time. Results are stored in Instance.
    //! If OnleForce is set, then the position and velocity are not computed.
    static void hermite_interpolate(TLMTimeData& Instance, std::deque<TLMTimeData>::iterator& it, bool OnlyForce);

    //! linear_interpolate is called with a vector containing 2 points
    //! computes the interpolation (or extrapolation) point with the the linear
    //! interpolation (extrapolation) The points are submitted using the p0 & p1
    //!  The desired time is given by the Instance.time. Results are stored in Instance.
    //! If OnleForce is set, then the position and velocity are not computed.
    static void linear_interpolate(TLMTimeData& Instance, TLMTimeData& p0, TLMTimeData& p1, bool OnlyForce);

    //! Hermite cubic interpolation. For the given 4 data points t[i], f[i] and time,
    //! such that t[0]<t[1]<time<t[2]<t[3], returns f(time). .
    static double hermite_interpolate(double time, double t[4], double f[4]);

    // Remove the data that is not needed (Simulation time moved forward)
    // We leave two time points intact, so that interpolation work
    static void clean_time_queue(std::deque<TLMTimeData>& Data, double CleanTime);

    void TransformTimeDataToCG(std::vector<TLMTimeData>& timeData, TLMConnectionParams& params);
};
#endif
