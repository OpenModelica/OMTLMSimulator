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

 public:

    //! TLMInterface constructor. Sends a registration message to the TLM manager
    //! and prepares the object for simulation.
    TLMInterface(TLMClientComm& theComm, std::string& aName, double StartTime, std::string type);
    virtual ~TLMInterface();
    //! Indecates if the interface is finished and waits for shutdown.
    //! This is use for interface request mode and not simulation mode.
    bool waitForShutdown(){ return waitForShutdownFlg; }

    //! Get name of the interface
    const std::string& GetName() const { return Name; }

    //! Get type of the interface
    const std::string& GetType() const { return Type; }

    //! Send out motion data from the DataToSend vector
    virtual void SendAllData() = 0;

    //! Get interface ID of this interface
    int GetInterfaceID() const { return  InterfaceID; }

    //! Unpack time data from a Message
    virtual void UnpackTimeData(TLMMessage& mess) = 0;
    
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


        //! Hermite cubic interpolation. For the given 4 data points t[i], f[i] and time,
    //! such that t[0]<t[1]<time<t[2]<t[3], returns f(time). .
    static double hermite_interpolate(double time, double t[4], double f[4]);

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

    std::string Type;
};
#endif
