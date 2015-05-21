//!
//! \file TLMCommUtil.h
//! 
//! Defines the common classes & data structures used for 
//! sending/receiving messages between TLM clients and the manager
//!
//! \author   Iakov Nakhimovski
//!
//!
#ifndef TLMCommUtil_h_
#define TLMCommUtil_h_

#include <vector>
#include <cstring>

#include "TLMCalcData.h"

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>

//// #define MSG_WAITALL 0

#else

#include  <sys/socket.h>
#include  <netdb.h>
#include <netinet/in.h>

#include <cstring>
#include <arpa/inet.h>

//!  SOCKET is used on Win32 but not defined on Linux, its a synonym to int.
typedef int SOCKET;

#endif

//! TLMMessageTypeConst lists the constants used in MessageType field
//! in the TLMMessageHeader.
struct TLMMessageTypeConst {
    //! Time stamped data
    static const char TLM_TIME_DATA = 1;
    //! Component registration message
    static const char TLM_REG_COMPONENT = 2;
    //! Interface registration message
    static const char TLM_REG_INTERFACE = 4;
    //! Component is done with registration, ready for simulation run.
    static const char TLM_CHECK_MODEL = 8;
};

//! Message header used in all the messages sent between
//! TLM clients & TLM manager.
struct TLMMessageHeader {

    //! The length of the signature string. 
    static const int TLM_SIGNATURE_LENGTH =  8;

    //! The signature that comes in the beginning of every message
    static const char TLMSignature[TLM_SIGNATURE_LENGTH+1];

    //! This constant tells if the host system is using BigEndian 
    //!  conversion for data storage 
    static const char IsBigEndianSystem;

    //! Constructor
    TLMMessageHeader();
    
    //! The signature, used for checking
    // Note, length was TLM_SIGNATURE_LENGTH, I added one since it's a string and is copied from
    // TLMSignature. I'm not totally sure if this affects big-little endian swapping though!?
    // Alex S. 21/02/2014
    char Signature[TLM_SIGNATURE_LENGTH];

    //! Message king (see TLMMessageTypeConst above)
    char MessageType;                     

    //! The bit conversion used on originator system
    char SourceIsBigEndianSystem;         

    //! Size of Data in bytes
    int  DataSize;

    //! Source interface ID (not used for registration messages)
    int  TLMInterfaceID;                  
};

//! TLMMessage structure is used to encapsulate all the TLM messages
struct TLMMessage {
    //! source/destination socket
    int SocketHandle; 

    //! Message header
    TLMMessageHeader Header;    

    //! Data array (contents depends on the message type)
    std::vector<unsigned char> Data;

    //! Constructor, initializes all attributes.
    TLMMessage()
        : SocketHandle(-1)
        , Header()
        , Data()
    {}
};


//! Class TLMCommUtil defines communication utility functions used both
//! on client and server
class TLMCommUtil {

 public:

    //! Empty contructor.
    TLMCommUtil(){}


    //! The IsBigEndian() function detects if the current hardware 
    //! uses Large or Small endian conversion

    static bool IsBigEndian()	{
	short word = 0x4321;
	if((*(char *)& word) != 0x21 )
	    return true;
	else 
	    return false;
    }

    //! The ByteSwap function swaps byte order of any atomic type
    //! as needed when trasferring binary data between large and small
    //! endian systems.
    //! \param Buff - memory buffer to be processed
    //! \param  type_size - number of bytes in the data type (2,4,8)
    //! \param  items - number of data items to be proccessed 
    inline static void ByteSwap(void * Buff, size_t type_size, size_t items = 1);

    //! Send the TLMMessage pointed by mess via socket with handle SocketHandle
    static void SendMessage( TLMMessage& mess );

    //! Basic receive of a TLMMessage. Insures correct signature and
    //! fixes byte order for the message header if necessary.
    //! Note that the actual message data is not processed, just received, 
    //! Returns 'true' on success, 'false' if socket is closed, aborts on error.
    static bool ReceiveMessage( TLMMessage& mess);

};

inline void TLMCommUtil::ByteSwap(void * Buff, size_t type_size, size_t items){
    register unsigned char * b = (unsigned char *)Buff;
    register size_t items_cnt = items;
    while(items_cnt-- > 0) {
	register size_t i = 0; // was int, warning removed.
	register size_t j = type_size-1;
	while (i<j)   {
	    std::swap(b[i], b[j]);
	    i++, j--;
	}
	b += type_size;
    }
}
#endif
