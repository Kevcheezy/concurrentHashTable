//#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

class RWLock{
private:
#ifdef RWLOCK
  
  // Placeholder (DO NOT IMPLEMENT)
  pthread_rwlock_t rwlock;
  // Mutex lock
  pthread_mutex_t m_mutex=PTHREAD_MUTEX_INITIALIZER;
  // Num. readers waiting
  int readersWaiting;
  // Flag for if a writer is waiting (0:No/1:Yes)
  int flagWriterWaiting;
  // PThread conditional variable
  pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;

#else 
  pthread_mutex_t m_mutex=PTHREAD_MUTEX_INITIALIZER; 
#endif
 
public:
    	RWLock();
    	~RWLock();
	
#ifdef RWLOCK 
		
	//Reader
    	void startRead();
    	void doneRead();
	
	// Writer
    	void startWrite();
    	void doneWrite();
	
#else
	// Getter for mutex lock
	pthread_mutex_t* getm_mutex();
#endif

};

