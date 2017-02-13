/**
 * File: TLMMessageQueue.cc
 *
 * Implementation of the MessageQueue methods
 */
#include "TLMMessageQueue.h"
#include "TLMCommUtil.h"
#include <cassert>

TLMMessageQueue::~TLMMessageQueue() {
    Terminate();

    SenderWait.signal(); // to be sure that no one "hangs" on it

    SendBufLock.lock();
    while(!SendBuffers.empty()) {
        SenderWait.wait( SendBufLock);
    }
    SendBufLock.unlock();

    FreeBufLock.lock();
    while(FreeBuffers.size() > 0) {
        delete FreeBuffers.top();
        FreeBuffers.pop();
    }
    FreeBufLock.unlock();
}


TLMMessage* TLMMessageQueue::GetReadSlot() {
    TLMMessage* ret = NULL;
    FreeBufLock.lock();
    if(FreeBuffers.size() > 0) {
        ret = FreeBuffers.top();
        FreeBuffers.pop();
    }
    FreeBufLock.unlock();
    if(ret == NULL )
        ret = new TLMMessage();
    return ret;
}

// Put the message on the message send queue
void TLMMessageQueue::PutWriteSlot(TLMMessage* mess) {
    if(Terminated) return;

    SendBufLock.lock();
    SendBuffers.push(mess);
    if(SendBuffers.size() == 1) {
        SenderWait.signal();
    }
    SendBufLock.unlock();
}

// Get the next message to be sent. May block if there are no
// messages in the queue. Returns "NULL" if no messages to send
// left.
TLMMessage* TLMMessageQueue::GetWriteSlot() {
    TLMMessage* ret = NULL;
    SendBufLock.lock();
    if(SendBuffers.empty() && !Terminated) {
        SenderWait.wait( SendBufLock);
    }
    if(SendBuffers.size() >  0) {
        ret = SendBuffers.front();
        SendBuffers.pop();
    }
    SendBufLock.unlock();

    if( Terminated && (SendBuffers.size() == 0)) {
        SenderWait.signal(); // signal destructor in case it is waiting
    }
    return ret;
}

// Put a message back on the free slots stack.
void TLMMessageQueue::ReleaseSlot(TLMMessage* mess) {
    FreeBufLock.lock();
    FreeBuffers.push(mess);
    FreeBufLock.unlock();
}

void TLMMessageQueue::Terminate() {
    Terminated = true;
    
    SenderWait.signal(); // to be sure that no one "hangs" on it
}
