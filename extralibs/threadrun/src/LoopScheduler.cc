#include <stdio.h>
#include <assert.h>
#include "TaskQueue.h"
#include "LoopScheduler.h"
#include <ErrorLog.h>
#ifdef USE_PARALLEL_INTERFACE_OFF
#include "timer.h"
#else
#include "../../scheduler3/src/timer.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////
///  Methods for LoopScheduler
LoopScheduler::LoopScheduler(int n_chunks):
    chunkNum(n_chunks),
    chunkStartInd(0),
    run_parallel(-1) // since we don't have feedback info from the beginning
{
    chunkStartInd = new int[n_chunks+1];
    
};

LoopScheduler::~LoopScheduler()    // Destructor
{ 
    delete [] chunkStartInd;
#ifdef DEBUGFLG
    printf("Loop scheduler: destructor\n");
#endif
}

const int LoopScheduler::END_OF_QUEUE = -2;
const int LoopScheduler::NO_TASK = -1;


static const int MAX_CHUNK_NUM = ThreadPool::MAX_N_THREADS * 10;

static void balance_blocks(const int chunkNum,         // number of chunks in the loop
			   const timeunit block_times[],  // chunkNum timers for the chunks
			   int chunkStartInd[],        // (chunkNum+1) integers giving the start indices
			   int& run_parallel)         // flag saying if the loop is longs enough to consider it for parallel run
{ //  balance_blocks
    assert(MAX_CHUNK_NUM >= chunkNum);
    timeunit block_times_scaled[MAX_CHUNK_NUM]; // time spent in this block * scale factor
    int old_chunk_iter[MAX_CHUNK_NUM]; // old balancing

    const int n_iter = chunkStartInd[chunkNum] - chunkStartInd[0];

    const int reschedule_threshold_frac =  n_iter; // scaling factor used to avoid floating point calculations = number of loop iterations

    timeunit ideal_load = 0; // time for block at ideal load balance

    int i; // loop index

    // calculate the total time scaled with reschedule_threshold_frac
    for(i = 0; i < chunkNum; i++){ 
	ideal_load += (block_times_scaled[i] = block_times[i] * reschedule_threshold_frac);
	old_chunk_iter[i] = chunkStartInd[i+1] - chunkStartInd[i];
#ifdef DEBUG_TQ_FLG
	printf("Old chunk %d has block_time %d (scaled %d) for %d iterations\n",
	       i, block_times[i].get_msec(), block_times_scaled[i].get_msec(), old_chunk_iter[i] );
	assert(old_chunk_iter[i] >= 0);
#endif
    }

    // ideal load balancing
    ideal_load = ideal_load / chunkNum ;

    if(ideal_load == 0) 
	{
#ifdef DEBUG_TQ_FLG
	    printf("The loop is too fast - no reason to run in parallel\n");
#endif
	    run_parallel = 0;
	    return ;
	}
    else if(!run_parallel){
#ifdef DEBUG_TQ_FLG
	printf("The loop is slow and can be run in parallel\n");
#endif
	run_parallel = 1;
	return;
    }

#ifdef DEBUG_TQ_FLG
    printf("Ideal load calculated %d with %d chunks\n", ideal_load, chunkNum);
#endif
    int chunk_ind = 0; // the chunk number during the previous iteration (old chunks)
      
    // estimated time to be distributed from the chunk[chunk_ind]
    timeunit time_last_chunk =  block_times_scaled[0];       

    // number of iterations to be distributed from the chunk[chunk_ind]
    int n_iter_last_chunk = old_chunk_iter[0];
             
    timeunit slice_time_last_chunk = time_last_chunk/n_iter_last_chunk; // estimated time for one slice   from the chunk[chunk_ind]

    int n_iter_left = n_iter;

    for(i = 0; i < chunkNum-1; i++){ // for all chunks except the last one
	timeunit time_left = ideal_load;    // preferred time 
	int n_iter_block = 0;          // iterations taken in the chunk

#ifdef DEBUG_TQ_FLG
	printf("Calculating iterations for chunk %d\n", i);
#endif
	while(chunk_ind < chunkNum) {  // for all old chunks
	    if(time_last_chunk > time_left + slice_time_last_chunk/2) { // the current old block is larger than we need

		int take_iter = 0;
		if(time_left > 0) {
		    if(slice_time_last_chunk > 0)
			take_iter = (time_left + slice_time_last_chunk/2).ratio(slice_time_last_chunk); // some iterations will go into current new chunk
		    else
			take_iter = n_iter_last_chunk;
		}
		      
		n_iter_block += take_iter;

		if(n_iter_block <= 0) {	      
		    n_iter_block = 1; // at least one
		    take_iter = 1;
		}

#ifdef DEBUG_TQ_FLG
		printf("Chunk %d gets %d iterations\n", i, n_iter_block );
#endif
		n_iter_left -= n_iter_block;

		if(n_iter_left < chunkNum - i) { // we have exactly one iteration for each chunk left
#ifdef DEBUG_TQ_FLG
		    printf("Exactly one iteration per chunk left \n" );
#endif
		    for(int j = chunkNum - 1;j > i; j--){
			chunkStartInd[j] = chunkStartInd[j+1] - 1;
		    }
		    i = chunkNum; // OBS! leave the for loop
		}
		else {
		    chunkStartInd[i+1] = chunkStartInd[i] + n_iter_block; // calc start index for the next chunk
		    n_iter_last_chunk -= take_iter;   // update the number of iteration sent to the next chunk
		    if(n_iter_last_chunk)
			time_last_chunk -= slice_time_last_chunk * take_iter; // update the time ms send to the next chunk
		    else
			time_last_chunk = 0;
		}
		break;
	    }

	    n_iter_block += n_iter_last_chunk;   // we take the whole old chunk
	    time_left -= time_last_chunk;        // time to put into this block

	    if(n_iter_left - n_iter_block < chunkNum - i) { // we have exactly one iteration for each chunk left
#ifdef DEBUG_TQ_FLG
		printf("Exactly one iteration per chunk left \n" );
#endif
		for(int j = chunkNum - 1;j > i; j--){
		    chunkStartInd[j] = chunkStartInd[j+1] - 1;
		}
		i = chunkNum; // OBS! leave the for loop
		break;
	    }
	      

	    chunk_ind++;  // go to the next old chunk
	    if(chunk_ind < chunkNum) {
		time_last_chunk =  block_times_scaled[chunk_ind];
		n_iter_last_chunk = old_chunk_iter[chunk_ind];
		slice_time_last_chunk = time_last_chunk/n_iter_last_chunk;
	    }
	}
    }
    return;
} // balance blocks

static void balance_blocks(const int chunkNum,         // number of chunks in the loop
			   const Timer blockTimers[],  // chunkNum timers for the chunks
			   int chunkStartInd[],        // (chunkNum+1) integers giving the start indices
			   int& run_parallel)         // flag saying if the loop is longs enough to consider it for parallel run
{ //  balance_blocks
    timeunit block_times[MAX_CHUNK_NUM];
    for(int i = 0; i < chunkNum; i++)
	block_times[i] = blockTimers[i].get_nano_last();
    balance_blocks(chunkNum, block_times, chunkStartInd, run_parallel);
}  // balance blocks



static void balance_blocks(const int num_proc,           // number of processors that were participating
			   const timeunit proc_delta_times[],        // delay - when processor "i" joins the execution
			   int&   chunkNum,             // number of "active" chunks in the loop, can be changed depending on num_proc
			   const timeunit block_times[],  // chunkNum timers for the chunks
			   int chunkStartInd[],        // (chunkNum+1) integers giving the start indices
			   int& run_parallel)         // flag saying if the loop is longs enough to consider it for parallel run
{ //  balance_blocks


#ifdef DEBUG_TQ_FLG
    printf("Entering rescheduling for an inner loop\n");
#endif
    assert(MAX_CHUNK_NUM >= chunkNum);
    timeunit block_times_scaled[MAX_CHUNK_NUM]; // time spent in this block ms * scale factor
    int old_chunk_iter[MAX_CHUNK_NUM]; // time spent in this block ms * scale factor

    timeunit total_loop_time(0); // total serial time for the loop

    timeunit ideal_load(0); // chunk size for ideal loading with all processor starting at the same time

    const int n_iter = chunkStartInd[chunkNum] - chunkStartInd[0];

    const int reschedule_threshold_frac =  n_iter; // scaling factor used to avoid floating point calculations = number of loop iterations

    // calculate the total time scaled with reschedule_threshold_frac
    for(int i = 0; i < chunkNum; i++){ 
	total_loop_time += (block_times_scaled[i] = block_times[i] * reschedule_threshold_frac);
	old_chunk_iter[i] = chunkStartInd[i+1] - chunkStartInd[i];
#ifdef DEBUG_TQ_FLG
	printf("Old chunk %d has block_time %d (scaled %d) for %d iterations\n",
	       i, block_times_ms[i].get_msec(), block_times_scaled[i].get_msec(), old_chunk_iter[i] );
	assert(old_chunk_iter[i] >= 0);
#endif
    }

    // ideal load balancing
    ideal_load = total_loop_time / chunkNum ;

    if(ideal_load == 0) 
	{
#ifdef DEBUG_TQ_FLG
	    printf("The loop is too fast - no reason to run in parallel\n");
#endif
	    run_parallel = 0;
	    chunkNum = 1;
	    chunkStartInd[1] = n_iter;
	    return ;
	}
    else if(!run_parallel){
#ifdef DEBUG_TQ_FLG
	printf("The loop is slow and can be run in parallel\n");
#endif
	run_parallel = 1;
	return;
    }

    // only one processor was trying to run the loop
    if(num_proc == 1) { 
#ifdef DEBUG_TQ_FLG
	printf("The loop was run by only one processor - not need to schedule\n");
#endif
	chunkNum = 1;
	chunkStartInd[1] = n_iter;
	return;
    }

    timeunit ideal_load_block[MAX_CHUNK_NUM];

    int new_chunk_num = num_proc;
    timeunit sum_proc_entry_time = 0;
    timeunit serial_loop_left = total_loop_time;

    timeunit proc_delta_times_scaled[MAX_CHUNK_NUM];

    for(int ip = 0; ip < num_proc; ip++) {
	proc_delta_times_scaled[ip] = proc_delta_times[ip] * reschedule_threshold_frac;

#ifdef DEBUG_TQ_FLG
	printf("Proc %d delta time %d (scaled %d)\n",
	       ip, proc_delta_timess[ip].get_msec(), proc_delta_times_scaled[ip].get_msec() );
#endif
	sum_proc_entry_time += proc_delta_times_scaled[ip];

	if(sum_proc_entry_time > serial_loop_left) {
	    // there will be no job for this processor
	    new_chunk_num = ip + 1; 
	    break;
	}

	serial_loop_left -= proc_delta_times_scaled[ip] * (ip + 1);
    }
    
    if (serial_loop_left > 0) 
	ideal_load = serial_loop_left / new_chunk_num ;
    else
	ideal_load = 0;

    ideal_load_block[new_chunk_num - 1] = ideal_load; // the last processor gets the smallest partition
    
    sum_proc_entry_time = 0;

    for(int ip = new_chunk_num -1; ip > 0; --ip) {

	sum_proc_entry_time += proc_delta_times_scaled[ip-1];

	ideal_load_block[ip - 1] = ideal_load + sum_proc_entry_time;

	ideal_load += ideal_load_block[ip - 1];
    }
    

#ifdef DEBUG_TQ_FLG
    {
	printf("Ideal load calculated %d chunks:\n", new_chunk_num);
	for(int ip = 0; ip < new_chunk_num; ++ip) {
	    printf("Block %d gets ideal load %d:\n", ip, ideal_load_block[ip]);
	}
	printf("Real total time %d, distributed time %d\n", total_loop_time, ideal_load);
    }
#endif

    int chunk_ind = 0; // the chunk number during the previous iteration (old chunks)
      
    // estimated time to be distributed from the chunk[chunk_ind]
    timeunit time_last_chunk =  block_times_scaled[0];       

    // number of iterations to be distributed from the chunk[chunk_ind]
    int n_iter_last_chunk = old_chunk_iter[0];
             
    timeunit slice_time_last_chunk = time_last_chunk/n_iter_last_chunk; // estimated time for one slice   from the chunk[chunk_ind]

    int n_iter_left = n_iter;

    for(int i = 0; i < new_chunk_num-1; i++){ // for all chunks except the last one
	timeunit time_left = ideal_load_block[i];    // preferred time 
	int n_iter_block = 0;          // iterations taken in the chunk

#ifdef DEBUG_TQ_FLG
	printf("Calculating iterations for chunk %d\n", i);
#endif
	while(chunk_ind < chunkNum) {  // for all old chunks
	    if(time_last_chunk > time_left + slice_time_last_chunk/2) { // the current old block is larger than we need

		int take_iter = 0;
		if(time_left > 0) {
		    if(slice_time_last_chunk > 0)
			take_iter = (time_left + slice_time_last_chunk/2).ratio(slice_time_last_chunk); // some iterations will go into current new chunk
		    else
			take_iter = n_iter_last_chunk;
		}
		      
		n_iter_block += take_iter;

		if(n_iter_block <= 0) {	      
		    n_iter_block = 1; // at least one
		    take_iter = 1;
		}

#ifdef DEBUG_TQ_FLG
		printf("Chunk %d gets %d iterations\n", i, n_iter_block );
#endif
		n_iter_left -= n_iter_block;

		if(n_iter_left < new_chunk_num - i) { // we have exactly one iteration for each chunk left
#ifdef DEBUG_TQ_FLG
		    printf("Exactly one iteration per chunk left \n" );
#endif
		    for(int j = new_chunk_num - 1;j > i; j--){
			chunkStartInd[j] = chunkStartInd[j+1] - 1;
		    }
		    i = chunkNum; // OBS! leave the for loop
		}
		else {
		    chunkStartInd[i+1] = chunkStartInd[i] + n_iter_block; // calc start index for the next chunk
		    n_iter_last_chunk -= take_iter;   // update the number of iteration sent to the next chunk
		    if(n_iter_last_chunk)
			time_last_chunk -= slice_time_last_chunk * take_iter; // update the time ms send to the next chunk
		    else
			time_last_chunk = 0;
		}
		break;
	    }

	    n_iter_block += n_iter_last_chunk;   // we take the whole old chunk
	    time_left -= time_last_chunk;        // time to put into this block

	    if(n_iter_left - n_iter_block < new_chunk_num - i) { // we have exactly one iteration for each chunk left
#ifdef DEBUG_TQ_FLG
		printf("Exactly one iteration per chunk left \n" );
#endif
		for(int j = new_chunk_num - 1;j > i; j--){
		    chunkStartInd[j] = chunkStartInd[j+1] - 1;
		}
		i = new_chunk_num; // OBS! leave the for loop
		break;
	    }
	      

	    chunk_ind++;  // go to the next old chunk
	    if(chunk_ind < chunkNum) {
		time_last_chunk =  block_times_scaled[chunk_ind];
		n_iter_last_chunk = old_chunk_iter[chunk_ind];
		slice_time_last_chunk = time_last_chunk/n_iter_last_chunk;
	    }
	}
    }
    chunkNum = new_chunk_num;
    chunkStartInd[chunkNum] = n_iter;
    return;
} // balance blocks
static void balance_blocks(const int nproc, 
                           const Timer procStartTimers[],
                           int& chunkNum,
                           const Timer blockTimers[],
			   int chunkStartInd[],
			   int&run_parallel)
{ //  balance_blocks
    timeunit proc_times[MAX_CHUNK_NUM];
    for(int ip = 0; ip < nproc; ip++)
	proc_times[ip] = procStartTimers[ip].get_nano_last();

    timeunit block_times[MAX_CHUNK_NUM];
    for(int i = 0; i < chunkNum; i++) 
	block_times[i] = blockTimers[i].get_nano_last();

    balance_blocks(nproc, proc_times, chunkNum, block_times, chunkStartInd, run_parallel);
}  // balance blocks

//////////////////////////////////////////////////////////////////////////////////////
///  Methods for FGLoopScheduler
int FGLoopScheduler::calc_chunk_num(int num_iter)
{
    int retChunkNum = 0;
    int np = ThreadPool::get_instance()->get_np(); // number of threads
        
    if( num_iter < np * 2) {
        Error("Cannot run in parallel: number of loop iterations < nthreads * 2");
        assert(0);
    }

    return np*2;
}

FGLoopScheduler::FGLoopScheduler(int num_iter):
    LoopScheduler(calc_chunk_num(num_iter)),
    lastTask_(NO_TASK),
    blockTimers(new Timer[chunkNum]),
    n_iter(num_iter)
{
    for(int i = 0; i <= chunkNum; i++) {
        chunkStartInd[i] = num_iter * i / chunkNum;
    }
#ifdef DEBUGFLG
    printf("FGLoopLoop scheduler: constructor\n");
#endif
};

FGLoopScheduler::~FGLoopScheduler(){
    assert(lastTask_ < 0); // the scheduler is not in use
#ifdef DEBUGFLG
    printf("FGLoopLoop scheduler: destructor\n");
#endif
}

void FGLoopScheduler::reset_tasks() {
    AutoLock refLock(queueMutex_);

    if(lastTask_ != END_OF_QUEUE) return;

    lastTask_ = NO_TASK;

    { // balance blocks
	if(run_parallel == -1)
	    run_parallel = 1;
	else
	    balance_blocks(chunkNum, blockTimers, chunkStartInd, run_parallel);
    }  // balance blocks

#ifdef DEBUG_TQ_FLG
    {
	printf("Generated schedule: \n");
	for(int i = 0; i < chunkNum; i++){ 
	    int nit = chunkStartInd[i+1] - chunkStartInd[i];
	    printf("Chunk %d got %d iterations\n", i, nit);
	    assert(nit >= 0);
	}
    }

#endif    

}

int FGLoopScheduler::get_task(int doneTask) {
    register int retTask, checkLeft, checkRight;
    AutoLock refLock(queueMutex_);

    if(!run_parallel) {
	if(doneTask >= 0) {
#ifdef DEBUGFLG
	    assert(doneTask == 0);
	    assert(lastTask_ == END_OF_QUEUE);
#endif
	    blockTimers[0].stop();
	    return NO_TASK;
	}
	if(lastTask_  == NO_TASK) {
	    blockTimers[0].start();
	    lastTask_  = END_OF_QUEUE;
	    return(0);
	}

	return NO_TASK;
    }

    if(doneTask >= 0) {
	blockTimers[doneTask].stop();
    }

    if(lastTask_ == END_OF_QUEUE) { // it might happen that we're called again after end of queue
#ifdef DEBUGFLG
	if(doneTask < 0)
	    printf("Very strange call when chunkNum = %d \n", chunkNum );
#endif
	return -1;
    }

    if(lastTask_ == NO_TASK) { // this is the first call after reset
        retTask = 0;
    }
    else {
        retTask = lastTask_ + 1; 
        if(retTask == chunkNum) { // all done
            lastTask_ = END_OF_QUEUE;  // -2 is used to indicate end of queue
            return NO_TASK;
        }
    }
 
    lastTask_ = retTask; // selected task

    blockTimers[retTask].start();

    return (retTask);
};

//////////////////////////////////////////////////////////////////////////////////////
///  Methods for INFGLoopScheduler
int INFGLoopScheduler::calc_chunk_num(int num_iter)
{
    int retChunkNum = 0;
    int np = ThreadPool::get_instance()->get_np(); // number of threads
        
    if( num_iter < np * 2) {
        Error("Cannot run in parallel: number of loop iterations < nthreads * 2");
        assert(0);
    }

    return np;
}

INFGLoopScheduler::INFGLoopScheduler(int num_iter):
    LoopScheduler(calc_chunk_num(num_iter)),
    lastTask_(NO_TASK),
    blockTimers(new Timer[chunkNum]),
    procStartTimers(new Timer[chunkNum]),
    nproc_(0),
    n_iter(num_iter)
{
    for(int i = 0; i <= chunkNum; i++) {
        chunkStartInd[i] = num_iter * i / chunkNum;
    }
#ifdef DEBUGFLG
    printf("FGLoopLoop scheduler: constructor\n");
#endif
};

INFGLoopScheduler::~INFGLoopScheduler(){
    assert(lastTask_ < 0); // the scheduler is not in use
    delete [] blockTimers;
    delete [] procStartTimers;
#ifdef DEBUGFLG
    printf("FGLoopLoop scheduler: destructor\n");
#endif
}

void INFGLoopScheduler::reset_tasks() {
    AutoLock refLock(queueMutex_);

    if(lastTask_ != END_OF_QUEUE) return;

    lastTask_ = NO_TASK;

    { // balance blocks
	if(run_parallel == -1)
	    run_parallel = 1;
	else
	    balance_blocks(nproc_, procStartTimers, chunkNum, blockTimers, chunkStartInd, run_parallel);
	nproc_ = 0;
    }  // balance blocks

#ifdef DEBUG_TQ_FLG
    {
	printf("Generated schedule: \n");
	for(int i = 0; i < chunkNum; i++){ 
	    int nit = chunkStartInd[i+1] - chunkStartInd[i];
	    printf("Chunk %d got %d iterations\n", i, nit);
	    assert(nit >= 0);
	}
    }

#endif    

}

int INFGLoopScheduler::get_task(int doneTask) {
    register int retTask, checkLeft, checkRight;
    AutoLock refLock(queueMutex_);

    if(!run_parallel) {
	if(doneTask >= 0) {
#ifdef DEBUGFLG
	    assert(doneTask == 0);
	    assert(lastTask_ == END_OF_QUEUE);
#endif
	    blockTimers[0].stop();
	    return NO_TASK;
	}
	if(lastTask_  == NO_TASK) {
	    blockTimers[0].start();
	    lastTask_  = END_OF_QUEUE;
	    return(0);
	}

	return NO_TASK;
    }

    if(doneTask >= 0) {
	blockTimers[doneTask].stop();
    }
    else {
	if(nproc_ > 0) {
	    procStartTimers[nproc_-1].stop();
#ifdef DEBUG_TQ_FLG
	    printf("Number of threads used became %d\n", nproc_+1 );
#endif
	}
	procStartTimers[nproc_].start();
	nproc_++;
    }

    if(lastTask_ == END_OF_QUEUE) { // it might happen that we're called again after end of queue
#ifdef DEBUGFLG
	if(doneTask < 0)
	    printf("Very strange call when chunkNum = %d \n", chunkNum );
#endif
	return -1;
    }

    if(lastTask_ == NO_TASK) { // this is the first call after reset
        retTask = 0;
    }
    else {
        retTask = lastTask_ + 1; 
        if(retTask == chunkNum) { // all done
            lastTask_ = END_OF_QUEUE;  // -2 is used to indicate end of queue
            return NO_TASK;
        }
    }
 
    lastTask_ = retTask; // selected task

    blockTimers[retTask].start();

    return (retTask);
};

        
//////////////////////////
// Methods for DynamicLoopScheduler
int DynamicLoopScheduler::calc_chunk_num(int num_iter, int chunk_size)
{ 
    int retChunkNum = 0;

    int np = ThreadPool::get_instance()->get_np(); // number of threads
        
    int maxChunkNum = num_iter/ chunk_size;  // maximum allowed parallel run
    int minChunkNum = (2 * np); // we need at least 2 cols per processor
        
    if(minChunkNum > maxChunkNum) { // we have more proc than we can use
        retChunkNum = maxChunkNum;
    }
    else {
        retChunkNum =  maxChunkNum; // OBS! TODO: find out the optimal chunkNum. Dynamic adjustment?
    }

    return retChunkNum;
}

DynamicLoopScheduler::DynamicLoopScheduler(int num_iter, int chunk_size):
    LoopScheduler(calc_chunk_num(num_iter,chunk_size)),
    lastTask_(NO_TASK),
    n_iter(num_iter)
{
    for(int i = 0; i <= chunkNum; i++) {
        chunkStartInd[i] = (i * n_iter)/ chunkNum;
    }

};

DynamicLoopScheduler::~DynamicLoopScheduler(){
    assert(lastTask_ == NO_TASK); // the scheduler is not in use
}

void DynamicLoopScheduler::reset_tasks() {
    lastTask_ = NO_TASK;
}

int DynamicLoopScheduler::get_task(int) {
    register int retTask;
    AutoLock refLock(queueMutex_);
    if(lastTask_ < 0) { // this is the first call after reset
        lastTask_= retTask = 0;
    }
    else if(lastTask_ == chunkNum - 1) {
	retTask = NO_TASK;
    }
    else {
        retTask = ++lastTask_; 
    }

    return (retTask);
};
