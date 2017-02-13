//!
//! \file TLMMessageQueue.h
//! 
//! Defines the MessageQueue thread safe class 
//!

#ifndef TLMMessageQueue_h_
#define TLMMessageQueue_h_

#include <queue>
#include <stack>
#include "TLMThreadSynch.h"
#include "TLMCommUtil.h"

//! Class TLMMessageQueue is a thread-safe message queue as needed
//! by the ManagerCommHandler class.
class TLMMessageQueue {

    //! The buffers to be sent.
    SimpleLock SendBufLock;
    std::queue<TLMMessage*> SendBuffers;

    //! Stack of free message buffers - to save allocations.
    //! Storage is messaged by this class.
    SimpleLock FreeBufLock;
    std::stack<TLMMessage*> FreeBuffers;

    //! Nothing to be send. Wait on this.
    SimpleCond SenderWait;

    //! Terminated flag tells if the protocol is over and
    //! no more messages are expected in PutWriteSlot
    bool Terminated;

public:

    //! Constructor
    TLMMessageQueue()
        : SendBufLock()
        , SendBuffers()
        , FreeBufLock()
        , FreeBuffers()
        , SenderWait()
        , Terminated(false)
    {}

    //! Destructor
    ~TLMMessageQueue() ;

    //! Get a free slot that can be filled in.
    TLMMessage* GetReadSlot() ;

    //! Put the message on the message send queue
    void PutWriteSlot(TLMMessage* mess) ;

    //! Get the next message to be sent. May block if there are no
    //! messages in the queue. Returns "NULL" if no messages to send
    //! left.
    TLMMessage* GetWriteSlot() ;

    //! Put a message back on the free slots stack.
    void ReleaseSlot(TLMMessage* mess);

    //! Terminate function marks the end of communication protocol.
    //! It causes GetWriteSlot to return NULL.
    void Terminate();
};

#endif
