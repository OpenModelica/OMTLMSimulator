#include "TaskQueueNode.h"
#include <assert.h>
#include <stdio.h>

//#define DEBUG_TQ_FLG

TaskQueueNode::TaskQueueNode(int ownerThreadID,
                             TaskQueue* queueIn,
                             TaskQueueNode* parentIn): 
    parentQNode_(parentIn),
    isRunnable_(true),
    queue_(queueIn),
    n_runners_(0),
    ownerThreadID_(ownerThreadID)
{
#ifdef DEBUGFLG
    assert(ThreadPool::get_instance()->get_threadID_unsafe() == ownerThreadID);
#endif
    {
	for(int i = 0; i< ThreadPool::MAX_N_THREADS; i++) childQNodes_[i] = 0;
    }

    // If there is a parent node -> register as child
    if(parentQNode_) {
        if(ThreadPool::SET_POINTER_IS_ATOMIC) parentQNode_->lock();
        parentQNode_->childQNodes_[ownerThreadID] = this;
        if(ThreadPool::SET_POINTER_IS_ATOMIC) parentQNode_->unlock();
    }
}

void TaskQueueNode::owner_run() {
#ifdef DEBUG_TQ_FLG
    fprintf(stdout,"Thread %d will run on queue %x as owner. Node %x, parent %x\n",
	   ThreadPool::get_instance()->get_threadID_unsafe(),
	   queue_,
	   this,
	   parentQNode_);
#endif

    TaskQueueNode* ancestor = parentQNode_;
    while(ancestor != 0) {
	if(!ancestor -> isRunnable_ && ancestor->ownerThreadID_ != ownerThreadID_) {
	    ancestor->lock();
	    ancestor ->ownerWait_.signal();
	    ancestor->unlock();
	}	
	ancestor = ancestor->parentQNode_;
    }

    queue_->run_task(ownerThreadID_,ThreadPool::get_instance()->get_np());  // run the queue

#ifdef DEBUG_TQ_FLG
    fprintf(stdout,"Thread %d done with queue %x as owner. Node %x, parent %x\n",
	   ThreadPool::get_instance()->get_threadID_unsafe(),
	   queue_,
	   this,
	   parentQNode_);
#endif
    lock();
    isRunnable_ = false;  // nothing to run
     
    while(n_runners_ != 0) { // some child nodes are still running
#ifdef DEBUG_TQ_FLG
    fprintf(stdout,"Thread %d entering child search. Node %x, parent %x\n",
	   ThreadPool::get_instance()->get_threadID_unsafe(),
	   queue_,
	   this,
	   parentQNode_);
#endif
        help_child(ownerThreadID_);     // try to go down the QNode tree and help
	
	lock();
	
	if(n_runners_ > 0) {
#ifdef DEBUG_TQ_FLG
	    fprintf(stdout,"Thread %d entering owner wait. Node %x, parent %x\n",
		   ThreadPool::get_instance()->get_threadID_unsafe(),
		   queue_,
		   this,
		   parentQNode_);
#endif
	    ownerWait_.wait(*this);
	}
    }
    unlock();          
#ifdef DEBUG_TQ_FLG
    fprintf(stdout,"Thread %d cleaning up queue %x as owner. Node %x, parent %x\n",
	   ThreadPool::get_instance()->get_threadID_unsafe(),
	   queue_,
	   this,
	   parentQNode_);
#endif
}


TaskQueueNode:: ~TaskQueueNode() {
#ifdef DEBUGFLG
    assert(isRunnable_ == false);  // nothing to run
    assert(n_runners_ == 0); // noone is running
#endif

    if(parentQNode_) { // the node is not a root. manage the tree
        parentQNode_->lock(); // lock parent
        lock();      // lock this node

	parentQNode_->childQNodes_[ownerThreadID_] = 0; // clear there

        unlock();
        parentQNode_->unlock();
    }
    else { 
        lock(); // just to be sure that no thread is working with this queue node
#ifdef DEBUGFLG
	{
	    for(int i = 0; i< ThreadPool::MAX_N_THREADS; i++) assert(childQNodes_[i] == 0);
	}
#endif
        unlock();
    }
}

void TaskQueueNode::helper_run_subtree(const int workerThreadID)  {
    if(isRunnable_) { // this queue has potential for parallel execution
        helper_run_queue(workerThreadID);
    }
    else {
      help_child(workerThreadID);
    }
};

void TaskQueueNode::helper_run_queue(const int workerThreadID)  {
#ifdef DEBUGFLG
    assert(isRunnable_ == true);
#endif
    n_runners_++;
    
    ThreadPool* pool = ThreadPool::get_instance();
    TaskQueueNode* savedQNode = pool->threadCurQueues[workerThreadID];
    pool->threadCurQueues[workerThreadID] = this;

    unlock();
#ifdef DEBUG_TQ_FLG
    fprintf(stdout,"Thread %d will run on queue %x as helper. Node %x, parent %x\n",
	   ThreadPool::get_instance()->get_threadID_unsafe(),
	   queue_,
	   this,
	   parentQNode_);
#endif
    queue_->run_task(workerThreadID,ThreadPool::get_instance()->get_np()); // run it!

#ifdef DEBUG_TQ_FLG
    fprintf(stdout,"Thread %d done with queue %x as helper. Node %x, parent %x\n",
	   ThreadPool::get_instance()->get_threadID_unsafe(),
	   queue_,
	   this,
	   parentQNode_);
#endif
    lock();
    isRunnable_ = false; // nothing left in this queue
    n_runners_--;
    pool->threadCurQueues[workerThreadID] = savedQNode;
    if(n_runners_ == 0) ownerWait_.signal();

    unlock();
};

void TaskQueueNode::help_child(const int workerThreadID)  {
    TaskQueueNode* node = get_runnable_child();
    unlock();
    if(!node) return;
    //    n_runners_ ++;

    node->helper_run_queue(workerThreadID);

//    node->unlock();
//     lock();
//     n_runners_--;
//     if(n_runners_ == 0) ownerWait_.signal();
}

TaskQueueNode* TaskQueueNode::get_runnable_child() {
    TaskQueueNode* retNode = 0; // the found runnable node 
    int level= 0;               // how deep down the tree we found the node
    bool moreLevels;            // should we search deeper
    do {
        moreLevels = false;
        level++;
        retNode = get_runnable_child(level, moreLevels);
    } while(!retNode && moreLevels); // continue until we find something or there's no nodes below level
    return retNode;
}

TaskQueueNode* TaskQueueNode::get_runnable_child(int level, bool& moreLevels) {
        
    TaskQueueNode* retNode = 0;

    if(level == 1) { // this is the level we're checking now
	for(int i = 0; i < ThreadPool::MAX_N_THREADS; i++ ) {
	    TaskQueueNode* curChild = const_cast<TaskQueueNode*>(childQNodes_[i]);
            if(curChild) {
		moreLevels = true;          // there are more levels
		if( curChild->isRunnable_) { // this node should be parallelized
		    curChild->lock();
		    if(curChild->isRunnable_) {
			retNode = curChild; // locked node is returned
			break;
		    }
		    else {
			curChild->unlock();
		    }
		}
            }
	}
    }
    else {
	for(int i = 0; i < ThreadPool::MAX_N_THREADS; i++ ) {
	    TaskQueueNode* curChild = const_cast<TaskQueueNode*>(childQNodes_[i]);
            if(curChild) {
		curChild->lock(); // we have to lock the whole path to the leaf node
		retNode = curChild->get_runnable_child( level - 1,moreLevels); // go deeper
		curChild->unlock();
		break;
            }
	}
    }
    return retNode;
}

bool TaskQueueNode::has_parallel_jobs() {
    if( isRunnable_) return true;
    for(int i = 0; i < ThreadPool::MAX_N_THREADS; i++) {
	if(0 != childQNodes_[i]) return true;
    }
    return false;
}
