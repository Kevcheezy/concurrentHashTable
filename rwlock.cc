#include "rwlock.h"

RWLock:: RWLock(){
#ifdef RWLOCK
  this->readersWaiting = 0;
  this->flagWriterWaiting = 0; // no writer waiting
#else
  pthread_mutex_init(&m_mutex,NULL);
#endif
}

#ifdef RWLOCK


void RWLock:: startRead(){
  int waitRetVal;
  //lock mutex
  pthread_mutex_lock(&m_mutex);
  //wait for good condition
  while(flagWriterWaiting){
    waitRetVal = pthread_cond_wait( &condVar, &m_mutex );
  }
  //increment readers waiting
  readersWaiting++;
  //unlock mutex
  pthread_mutex_unlock(&m_mutex);
}

void RWLock:: doneRead(){
  int sigRetVal;
  //decrement readers waiting
  readersWaiting--;
  //signalling cond if readersWaiting has become 0 (both while holding mutex lock)
  if(readersWaiting == 0){
    sigRetVal = pthread_cond_signal(&condVar);
  }  
}

void RWLock::startWrite(){
  int waitRetVal;
  //lock mutex
  pthread_mutex_lock(&m_mutex);
  //wait if a thread is currently writing OR someone is waiting to read
  while(flagWriterWaiting || readersWaiting > 0){
    waitRetVal = pthread_cond_wait(&condVar, &m_mutex);
  }
  // done waiting, now change flag (currently writing)
  flagWriterWaiting = 1;
  // unlock mutex
  pthread_mutex_unlock(&m_mutex);
}

void RWLock:: doneWrite(){
  // done writing (change flag)
  flagWriterWaiting = 0;
  // broadcast to everyone that it is done
  int broadcastRetVal;
  broadcastRetVal = pthread_cond_broadcast(&condVar);
}

#else
pthread_mutex_t* RWLock:: getm_mutex(){
  return &m_mutex;
}
#endif


RWLock:: ~RWLock(){
  pthread_mutex_unlock(&m_mutex);
  pthread_mutex_destroy(&m_mutex);
}
