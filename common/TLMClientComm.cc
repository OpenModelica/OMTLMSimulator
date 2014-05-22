/**
* File: TLMClientComm.cc
* 
* Implementation for the methods defined in TLMClientComm.h
*/
#include "TLMCommUtil.h"
#include "TLMClientComm.h"
#include "TLMErrorLog.h"
#include <vector>
#include <deque>
#include <string>
#include <cstring>

#include <iostream>
#include <fstream>
using std::ofstream;
using std::endl;

using std::vector;
using std::deque;
using std::string;

#ifndef WIN32
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#include <windows.h>
#include <cassert>
#include <io.h>
#endif

#ifndef _MSC_VER
#include <unistd.h> 
#endif

// Constructor
TLMClientComm::TLMClientComm():
SocketHandle(-1) 
{
}


// Fill in TLMMessage with the information from TLMTimeData vector 
// coming to given InterfaceID. This function is called by TLMPlugin
//  when constructing messages with time-stamped data.
void TLMClientComm::PackTimeDataMessage(int InterfaceID, vector<TLMTimeData>& Data, 
                                        TLMMessage& out_mess) {
                                            out_mess.Header.MessageType =  TLMMessageTypeConst::TLM_TIME_DATA;
                                            out_mess.Header.TLMInterfaceID = InterfaceID;
                                            out_mess.Header.SourceIsBigEndianSystem = TLMMessageHeader::IsBigEndianSystem;
                                            out_mess.Header.DataSize = Data.size() * sizeof(TLMTimeData);
                                            out_mess.Data.clear();
                                            out_mess.Data.resize( out_mess.Header.DataSize );
                                            memcpy(& out_mess.Data[0], & Data[0], out_mess.Header.DataSize);
}

// Unpack TLMTimeData from TLMMessage into Data queue
void TLMClientComm::UnpackTimeDataMessage(TLMMessage& mess, deque<TLMTimeData>& Data) {

    // since mess.Data is continious we can just convert the pointer
    TLMTimeData* Next = (TLMTimeData*)(&mess.Data[0]);

    // check if we have byte order missmatch in the message and perform
    // swapping if necessary
    bool switch_byte_order = 
        (TLMMessageHeader::IsBigEndianSystem != mess.Header.SourceIsBigEndianSystem);
    if (switch_byte_order) 
        TLMCommUtil::ByteSwap(Next, sizeof(double),  mess.Header.DataSize/sizeof(double));

    for(unsigned i = 0; i < mess.Header.DataSize/sizeof(TLMTimeData); i++, Next++) {
        TLMErrorLog::Log(" RECV for time= " + TLMErrorLog::ToStdStr(Next->time));
        Data.push_back(*Next);
    }

}

// ConnectManager function tries to establish a TCP/IP connection
// to the TLM manager.Returns socket handle on success.
// Input: hostname (callname) & port number
int TLMClientComm::ConnectManager(string& callname, int portnr) {
    struct sockaddr_in sa;
    int s,count;

#ifdef WIN32
    DWORD e;
    WSADATA ws; 
    e=WSAStartup(0x0101,&ws);
    assert(e==0);
    TLMErrorLog::Log("WinSocket started\n");

    sa.sin_family=AF_INET;

    struct hostent *hp;
    char* localIP;

    hp = gethostbyname(callname.c_str());
    if (hp == NULL) {
        TLMErrorLog::FatalError(string("TLM: Cannot resolve the host") + callname);
        return(-1);
    }
    localIP = inet_ntoa (*(struct in_addr *)*hp->h_addr_list);

    TLMErrorLog::Log(string("Result of inet_ntoa:") + localIP);

    //    sa.sin_addr =*((struct in_addr *)(hp->h_addr));
    //     if (0 == strcmp(callname.c_str(), "localhost")){
    sa.sin_addr.s_addr = inet_addr(localIP);

    sa.sin_port=htons((u_short)portnr);

    s = socket(hp->h_addrtype, SOCK_STREAM, IPPROTO_TCP);

#else
    struct hostent *hp;

    TLMErrorLog::Log("Trying to find TLM manager host " + callname);

    hp = gethostbyname(callname.c_str());
    if (hp == NULL) {
        TLMErrorLog::FatalError(string("TLM: Cannot resolve the host") + callname);
        return(-1);
    }

    memset(&sa, 0 , sizeof(sa));
    memcpy((char *)&sa.sin_addr, hp->h_addr, hp->h_length);

    sa.sin_family = hp->h_addrtype;
    sa.sin_port = htons((u_short)portnr);
    s = socket(hp->h_addrtype,SOCK_STREAM,0);
#endif

    if (s < 0 ){
        TLMErrorLog::FatalError("TLM: Can not contact TLM manager");
    }
    else {
        TLMErrorLog::Log("TLM manager host found, trying to connect...");
    }

    count = 0;

    while (connect(s, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
        count++;
        TLMErrorLog::Log(string("Connection attempt ") +  TLMErrorLog::ToStdStr(count) + " failed");
        if (count>=10){
#ifndef WIN32
            close(s);
#else
            closesocket(s); // BZ306, do not use close() on Windows.
            WSACleanup();
#endif
            TLMErrorLog::FatalError("TLM: Can not connect to manager");
        }

        TLMErrorLog::Log("Pausing...");
#ifndef _MSC_VER
        usleep(count * count * 1000000); // micro seconds
#else
        Sleep(count * count * 1000); // milli seconds
#endif        
        TLMErrorLog::Log("Trying again...");
    }


    SocketHandle = s;

    return(s);
}

void TLMClientComm::CreateComponentRegMessage(std::string& Name, TLMMessage& mess) {
    mess.Header.MessageType = TLMMessageTypeConst::TLM_REG_COMPONENT;
    mess.Header.DataSize = Name.length();
    mess.Data.resize(Name.length());
    memcpy(&mess.Data[0], Name.c_str(), Name.length());
}

void TLMClientComm::CreateInterfaceRegMessage(std::string& Name, TLMMessage& mess) {
    mess.Header.MessageType = TLMMessageTypeConst::TLM_REG_INTERFACE;
    mess.Header.DataSize = Name.length();
    mess.Data.resize(Name.length());
    memcpy(&mess.Data[0], Name.c_str(), Name.length());
}

void TLMClientComm::UnpackRegInterfaceMessage(TLMMessage& mess, TLMConnectionParams& param) {
    if(mess.Header.DataSize == 0) return; // non connected interface
    if(mess.Header.DataSize != sizeof(TLMConnectionParams)) {
        TLMErrorLog::FatalError("Wrong size of message in interface registration : DataSize "+
            Int2Str(mess.Header.DataSize)+
            " sizeof(TLMConnectionParams)="+
            Int2Str(sizeof(TLMConnectionParams)));
    }

    // check if we have byte order missmatch in the message and perform
    // swapping if necessary
    bool switch_byte_order = 
        (TLMMessageHeader::IsBigEndianSystem != mess.Header.SourceIsBigEndianSystem);
    if (switch_byte_order) 
        TLMCommUtil::ByteSwap(& mess.Data[0], sizeof(double),  
        mess.Header.DataSize/sizeof(double)); 

    memcpy(&param, & mess.Data[0], mess.Header.DataSize);
}
