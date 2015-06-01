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
    static void PackTimeDataMessage(int InterfaceID, std::vector<TLMTimeData>& Data, 
				    TLMMessage& out_mess);
    

    //! Unpack TLMTimeData from TLMMessage into Data queue
    static void UnpackTimeDataMessage(TLMMessage& mess, std::deque<TLMTimeData>& Data);

    //! ConnectManager function tries to establish a TCP/IP connection
    //! to the TLM manager.Returns socket handle on success.
    //! Input: hostname (callname) & port number
    int ConnectManager(std::string& callname, int portnr);

    //! CreateComponentRegMessage packs component name into a message
    //! to be sent to the TLM manager
    void CreateComponentRegMessage(std::string& Name, TLMMessage& mess);

    //! CreateInterfaceRegMessage packs interface name into a message
    //! to be sent to the TLM manager
    void CreateInterfaceRegMessage(std::string& Name, TLMMessage& mess);

    //! UnpackRegInterfaceMessage unpacks the parameters for the connection
    //! attached to the specified interface
    void UnpackRegInterfaceMessage(TLMMessage& mess, TLMConnectionParams& param);

    //! GetSocketHandle returns the SocketHandle obtained after a call to ConnectManager
    int GetSocketHandle() const { return SocketHandle; }
};

#endif
