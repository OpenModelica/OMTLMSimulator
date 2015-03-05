#ifndef LoopScheduler_H_
#define LoopScheduler_H_

#ifndef USE_THREADS
#ifndef SKIP_PTHREADS
#define SKIP_PTHREADS
#endif
#endif

class Timer; // Forward declaration. timer.h from scheluder2 should be used.

// LoopScheduler is an abstact class that defines access to different 
// loop scheduling methods.
class LoopScheduler {

protected:

    //! number of chunks (iteration blockes) used for scheduling
    int chunkNum;

    //! Array of (chunkNum+1) start loop indices for every chunk. chunkStartInd[0] is typically 0
    //! chunkStartInd[chunkNum] = total number of loop iterations. Child classes should fill
    //! in this array in the constructor.
    int* chunkStartInd;

    //! This flag can be set to false to indicate that the loop takes too little time 
    //! and so it should not be run in parallel. Initialized to -1 to indicate lack of feedback info
    int run_parallel;

    //! const used to indicate that the last iteration is taken = -1
    static const int NO_TASK;

    //! const used to indicate end of queue = -2
    static const int END_OF_QUEUE;

public:
    
    //! Constructor.
    //! Input: n_chunks - the number of chunks (blocks of loop iterations)
    LoopScheduler(int n_chunks);
        
        //! Destructor
    virtual ~LoopScheduler();

    //! Reset the internal data. Note: it is not safe to call
    //! reset when calculation is in progress
    virtual void reset_tasks() = 0;

    //! Interface function get_task returns the index of the next
    //! chunk to be processed by a thread or -1 if all chunks are processed.
    //! The function must be thread safe.
    //! Input: index_done - the index of a chunk processed by a thread.
    //! -1 for the first call.
    virtual int get_task(int index_done) = 0;

        //! Get the total number of chunks used by the scheduler as set in the constructor.
    int get_chunkNum() const
        { 
	  if(!run_parallel) return 1; 
	  
	  return chunkNum; 
	};

        //! Returns the start index (inclusive) for a chunk "ind".
    int get_chunkStart(int ind) const
        {
            return  chunkStartInd[ind];
        };

        //! Returns the end index (inclusive) for the chunk "ind"
    int get_chunkEnd(int ind) const
        {
	  if(!run_parallel) return chunkStartInd[chunkNum] - 1;
	  return  chunkStartInd[ind+1] - 1;
        };

    
};


// INFGLoopScheduler class implements the inner feedback guided
// dynamic scheduling algorithm.
// It should be used for inner loops with larger number of iterations
// (> nthreads * 2)

class INFGLoopScheduler:public LoopScheduler {

 public:

    //! Constructor. Initializes the class for num_iter loop
    //! steps. 
    INFGLoopScheduler(int num_iter);

    //! Destructor
    virtual ~INFGLoopScheduler();

    // Reset the internal data. Note: it is not safe to call
    // reset if "run" is in progress.
    virtual void reset_tasks();

protected:
    
    //! Utility function calculates the number of chunks 
    //! to be used for a given number of loop iterations.
    static int calc_chunk_num(int num_iter);


    //! Synchronized function. Returns the index of the chunk to be processed next.
    //! The caller must process the chunk and call get task again giving this
    //! chunk number to indicate that the chunk is ready. The function returns
    //! -1 when all the chunks are processed.
    int get_task(int doneTask);

protected:

    volatile int lastTask_; // The last chunk index returned by get_task

    SimpleLock queueMutex_; // This queue lock used in get_task

    Timer* blockTimers; // array of timers for every chunk. Used for load balancing.
    Timer* procStartTimers; // array of timers for every chunk. Used for load balancing.

    volatile int nproc_; // number of processors actually running the loop

    int n_iter;            // number of iterations in the loop to be run in parallel
};

// FGLoopScheduler class implements the feedback guided
// dynamic scheduling algorithm.
// It should be used for loops with larger number of iterations
// (> nthreads * 2).
class FGLoopScheduler:public LoopScheduler {
public:

    //! Constructor. Initializes the class for num_iter loop
    //! steps. 
    FGLoopScheduler(int num_iter);

    //! Destructor
    virtual ~FGLoopScheduler();

    // Reset the internal data. Note: it is not safe to call
    // reset if "run" is in progress.
    virtual void reset_tasks();

protected:
    
    //! Utility function calculates the number of chunks 
    //! to be used for a given number of loop iterations.
    static int calc_chunk_num(int num_iter);


    //! Synchronized function. Returns the index of the chunk to be processed next.
    //! The caller must process the chunk and call get task again giving this
    //! chunk number to indicate that the chunk is ready. The function returns
    //! -1 when all the chunks are processed.
    int get_task(int doneTask);

protected:

    volatile int lastTask_; // The last chunk index returned by get_task

    SimpleLock queueMutex_; // This queue lock used in get_task

    Timer* blockTimers; // array of timers for every chunk. Used for load balancing.

    int n_iter;            // number of iterations in the loop to be run in parallel
};

// DynamicLoopScheduler class implements the basic dynamic algorithm
// for running loops in parallel when different loop iterations are
// independent
class DynamicLoopScheduler:public LoopScheduler {
public:

    // Constructor. Initializes the scheduler for num_iter loop
    // steps. "chunk_size" specifies the minimum number of iterations
    // to be run without synchronization
    DynamicLoopScheduler(int num_iter, int chunk_size = 1);

    // Destructor
    virtual ~DynamicLoopScheduler();

    //! Reset the internal data. Note: it is not safe to call
    //! reset if "run" is in progress.
    virtual void reset_tasks();

protected:

    //! Utility function calculates the number of chunks 
    //! to be used for a given number of loop iterations.
    //! Synchronized function. Returns the index of the chunk to be processed next.
    //! The function returns -1 when all the chunks are processed.
    static int calc_chunk_num(int num_iter, int chunk_size);

    //! Synchronized function. Returns the index of the chunk to be processed next.
    //! The caller must process the chunk and call get task again giving this
    //! chunk number to indicate that the chunk is ready. The function returns
    //! -1 when all the chunks are processed.
    int get_task(int );

private:

    volatile int lastTask_; // The last chunk index returned by get_task

    SimpleLock queueMutex_; // This queue lock used in get_task

    int n_iter;            // number of iterations in the loop to be run in parallel
    int np_;               // number of threads to be used
};


#endif
