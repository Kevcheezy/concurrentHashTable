//#include<stdio.h>

#include<pthread.h>
#include<semaphore.h>

#ifndef RWLOCK_H
#define RWLOCK_H


class RWLock{
private:
#ifdef RWLOCK
  
  
  // Num. readers waiting
  int readersWaiting;

  // Num. readers active
  int readersActive;
  
  // Num. writers active
  int writersActive;

  // Num writers waiting
  int writersWaiting;
  
  // PThread reading/writing conditional variable
  pthread_cond_t readCondVar;
  pthread_cond_t writeCondVar;

#endif
  // Mutex lock
  pthread_mutex_t m_mutex; 

 
public:
    	RWLock();
    	~RWLock();
	
//	pthread_mutex_t* getmutex();	
	//Reader
    	void startRead();
    	void doneRead();
	
	// Writer
    	void startWrite();
    	void doneWrite();
      
};
#endif