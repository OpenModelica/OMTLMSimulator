#ifndef _TASKQUEUE_H_
#define _TASKQUEUE_H_

// #define SKIP_PTHREADS
// By defining SKIP_PTHREADS macro all the references to pthreads
// are removed from compilation. Empty functions that left should
// be "optimized out" by compiler.

#include <ThreadSynch.h>

#ifndef USE_THREADS
#ifndef SKIP_PTHREADS
#define SKIP_PTHREADS
#endif
#endif

#ifndef SKIP_PTHREADS
#include <pthread.h>
#endif

// Forward class declarations
class TaskQueueNode;
typedef TaskQueueNode* P_TaskQueueNode;
typedef volatile TaskQueueNode* VP_TaskQueueNode;

// Interface class for task queues to be run by the ThreadPool
class TaskQueue{

public:
    //! This function is called concurrently by several threads until one
    //! of threads returns.
    //! Input:
    //!  threadID ID of the calling thread (0, threadNum - 1)
    //!  threadNum - number of worker threads
    virtual void run_task(const int threadID, const int threadNum) = 0; 
    
    virtual ~TaskQueue(){}

};


// The thread pool class is responsible for the creation and maintenance of threads
class ThreadPool {

    union ConvertVoidToInt {
	void* ptr;
	int   value;
    };

public:
  
    //! Inititalize the thread pool with the specified number of threads
    ThreadPool( ); 

    //! Destructor. insures gracious exit of threads 
    ~ThreadPool( );

    //! Maximum number of threads supported by the library
    static const int MAX_N_THREADS = 8;

    //! Assumption that setting a pointer is an atomic operation is correct
    static const bool SET_POINTER_IS_ATOMIC = true;

    //! Start threads associated with the singleton instance of ThreadPool
    static void start(int nthreads);

    //! Stop threads and destroy the singleton instance of the ThreadPool
    //! note that there is no way to restart the thread pool for now.
    static void stop( ) ;


    //! get the singleton pointer
    static ThreadPool* get_instance() { return thePool_; } 

    //! Returns the threadID. Returns 0 if singleton was not initialized
    static int get_threadID() 
	{ 
#ifndef SKIP_PTHREADS
	    if (get_instance()) {
		ConvertVoidToInt convert;
		convert.ptr = pthread_getspecific(keyThreadID);
		return (convert.value);
	    }
	    else 
#endif
                return 0;
	}

    //! Returns the threadID [0..nthreads]. The function is unsafe for efficiency reasons!
    //! It will crash if the singleton was not initialized. Mater thread's ID = 0.
    static int get_threadID_unsafe() {
#ifndef SKIP_PTHREADS
	ConvertVoidToInt convert;
	convert.ptr = pthread_getspecific(keyThreadID);
	return (convert.value);
#else
        return 0;
#endif
    }

    //! Execute the tasks as defined by the queue
    void run(TaskQueue* queue);

    //! Get the number of "worker" threads in the application
    static int get_nthreads()  
	{ 
	    ThreadPool* pool = get_instance();
	    if (pool) 
		return pool->get_np();
	    else return 0;
	}

    //! Get the number of "worker" threads in this thread pool
    int get_np() const 
#ifndef SKIP_PTHREADS
      { return nthreads_; } 
#else
      { return 1; } 
#endif


protected:

    //! Start specified number of threads
    void start_threads(const int nthreads);
  
    //! Gracious termination of the working threads
    void stop_threads( );
    
    //! Singleton pointer.
    static ThreadPool* thePool_; 

    //! Number of running threads in this ThreadPool.
    volatile int nthreads_;  

    //! This flag is used to terminate the threads.
    volatile int runPoolFlg_;

    //! Number of queues that have some work.
    volatile int n_runnableQueues_;

    //! Lock used for access to runnable queues.
    SimpleLock runnable_queue_lock;

    //! Condition variable that is used for synchronization on runnable queues.
    SimpleCond runnable_queue_cond;

    // condition variable and mutex used in initialization and destruction
    SimpleLock lock_init;
    SimpleCond init_cond;

#ifndef SKIP_PTHREADS
    static pthread_key_t keyThreadID; 
#endif

    SimpleLock* rootQueueLocks;
    VP_TaskQueueNode* threadRootQueues;
    P_TaskQueueNode* threadCurQueues;
    
    //! Main waiting loop of the thread in the pool
    void thread_main_loop();

    //! This function is used as a thread entry point in the ThreadPool class. 
    //! It is put outside the class for better compiler portability.
    //! GCC 4.1 requires that it is declared outside as well.  (B532)
    friend void* thread_entry(void* pool);

    friend class TaskQueueNode;
};


//! This function is used as a thread entry point in the ThreadPool class. 
//! It is put outside the class for better compiler portability.
//! GCC 4.1 requires that it is declared here  as well.
void* thread_entry(void* pool);

#endif
