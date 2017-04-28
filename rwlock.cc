#include "rwlock.h"

RWLock:: RWLock(){
#ifdef RWLOCK
  
#else
  pthread_mutex_init(&m_mutex,NULL);
#endif
}

#ifdef RWLOCK
pthread_rwlock_t* RWLock:: getrwlock(){
  return &rwlock;
}
#else
pthread_mutex_t* RWLock:: getm_mutex(){
  return &m_mutex;
}
#endif

//RWLock:: startRead(){}

//RWLock:: doneRead(){}

//RWLock:: startWrite(){}

//RWLock:: doneWrite(){}

RWLock:: ~RWLock(){
#ifdef RWLOCK
#else
  pthread_mutex_unlock(&m_mutex);
  pthread_mutex_destroy(&m_mutex);
#endif
}
