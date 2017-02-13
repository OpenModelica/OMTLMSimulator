/**
* File: TLMManagerComm.cc
* 
* Implementation of classes used for communication with client apps by TLMManager
*/
#include "TLMManagerComm.h"
#include "TLMErrorLog.h"
#include <cassert>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

using std::vector;
using std::string;

#ifndef WIN32
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BCloseSocket close
#else
#include <winsock2.h>
#include <windows.h>
#include <cassert>
#include <io.h>
#define BCloseSocket closesocket
#endif

// CreateServerSocket create a server TCP/IP socket
// and start listening. Returns the socket ID.
int TLMManagerComm::CreateServerSocket()
{
    assert(ContactSocket == -1);

    struct sockaddr_in sa;  // My socket addr.

#define MAXHOSTNAME 1024

    char myname[MAXHOSTNAME+1];
    struct hostent *hp;

#ifdef WIN32
    WSADATA ws;
    int d;
    d=WSAStartup(0x0101,&ws);
    assert(d==0);
#endif

    memset(&sa,0, sizeof(struct sockaddr_in));
    gethostname(myname,MAXHOSTNAME);
    hp = gethostbyname((const char*) myname);

    if (hp==NULL){
        TLMErrorLog::FatalError("Create server socket - failed to get my hostname, check that name resolves, e.g. /etc/hosts has "+std::string(myname)) ;
        // See BZ2161.

        // Adding this line to /etc/hosts resolves (for me) the problem with
        //  "Create server socket - failed to get my hostname"

        // 127.0.0.3       homer.mathcore.local


        return -1;
    }
    sa.sin_family = hp->h_addrtype;

#ifdef WIN32
    char* localIP;
    localIP = inet_ntoa (*(struct in_addr *)*hp->h_addr_list);
    sa.sin_addr.s_addr = inet_addr(localIP);
#endif

    if(AF_INET != sa.sin_family) {
        TLMErrorLog::FatalError("Unsupported address family returned by gethostbyname") ;
        return -1;
    }
    sa.sin_port = htons(ServerPort);

    int theSckt;

    if ((theSckt =
     #ifdef WIN32
         socket(AF_INET, SOCK_STREAM,IPPROTO_TCP)
     #else
         socket(AF_INET, SOCK_STREAM,0)

     #endif
         ) < 0 ) {
        TLMErrorLog::FatalError("Create server socket - failed to get a socket handle") ;

        return -1;
    }

    int bindCount = 0;
    int maxIterations = 1000; // BUG: should be calculated from a max. port range!
    // Bind the socket, first try the predefined port, then increase port number.
    while (bind(theSckt,(struct sockaddr *) &sa, sizeof(struct sockaddr_in)) < 0 && bindCount < maxIterations ) {
        ServerPort++;
        bindCount++;
        sa.sin_port = htons(ServerPort);
    }

    if( bindCount == maxIterations ){
        BCloseSocket(theSckt);
        TLMErrorLog::FatalError("Create server socket - failed to bind. Check that the port is free.") ;
        return -1;
    }

    if(listen(theSckt, NumClients) != 0) {
        BCloseSocket(theSckt);
        TLMErrorLog::FatalError("Crate server socket - failed in listen on the server socket.") ;
    };

    ContactSocket = theSckt;

    TLMErrorLog::Log(string("TLM manager is listening on port ") + TLMErrorLog::ToStdStr(ServerPort));

    return theSckt;
}


void TLMManagerComm::SelectReadSocket( ) {

    int maxFD = -1;
    FD_ZERO(& CurFDSet);
    for(vector<int>::iterator it = ActiveSockets.begin(); it != ActiveSockets.end(); it++) {
        FD_SET(*it, &CurFDSet);
        if(*it > maxFD) {
            maxFD = *it;
        }
    }

    assert(maxFD > 0); // assert that at least one socket needs to be checked

    struct timeval tv;

    // sock is an intialized socket handle

    tv.tv_sec = 10;

    tv.tv_usec = 0;

    /* wait 10 seconds for any data to be read from any single socket */

    select(maxFD + 1, &CurFDSet, NULL, NULL, &tv);
}


// Check if the data is pending to be read on the specified socket
// Should be called after SelectReadSocket
bool TLMManagerComm::HasData(int socket) {
    bool ret = FD_ISSET(socket, &CurFDSet);
    //    if( ret ) FD_CLR(socket, &CurFDSet);
    return ret;
}

// Switch from startup mode, when
void TLMManagerComm::SwitchToRunningMode() {
    assert(StartupMode == true);
    StartupMode = false;

    ActiveSockets.clear();
    ActiveSockets = ClientSockets;
}

int TLMManagerComm::AcceptComponentConnections() {
    TLMErrorLog::Log("TLM_manager - accepting connection");
    int theCon;
    if ((theCon = accept(ContactSocket,NULL,NULL)) < 0){
        TLMErrorLog::FatalError("Could not accept a connection");
    }

    ClientSockets.push_back(theCon);

    return theCon;
}

// Add a socket handle to the active sockets set
void TLMManagerComm::DropActiveSocket(int socket) {
    BCloseSocket(socket);
    ActiveSockets.erase(std::find(ActiveSockets.begin(), ActiveSockets.end(), socket));
}

// Close all active sockets
void TLMManagerComm::CloseAll()
{
    std::vector<int>::iterator activeSockIter;
    for( activeSockIter = ActiveSockets.begin() ; activeSockIter != ActiveSockets.end() ; activeSockIter++ ){
        BCloseSocket(*activeSockIter);
    }
    BCloseSocket(ContactSocket);
}
