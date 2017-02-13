#ifndef _THREADSYNCH_H_
#define _THREADSYNCH_H_

// By defining SKIP_PTHREADS macro all the references to pthreads
// are removed from compilation. Empty functions that left should
// be "optimized out" by compiler.

#ifndef USE_THREADS
#ifndef SKIP_PTHREADS
#define SKIP_PTHREADS
#endif
#endif

#ifndef SKIP_PTHREADS
#include <pthread.h>
#else
#include <stdlib.h>
#endif

#ifdef DEBUG_TQ_VER_FLG
#include <assert.h>
#define DBG_VALIDATE(F_) assert(F_ == 0)
#else
#define DBG_VALIDATE(F_) F_
#endif


//! Class SimpleLock provides interface to pthread_mutex
class SimpleLock {

#ifndef SKIP_PTHREADS
    //! the mutex variable used
    pthread_mutex_t the_lock;
#endif

#ifdef DEBUG_TQ_VER_FLG
    pthread_t ownerThreadID;
#endif

    friend class SimpleCond;
public:

    //! Constructor. Initializes the mutex
    SimpleLock()
#ifdef DEBUG_TQ_VER_FLG
        :ownerThreadID(777)
    #endif

    {
#ifndef SKIP_PTHREADS
        DBG_VALIDATE(pthread_mutex_init(&the_lock, NULL));
#endif
    };

    //! Unlock and destroy.
    ~SimpleLock()
    {
#ifdef DEBUG_TQ_VER_FLG
        assert(ownerThreadID == 777);
#endif
#ifndef SKIP_PTHREADS
        DBG_VALIDATE(pthread_mutex_destroy (&the_lock));
#endif
    };

    //! Lock the mutex.
    inline void lock();
    
    //! Unlock the mutex.
    inline void unlock();
};

//! Class SimpleCond provides interface to pthread_cond
class SimpleCond {

#ifndef SKIP_PTHREADS
    pthread_cond_t the_cond; // the mutex variable used
#endif

public:

    //! Constructor. Initializes the mutex.
    SimpleCond()
    {
#ifndef SKIP_PTHREADS
        DBG_VALIDATE(pthread_cond_init(&the_cond, NULL));
#endif
    };

    //! Unlock and destroy.
    ~SimpleCond()
    {
#ifndef SKIP_PTHREADS
        DBG_VALIDATE(pthread_cond_destroy (&the_cond));
#endif
    };

    //! Wait on the condition using the specified mutex.
    inline void wait(SimpleLock& lock);

    //! Signal on the condition.
    void signal()
    {
#ifndef SKIP_PTHREADS
        DBG_VALIDATE(pthread_cond_signal(&the_cond));
#endif
    }

    //! Broadcast on the condition.
    void broadcast()
    {
#ifndef SKIP_PTHREADS
        DBG_VALIDATE(pthread_cond_broadcast(&the_cond));
#endif
    }
};


//! Class AutoLock should primarily be used for locking
//! of blocks in a program with SimpleLock: {AutoLock dolock(a_lock); ...protected code }
class AutoLock {
    //! reference of the lock used
    SimpleLock& the_lock;

public:

    //! Constructor. Locks the mutex.
    AutoLock(SimpleLock& a_lock):the_lock(a_lock)
    {
        the_lock.lock();
    };

    //! Destructor. Unlocks the mutex
    ~AutoLock()
    {
        the_lock.unlock();
    };
private:
    // Should never be used
    AutoLock& operator=(AutoLock& a_in) ;
};

//! Class ThreadLocal provides a simple interface to thread specific data.
class ThreadLocal {

#ifndef SKIP_PTHREADS
    pthread_key_t keyID;
#else
    void * data;
#endif

public:
    
    //! Allocate a slot for thread specific data.
    ThreadLocal() {
#ifndef SKIP_PTHREADS
        DBG_VALIDATE(pthread_key_create(& keyID, NULL));
#else
        data = NULL;
#endif
    }

    //! Release the slot.
    ~ThreadLocal() {
#ifndef SKIP_PTHREADS
        DBG_VALIDATE(pthread_key_delete(keyID));
#endif
    }

    //! Get the thread specific pointer.
    void * get_thread_specific() {
#ifndef SKIP_PTHREADS
        return pthread_getspecific(keyID);
#else
        return data;
#endif
    }

    void set_thread_specific(const void * value) {
#ifndef SKIP_PTHREADS
        pthread_setspecific(keyID, value);
#else
        data = (void*)value;
#endif
    }
    
};

//! Lock the mutex.
inline void SimpleLock::lock()
{
#ifndef SKIP_PTHREADS
    DBG_VALIDATE(pthread_mutex_lock(&the_lock));
#endif
#ifdef DEBUG_TQ_VER_FLG
    assert(ownerThreadID == 777);
    ownerThreadID = pthread_self();
#endif
}

//! Unlock the mutex.
inline void SimpleLock::unlock()
{
#ifdef DEBUG_TQ_VER_FLG
    assert(pthread_equal(ownerThreadID,pthread_self()));
    ownerThreadID = 777;
#endif
#ifndef SKIP_PTHREADS
    DBG_VALIDATE(pthread_mutex_unlock(&the_lock));
#endif
}

// Wait on the condition using the specified mutex.
inline void SimpleCond::wait(SimpleLock& lock)
{
#ifdef DEBUG_TQ_VER_FLG
    pthread_t savedID = lock.ownerThreadID;
    lock.ownerThreadID = 777;
#endif
#ifndef SKIP_PTHREADS
    DBG_VALIDATE(pthread_cond_wait(&the_cond, &(lock.the_lock)));
#else
    lock; // does nothing, just to avoid warning.
#endif
#ifdef DEBUG_TQ_VER_FLG
    lock.ownerThreadID = savedID;
#endif
}



#endif
