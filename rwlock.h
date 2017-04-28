//#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

class RWLock{
private:
#ifdef RWLOCK
  
  // Mutex lock
  pthread_mutex_t m_mutex;
  
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

  // Flag
  int flag;



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

