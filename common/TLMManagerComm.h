//!
//! \file TLMManagerComm.h
//! 
//! Classes used for communication with client apps by TLMManager
//!
#ifndef WIN32
#include <sys/select.h>
#else
#include <winsock2.h>
#endif
#include <vector>

//!
//! TLMManagerComm is responsible for communications on the tlmmanager side
//!
class TLMManagerComm {
    
    //! The FD set structure used in select
    fd_set CurFDSet;

    //! The server socket created with CreateServerSocket
    int ContactSocket;

    //! Client sockets open with AcceptComponentConnection
    std::vector<int> ClientSockets;

    //! Currently active sockets used with Select
    std::vector<int> ActiveSockets;

    //! Startup mode checks both ContactSocket and active sockets
    //! Accept is allowed
    bool StartupMode;

    //! port used for CreateServerSocket
    unsigned short ServerPort;
    
    //! Number of clients processed
    const int NumClients;

public:

    //! Constructor for the specified number of components.
    //! Listen on the specified port.
    TLMManagerComm(int numClients, unsigned short portNr)
        : ContactSocket(-1),
          ClientSockets(),
          ActiveSockets(),
          StartupMode(true),
          ServerPort (portNr),
          NumClients(numClients)
    {
        FD_ZERO(& CurFDSet);
    }

    //! Create socket that will accept the client connections on port ServerPort
    int CreateServerSocket();

    //! Run select on the active set of sockets
    void SelectReadSocket( );

    //! Check if the data is pending to be read on the specified socket
    //! Should be called after SelectReadSocket
    bool HasData(int socket);

    //! Clear the active sockets set. Note that HasData function still
    //! checks the results of the last select.
    void ClearActiveSockets( ) {
        ActiveSockets.resize(0);
    }

    //! Add a socket handle to the active sockets set
    void AddActiveSocket(int socket) {
        ActiveSockets.push_back(socket);
    }

    //! Remove a socket handle from the active sockets set
    void DropActiveSocket(int socket);

    //! Switch from startup mode, when components are sending registration
    //! requests and manager is accepting connections, to running mode, when
    //! manager forwards messages between components.
    void SwitchToRunningMode();

    //! Accept a client component connection
    int AcceptComponentConnections() ;

    //! Close all active sockets.
    void CloseAll();

    //! Return the actual server port.
    //! This port might be different from the server
    //! port specified in the constructor.
    unsigned short GetServerPort()const { return ServerPort; }

};
