/*
    Author: R. Bettati, Joshua Capehart
            Department of Computer Science
            Texas A&M University

	    A thread scheduler.

*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/


#include "machine.H"
#include "thread.H"
#include "utils.H"

/*--------------------------------------------------------------------------*/
/* !!! IMPLEMENTATION HINT !!! */
/*--------------------------------------------------------------------------*/
/*
    One way to proceed is to implement the FIFO scheduling policy inside
    class 'Scheduler'.

    If you plan to implement a Round-Robin Scheduler, derive it from class
    'Scheduler', say as class 'RRScheduler'. The class 'RRScheduler' is
    really just a FIFO scheduler with THREE MODIFICATIONS:
    1. It manages a timer, which fires at the end-of-quantum (EOQ).
    (For details on how to set up a timer and how to handle timer interrupts
    see the 1-second timer in 'kernel.C'.)  The timer is set up in the
    constructor.
    2. It uses an additional function, the EOQ handler. This function gets
    called whenever an EOQ timer event fires. The EOQ handler forces the
    current thread to call the scheduler's 'yield' function.
    3. The 'yield' function must be modified to account for unused quantum
    time. If a thread voluntarily yields, the EOQ timer must be reset in order
    to not penalize the next thread.

    (Note that this qualifies as programming at about the level of a baboon.
     Much better woudl be to have the abstract class 'Scheduler' implement
     the basic scheduling MECHANISMS and provide abstract funtions to define
     the queue management POLICIES in derived classes,
     such as 'FIFOScheduler'.)

 */

/*--------------------------------------------------------------------------*/
/* SCHEDULER */
/*--------------------------------------------------------------------------*/

//data structure of queue for implementation of ready queue
class Node{
    private:
    Thread* thread;
    Node* next;
    public:
    Node(){
        thread = NULL;
        next = NULL;
    }
    Node(Thread* t){
        thread = t;
        next = NULL;
    }
    Node(Node& n){//copy constructor
        thread = n.thread;
        next = n.next;
    }

    //add thread to the end of ready queue
    void enqueue(Thread* t){
        if (thread == NULL)
            thread = t;
        else
            if (next == NULL)// check if next exists
                next=new Node(t);//if not set next
            else
                next->enqueue(t);//else call enqueue on next
    }

    Thread* dequeue(){//removes thread at the beginning of the list and return it
        if (thread == NULL)//case where list is empty
            return NULL;
        if (next != NULL){
            Thread* popped_t = thread;// save oridinal thread
            thread = next->thread;//set thread to the next thread
            Node* consumed = next;
            next=next->next;//set next to the next of the new head
            delete consumed;// clean  up old node
            return popped_t; //return the original thread
        }
        Thread* temp=thread;//temp pointer to return
        thread=NULL;//set thread to null so that push works correctly
        return temp;//case where there is only one item in list, return thread
    }
};

class Scheduler {

  /* The scheduler may need private members... */
  Node    readyQueue;
  int     queueSize;//book keeping for easier things like printing

public:

   Scheduler();
   /* Setup the scheduler. This sets up the ready queue, for example.
      If the scheduler implements some sort of round-robin scheme, then the
      end_of_quantum handler is installed in the constructor as well. */

   /* NOTE: We are making all functions virtual. This may come in handy when
            you want to derive RRScheduler from this class. */

   virtual void yield();
   /* Called by the currently running thread in order to give up the CPU.
      The scheduler selects the next thread from the ready queue to load onto
      the CPU, and calls the dispatcher function defined in 'Thread.H' to
      do the context switch. */

   virtual void resume(Thread * _thread);
   /* Add the given thread to the ready queue of the scheduler. This is called
      for threads that were waiting for an event to happen, or that have
      to give up the CPU in response to a preemption. */

   virtual void add(Thread * _thread);
   /* Make the given thread runnable by the scheduler. This function is called
      after thread creation. Depending on implementation, this function may
      just add the thread to the ready queue, using 'resume'. */

   virtual void terminate(Thread * _thread);
   /* Remove the given thread from the scheduler in preparation for destruction
      of the thread.
      Graciously handle the case where the thread wants to terminate itself.*/

};



#endif
