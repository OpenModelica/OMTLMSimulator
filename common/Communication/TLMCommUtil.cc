#include "Communication/TLMCommUtil.h"
#include "Logging/TLMErrorLog.h"
#include "tostr.h"

#include <string>

// BZ306: due to this difficulr bug detailed loggning of each send/recv was added.
// However for performance reasons, i.e. tp
// avoid string maniplulations, they are turned off when this constant is false !
// If TLM operation is correct, no messages should be produced by functions in this file.
bool doDetailedLogging=false;


using std::string;

const char TLMMessageHeader::TLMSignature[TLM_SIGNATURE_LENGTH+1] = "TLM_0101";
const char TLMMessageHeader::IsBigEndianSystem =  TLMCommUtil::IsBigEndian();
TLMMessageHeader::TLMMessageHeader():
    // Constructor
    //Signature({0}),
    MessageType(0),
    SourceIsBigEndianSystem(IsBigEndianSystem),
    DataSize(0),
    TLMInterfaceID(-1) {
    strncpy(Signature, TLMSignature, TLM_SIGNATURE_LENGTH);
}

// Send the TLMMessage pointed by mess via socket with handle SocketHandle
void TLMCommUtil::SendMessage(TLMMessage& mess) {

    int DataSize = mess.Header.DataSize;

    if(doDetailedLogging) {
        TLMErrorLog::Info("SendMessage: wants to send "+
                         Int2Str(sizeof(TLMMessageHeader))+"+"+
                         Int2Str(DataSize)+ " bytes ");
    }

    if(TLMMessageHeader::IsBigEndianSystem != mess.Header.SourceIsBigEndianSystem) {
        // switch byte order for DataSize and InterfaceID
        TLMCommUtil::ByteSwap(&mess.Header.DataSize, sizeof(mess.Header.DataSize));
        TLMCommUtil::ByteSwap(&mess.Header.TLMInterfaceID, sizeof(mess.Header.TLMInterfaceID));
    }

#ifdef  WIN32
    const int MSG_MORE = 0;
#endif

    // NOTE, "MSG_MORE" flag is important for Linux socket performance!
    int sendBytes = send(mess.SocketHandle, (const char*)&(mess.Header) , sizeof(TLMMessageHeader), MSG_MORE);

    if(sendBytes < 0) {
        // try to resend
        TLMErrorLog::Warning("Failed to send message header, will try to continue anyway");
        sendBytes = send(mess.SocketHandle, (const char*)&(mess.Header) , sizeof(TLMMessageHeader), MSG_MORE);
    }

#ifdef  WIN32
    int errcode = WSAGetLastError();
    if(errcode) TLMErrorLog::Warning("send() SOCKET_ERROR received, error code ="+Int2Str(errcode));
#endif

    if(doDetailedLogging) {
        TLMErrorLog::Info("SendMessage:send() sent "+Int2Str(sendBytes)+ " bytes ");
    }

    if(DataSize > 0) {
        sendBytes = send(mess.SocketHandle, (const char*)&(mess.Data[0]), DataSize, 0);
        if(sendBytes < 0) {
            // try to resend
            TLMErrorLog::Warning("Failed to send data, will try to continue anyway");
            sendBytes=send(mess.SocketHandle, (const char*)&(mess.Data[0]), DataSize, 0);
        };

#ifdef  WIN32
        int errcode = WSAGetLastError();
        if(errcode) TLMErrorLog::Warning("send() SOCKET_ERROR received, error code ="+Int2Str(errcode));
#endif

        if(doDetailedLogging) {
            TLMErrorLog::Info("SendMessage:send()(part 2) sent "+Int2Str(sendBytes)+ " bytes ");
        }

    }


}

#ifndef MSG_WAITALL
#define MSG_WAITALL 0
#endif
// Danger : winsock2.h in
// c:\program files\microsoft platform sdk for windows server 2003 r2\include\winsock2.h(658) 
// previously defines it as 
// #define MSG_WAITALL     0x8             /* do not complete until packet is completely filled */
// And this is *not implemented* reported. 

// Basic receive of a TLMMessage. Insures correct signature and
// fixes byte order for the message header if necessary.
// Note that the actual message data is not processed, just received, 
bool TLMCommUtil::ReceiveMessage(TLMMessage& mess) {
    int bcount = recv(mess.SocketHandle, (char*)(&mess.Header), sizeof(TLMMessageHeader) , MSG_WAITALL);
    while((bcount >= 0) && (bcount <  static_cast<int>(sizeof(TLMMessageHeader)))) {
        // this should never happen, but it does...
        TLMErrorLog::Warning("Could not receive the header, will try again");
        bcount += recv(mess.SocketHandle,
                       (char*)(&mess.Header) + bcount,
                       sizeof(TLMMessageHeader) - bcount,
                       MSG_WAITALL);
        if(bcount == 0)
        {
            TLMErrorLog::Warning("Received 0 bits. Socket is probably closed.");
            return false; // seems like on windows this may indicate "socket closed"
        }
    }
    if(bcount == -1) {
#ifdef  WIN32
        int errcode=WSAGetLastError();
        if(errcode==WSAECONNRESET)
            // This is called by normal termination of BEAST
            TLMErrorLog::Info("SOCKET_ERROR received, error code ="+Int2Str(errcode));
        else
            TLMErrorLog::Warning("SOCKET_ERROR received, error code ="+Int2Str(errcode));
#endif
        return false;
    }
    if(doDetailedLogging) {
        TLMErrorLog::Info("ReceiveMessage:recv() returned "+Int2Str(bcount)+ " bytes ");
    }

    if(strncmp(mess.Header.Signature, TLMMessageHeader::TLMSignature, TLMMessageHeader::TLM_SIGNATURE_LENGTH) != 0) {
        char sig1[TLMMessageHeader::TLM_SIGNATURE_LENGTH+1] = {0};
        char sig2[TLMMessageHeader::TLM_SIGNATURE_LENGTH+1] = {0};

        strncpy(sig1, mess.Header.Signature, TLMMessageHeader::TLM_SIGNATURE_LENGTH);
        strncpy(sig2, TLMMessageHeader::TLMSignature, TLMMessageHeader::TLM_SIGNATURE_LENGTH);

        // Just to make sure we have 0 terminated strings.
        sig1[TLMMessageHeader::TLM_SIGNATURE_LENGTH] = 0x00;
        sig2[TLMMessageHeader::TLM_SIGNATURE_LENGTH] = 0x00;

        TLMErrorLog::FatalError("Wrong signature in TLM message, incompatiple TLM format!\n" + std::string(sig1) + " != " + std::string(sig2));
    }

    if(TLMMessageHeader::IsBigEndianSystem != mess.Header.SourceIsBigEndianSystem) {
        // switch byte order for DataSize and InterfaceID
        TLMCommUtil::ByteSwap(&mess.Header.DataSize, sizeof(mess.Header.DataSize));
        TLMCommUtil::ByteSwap(&mess.Header.TLMInterfaceID, sizeof(mess.Header.TLMInterfaceID));
    }
    if(mess.Header.DataSize < 0) {
        TLMErrorLog::FatalError("Negative size of data in TLM message. Protocol error.");
    }
    if(mess.Header.DataSize > 0) {

        mess.Data.clear(); // just to be on the safe side.
        mess.Data.resize(mess.Header.DataSize);
        bcount = recv(mess.SocketHandle,(char*)&(mess.Data[0]), mess.Header.DataSize,  MSG_WAITALL);
        while((bcount >= 0) && (bcount <  mess.Header.DataSize)) {
            // this should never happen, but it does...
            TLMErrorLog::Warning("Could not receive the TLM data, will try again");
            bcount += recv(mess.SocketHandle,
                           (char*)&(mess.Data[0]) + bcount,
                    mess.Header.DataSize - bcount,
                    MSG_WAITALL);
        }
        if(bcount == -1) {

#ifdef  WIN32
            int errcode=WSAGetLastError();
            TLMErrorLog::Warning("SOCKET_ERROR received(part 2), error code ="+Int2Str(errcode));
#endif

            return false;
        }
        if(doDetailedLogging) {
            TLMErrorLog::Info("ReceiveMessage:recv()(part 2) returned "+Int2Str(bcount)+ " bytes ");
        }
        if(bcount != mess.Header.DataSize) {
            TLMErrorLog::FatalError("Error receiving message data");
        }
    }

    return true;
}

