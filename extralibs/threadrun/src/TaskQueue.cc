#include <assert.h>

#ifdef NO_POSIX_THREADS
// Vadim added for .NET port
#ifndef SKIP_PTHREADS
#define SKIP_PTHREADS
#endif
#endif


#ifndef USE_THREADS
#ifndef SKIP_PTHREADS
#define SKIP_PTHREADS
#endif
#endif

#include "TaskQueue.h"
#include "TaskQueueNode.h"

#include <ErrorLog.h>

//#ifdef DEBUGFLG
#include <stdio.h>
//#endif

#ifndef NO_POSIX_THREADS
#include <pthread.h>
#endif

//#define DEBUG_TQ_FLG

// Inititalize the thread pool with the specified number of threads
ThreadPool::ThreadPool():
    nthreads_(1),
    runPoolFlg_(0),
    n_runnableQueues_(0),
    rootQueueLocks(0),
    threadRootQueues(0),
    threadCurQueues(0)
{
#ifndef SKIP_PTHREADS

    assert(pthread_key_create(&ThreadPool::keyThreadID, NULL) == 0);
    
    pthread_setspecific(keyThreadID, (const void *)0); // master thread ID = 0
#endif
}


void ThreadPool::start_threads(const int nthreads)
{
    assert(nthreads_ == 1); // restart is not allowed!
    assert(nthreads_ <= ThreadPool::MAX_N_THREADS);  // not allowed to create more threads.

    if(nthreads <= 1) return;  // nothing to be done

#ifndef SKIP_PTHREADS
    runPoolFlg_ = 1;


    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr,  PTHREAD_SCOPE_SYSTEM);

    AutoLock refLock(lock_init);
    rootQueueLocks = new SimpleLock[nthreads];
    threadRootQueues = new VP_TaskQueueNode[nthreads];
    threadCurQueues = new P_TaskQueueNode[nthreads];

    int i;
    for( i = 0; i < nthreads; i++) {
	threadRootQueues[i] = 0;
	threadCurQueues[i] = 0;
    }
    for(i = 1; i < nthreads; i++) {
	pthread_t tmp;
	pthread_create( &tmp, &attr, thread_entry, (void*)(ThreadPool*)this);
    }

    while(nthreads != nthreads_)
        init_cond.wait(lock_init); // wait for all the threads to start


#ifdef DEBUG_TQ_FLG
    printf("Has created %d threads\n", nthreads_);
#endif
#else
    printf("Threads library was not compiled in this binary\n");
    assert(0);
#endif
}; 

void ThreadPool::stop_threads( ) {
    if(runPoolFlg_ == 0) return; // nothing is running
    AutoLock refLock(lock_init);
    runPoolFlg_ = 0;            // clear run-flag

    runnable_queue_lock.lock();
    n_runnableQueues_++;
    runnable_queue_lock.unlock();
    runnable_queue_cond.broadcast();

    while(nthreads_ > 1) {      // while there are running threads
        init_cond.wait(lock_init); // wait for a signal on done_cond
    }
    delete [] rootQueueLocks;
    delete [] threadRootQueues;
    delete [] threadCurQueues;
}

// Destructor. insures gracious exit of threads 
ThreadPool::~ThreadPool( ) {
    stop_threads( );
};

void ThreadPool::start(int nthreads) { 
    assert(thePool_);

    if(nthreads > ThreadPool::MAX_N_THREADS) {
	printf("At most %d threads are supported (%d requested). Will run the maximum supported number of threads\n", ThreadPool::MAX_N_THREADS, nthreads );
	nthreads =  ThreadPool::MAX_N_THREADS;
    }

    thePool_->start_threads(nthreads);  // initialize the singleton object
};

void ThreadPool::stop( ) { 
    assert(thePool_);
    thePool_->stop_threads();
    delete thePool_;
    thePool_ = 0;

};

ThreadPool* ThreadPool::thePool_ = new ThreadPool();  // singleton pointer

#ifndef SKIP_PTHREADS
pthread_key_t ThreadPool::keyThreadID = 0; // the key handle
#endif

void ThreadPool::thread_main_loop() {

    int threadID;
    {
        AutoLock refLock(lock_init);
        threadID = nthreads_++;
    }

#ifndef SKIP_PTHREADS
    ConvertVoidToInt convert;
    convert.ptr = 0;
    convert.value = threadID;

    pthread_setspecific(keyThreadID, convert.ptr);
#endif

    init_cond.signal();  // wake-up constructor
#ifdef DEBUG_TQ_FLG
    printf("Thread %d is entering the main loop\n", threadID);
#endif

    int check_for_work = 1;
    while( runPoolFlg_ ) {   // while the pool is enabled
	if(n_runnableQueues_ <= 0) {
	    runnable_queue_lock.lock();
	    if(n_runnableQueues_ <= 0) {
#ifdef DEBUG_TQ_FLG
		printf("Thread %d will wait on runnable_queue_cond\n", threadID);
#endif
		runnable_queue_cond.wait(runnable_queue_lock);
#ifdef DEBUG_TQ_FLG
		printf("Thread %d gets out\n", threadID);
#endif
	    }
	    runnable_queue_lock.unlock();
	}
  
        if(threadRootQueues[threadID]) { // some work is available in this processor queue
            rootQueueLocks[threadID].lock(); // lock
            if(threadRootQueues[threadID]) { // recheck just to be sure
                threadCurQueues[threadID] = const_cast<TaskQueueNode*>(threadRootQueues[threadID]); // the queue become running
                rootQueueLocks[threadID].unlock(); // unlock root - allow helpers

                const_cast<TaskQueueNode*>(threadRootQueues[threadID])->owner_run(); // run the queue

                rootQueueLocks[threadID].lock(); // lock root to disallow stealing
#ifdef DEBUGFLG
                assert(threadRootQueues[threadID] == threadCurQueues[threadID]);
#endif
                threadCurQueues[threadID] = 0; // clean queue pointers
                threadRootQueues[threadID] = 0; 
            }
            rootQueueLocks[threadID].unlock();  // unlock root
        }
        else { // check other processors
	    int i = 0;
	    for(; i < nthreads_ - 1; i++) {

		int checkID = i ;   // Something more advanced for many proc is needed. OBS!?
	      
		if(threadRootQueues[checkID] ) 
		{ // the processor has some work
		    rootQueueLocks[checkID].lock(); // try stealing
		    TaskQueueNode* stolenNode = const_cast<TaskQueueNode*>(threadRootQueues[checkID]);
		    if(stolenNode && stolenNode->has_parallel_jobs()) { // the queue is still there			
			stolenNode->lock(); // lock it
			rootQueueLocks[checkID].unlock(); // release victim's root

			threadCurQueues[threadID] = stolenNode;

			stolenNode->helper_run_subtree(threadID); // run helper task
			//			stolenNode->unlock();
                    
			threadCurQueues[threadID] = 0;
		    
			break; // out of the loop is we've done something useful
		    }
		    else 
			rootQueueLocks[checkID].unlock(); // release the lock on no success		
		}
            }

        }
    }
            

    lock_init.lock();
    nthreads_--;
    if(nthreads_ <= 1) init_cond.signal(); // wake up the thread that runs destructor.
    //#ifdef DEBUG_TQ_FLG
    printf("Thread %d has exited\n", threadID);
    //#endif
    lock_init.unlock();

};

void ieee_round_precision(int v);

void* thread_entry(void* pool){
    register ThreadPool* pool_typed = (ThreadPool*)pool;
    ieee_round_precision(1);
    pool_typed->thread_main_loop();
    return 0;
}

void ThreadPool::run(TaskQueue* queue) { // use the thread pool to execute queue using maximum of max_threads
    if(nthreads_ == 1) {
        queue->run_task(0,1);
        return;
    }
    int id = get_threadID_unsafe();
    TaskQueueNode* parent = threadCurQueues[id]; // the currently running thread becomes the parent
    // note that parent cannot disappear since this thread is running a child for it -> no locking
    if(!parent)
        rootQueueLocks[id].lock(); // otherwise this is the root queue node -> manage

    TaskQueueNode newNode(id, queue, parent); // create the new node object
    threadCurQueues[id] = &newNode;       // set is as currently running

    if(!parent) {  // there was no parent - manage with the root
#ifdef DEBUGFLG
	assert(threadRootQueues[id] == 0);
#endif
	threadRootQueues[id] = &newNode;  // otherwise set the new root queue node
	rootQueueLocks[id].unlock();      // allow stealing
    }
    runnable_queue_lock.lock();
    n_runnableQueues_++;
    runnable_queue_lock.unlock();

    runnable_queue_cond.broadcast(); //if there are other processors waiting - signal that there is a queue

    newNode.owner_run();                  // run the queue as owner

    if(!parent) {                         // if this was a root node
#ifdef DEBUGFLG
	assert(threadRootQueues[id] == &newNode);
#endif
        rootQueueLocks[id].lock();
        threadRootQueues[id] = 0; // clean queues 
        threadCurQueues[id] = 0;
        rootQueueLocks[id].unlock();
    }
    else 
        threadCurQueues[id] = parent; // return control to the parent queue

    runnable_queue_lock.lock();
    n_runnableQueues_--;
    runnable_queue_lock.unlock();
}
