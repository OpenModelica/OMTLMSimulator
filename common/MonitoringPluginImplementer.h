//!
//! File: MonitoringPluginImplementer.h
//! Provides one implementation class for the TLMPlugin interface that is
//! useful for a monitoring process.
//!
#ifndef MONITORINGPLUGINIMPLEMENTER_H
#define MONITORINGPLUGINIMPLEMENTER_H

#include "PluginImplementer.h"


//! MonitoringPluginImplementer class implements the TLMPlugin interface for
//! communication monitoring.
//! Inherits PluginImplementer and re-implements ReceiveTimeData(...).
class MonitoringPluginImplementer : public PluginImplementer
{
public:
    //! Contruction and Initialization
    MonitoringPluginImplementer();

    //! CreateInstance static "factory" method returns
    //! a pointer to the object implementing the interface.
    static MonitoringPluginImplementer* CreateInstance();

    //! ReceiveTimeData receives time-stamped data from coupled simulations
    //! if the specified interface needs more data for the given time.
    //! Since the order of messages can vary the specified interface
    //! is used only to detect the last message expected when the function
    //! is called. Any number of messages to other interfaces can arrive
    //! before the desired message is received.
    //! Input:
    //!   reqIfc - TLM interface that triggered the request;
    //!   time - time needed
    void ReceiveTimeData(TLMInterface *reqIfc, double time);

    //! Initialize plugin for interface monitoring. Should be called
    //! after the default constructor. It will initialize the object
    //! and connect to TLMManager. Will return true on success, false
    //! otherwize. Note that the method can be called only once.
    //! \param name some identifier
    //! \param timeStart start time for the simulation
    //! \param timeEnd end time for the simulation
    //! \param maxStep maximum step of the solver
    //! \param serverName IP address and port of the computer running TLM manager
    //!        separated by colon (e.g., 198.111.123.2:1111)
    bool Init(std::string name,
               double timeStart,
               double timeEnd,
               double maxStep,
               std::string ServerName);
};

#endif // MONITORINGPLUGINIMPLEMENTER_H
