/*
 File: scheduler.C

 Author:
 Date  :

 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "scheduler.H"
#include "thread.H"
#include "console.H"
#include "utils.H"
#include "assert.H"
#include "simple_keyboard.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   S c h e d u l e r  */
/*--------------------------------------------------------------------------*/

Scheduler::Scheduler() {
  // assert(false);
  queueSize = 0;
  Console::puts("Constructed Scheduler.\n");
}

void Scheduler::yield() {
  // assert(false);
  //BONUS POINTS 1: Deal with Interrupts
  // Machine::disable_interrupts();//disable interupts while selecting next thread to dispatch
  if (queueSize != 0){
    queueSize--;
    Thread* next = readyQueue.dequeue();// get next thread
    Thread::dispatch_to(next); // run the new thread
  }
  // Machine::enable_interrupts();
}

void Scheduler::resume(Thread * _thread) {
  // assert(false);
    readyQueue.enqueue(_thread); // add thread to queue
    queueSize++;
}

void Scheduler::add(Thread * _thread) {
  // assert(false);
    readyQueue.enqueue(_thread); // add thread to queue
    queueSize++;
}

void Scheduler::terminate(Thread * _thread) {
  // assert(false);
    bool found = false;
    for (int i = 0;i < queueSize; i++){
        Thread* curt = readyQueue.dequeue();
        if (curt->ThreadId() == _thread->ThreadId())
            found = true;//variable used to check if we found the thread we wanted to terminate
        else //if temp matches thread do not re-enqueue it back on the list
            readyQueue.enqueue(curt);//else just push it back on
    }
    if (found)
        queueSize--;
}
