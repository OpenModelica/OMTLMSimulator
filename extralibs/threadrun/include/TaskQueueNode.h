#ifndef _TaskQueueNode_H_
#define _TaskQueueNode_H_

#include "TaskQueue.h"

// Class TaskQueueNode represents a node in the currently running
// task graph (tree or forest). The "owner" thread creates a TaskQueueNode before
// running a TaskQueue and puts a pointer on it into the global structure.
// The owner_run method is then executed. 
//  isRunnable_ flag indicates if the associated TaskQueue has pottential for
// running in parallel. The flag is set to false as soon as any call to
// run_task on the queue completes. Note that non-runable TaskQueue can have
// runnable children.
//  Owner thread is responsible for managing the root nodes of the graph
// while the internal nodes are managed by the constructor/destructor of
// this class. The general locking protocol: from parent to branch, i.e.,
//  if a thread is holding a lock on a node it is not allowed to try 
// to lock any ancestor node. Recursive locking is not allowed (asserted 
// if compiled with DEBUG_TQ_FLG)


class TaskQueueNode:public SimpleLock {

    TaskQueueNode* parentQNode_; // parent node in the tree

    // volatile TaskQueueNode* childQNode_;  // the head (left-most node) of the list of child nodes

    //! left is 0 for the head node of the list, right is 0 for the tail node.
    //! volatile TaskQueueNode *leftSiblingQNode_, *rightSiblingQNode_; // sibling nodes.


    //! Array of pointer to child nodes
    volatile TaskQueueNode* childQNodes_[ThreadPool::MAX_N_THREADS];
    
    //! The node is runnable flag, that is owner_run has not completed
    volatile bool isRunnable_; 

    //! Associated task queue
    TaskQueue* queue_;

    //! Number of worker threads running on the queue, not counting the owner
    volatile int n_runners_;  

    //! ID of the owner thread
    const int ownerThreadID_; 

    // Owner might need to wait for a worker on this condition
    SimpleCond ownerWait_; 

protected:

    

public:

//!Constructor.
//! Input:
//!   ownerThreadID - ID of the thread creating this node
//!   queueIn - the queue interface to be run in parallel 
//!   parentIn - the parent queue node
//!   Att!!! the thread must own the lock on the parent queue node before
//!   calling the constructor or alternatively the lock in the ThreadPool object
//!   for the current thread root node. This lock should be released and 
//!   the owner_run method should be called directly after the queue node is constructed
    TaskQueueNode(const int ownerThreadID,
                  TaskQueue* queueIn,
                  TaskQueueNode* parentIn);

    //! This function runs the queue supplied in the constructor. Note that the
    //! caller should not own the lock on this node. The method terminates
    //! when the associated TaskQueue is completed (including all possible
    //! child queues) and this TaksQueueNode can be destroyed.
    void owner_run();
    
    //! The thread calling the destructor should not own the lock on this node nor
    //! on the parent node. However if this is the root node of a processor
    //! the owner must prevent stealing before destroying the node.
    ~TaskQueueNode();

    //! Get the parent TaksQueueNode, returns 0 for root nodes.
    TaskQueueNode* get_parent_node() const { return parentQNode_; }

  
    //! Try to help this taskqueue or its children to execute (i.e. steal some work).
    //! This function should be used by threads other than the owner
    //! thread after acquiring a lock on this node. It returns only if there is no
    //! currently available work. Note, however that some new nodes with
    //! more work can be created later on.
    void helper_run_subtree(const int workerThreadID);


    //! Try to help children of this node to execute (i.e. steal some work).
    //! This function uses breadth first search to find a child node
    //! with runnable queue (current node should be not runnable).
    //! It then calls helper_run_queue on that node.
    //! The caller must have a lock on this node before calling the function.
    void help_child(const int workerThreadID);

    //! Check if the queue has potential for parallel execution:
    //! Runnable or have child nodes
    bool has_parallel_jobs();
    
protected:
    //! Help this taskqueue to execute (i.e. steal some work).
    //! The method is called by helper_run_subtree & help_child
    //! to make work on the queue. Caller owns the lock on the node.
    void helper_run_queue(const int workerThreadID);

    //! The two get_runnable_child methods perform the bredth-first search
    //! over the task queue nodes tree for the top-most runnable node.
    //! If a runnable node is found - it is locked and returned to the caller.
    //! Return the pointer to the found node or 0 if there no such nodes.
    TaskQueueNode* get_runnable_child();
    TaskQueueNode* get_runnable_child(int level, bool& moreLevels);

};


#endif
