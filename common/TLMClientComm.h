//!
//! \file TLMClientComm.h
//! 
//! Defines the classes & data structures used for 
//! sending/receiving messages by TLM clients
//!

#ifndef TLMClientComm_h_
#define  TLMClientComm_h_

#include <vector>
#include <deque>
#include <string>
#include <cstdlib>
#include "TLMCommUtil.h"
#include "TLMErrorLog.h"
#include "common.h"

//! Class TLMClientCommUtil contains utility functions used by
//! TLM client applications
class TLMClientComm {

    int SocketHandle;
    
 public:

    //! Constructor
    TLMClientComm();

    //! Destructor, closes socket.
    ~TLMClientComm();

    //! Fill in TLMMessage with the information from TLMTimeData vector 
    //! coming to given InterfaceID. This function is called by TLMPlugin
    //!  when constructing messages with time-stamped data.
    static void PackTimeDataMessageSignal(int InterfaceID,
                                          std::vector<TLMTimeDataSignal> &Data,
                                          TLMMessage &out_mess);
    static void PackTimeDataMessage1D(int InterfaceID,
                                      std::vector<TLMTimeData1D> &Data,
                                      TLMMessage &out_mess);
    static void PackTimeDataMessage3D(int InterfaceID,
                                      std::vector<TLMTimeData3D> &Data,
                                      TLMMessage& out_mess);

    //! Unpack TLMTimeData from TLMMessage into Data queue
    static void UnpackTimeDataMessageSignal(TLMMessage &mess, std::deque<TLMTimeDataSignal> &Data);
    static void UnpackTimeDataMessage1D(TLMMessage &mess, std::deque<TLMTimeData1D> &Data);
    static void UnpackTimeDataMessage3D(TLMMessage& mess, std::deque<TLMTimeData3D>& Data);


    //! ConnectManager function tries to establish a TCP/IP connection
    //! to the TLM manager.Returns socket handle on success.
    //! Input: hostname (callname) & port number
    int ConnectManager(std::string& callname, int portnr);

    //! CreateComponentRegMessage packs component name into a message
    //! to be sent to the TLM manager
    void CreateComponentRegMessage(std::string& Name, TLMMessage& mess);

    //! CreateInterfaceRegMessage packs interface name into a message
    //! to be sent to the TLM manager
    void CreateInterfaceRegMessage(std::string& Name, int dimensions, std::string& causality, std::string domain, TLMMessage& mess);

    //! CreateInterfaceRegMessage packs interface name into a message
    //! to be sent to the TLM manager
    void CreateParameterRegMessage(std::string& Name, std::string& Value, TLMMessage& mess);

    //! UnpackRegInterfaceMessage unpacks the parameters for the connection
    //! attached to the specified interface
    void UnpackRegInterfaceMessage(TLMMessage& mess, TLMConnectionParams& param);

    void UnpackRegParameterMessage(TLMMessage &mess, std::string &Value);

    //! GetSocketHandle returns the SocketHandle obtained after a call to ConnectManager
    int GetSocketHandle() const { return SocketHandle; }
};

#endif
