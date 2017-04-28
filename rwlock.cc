#include "rwlock.h"
#include <pthread.h>

RWLock:: RWLock(){
#ifdef RWLOCK
  readersWaiting = 0;
  readersActive = 0;
  writersActive = 0;
  writersWaiting = 0;
  pthread_cond_init(&readCondVar, NULL);
  pthread_cond_init(&writeCondVar, NULL);

#endif
  pthread_mutex_init(&m_mutex, NULL);
}

pthread_mutex_t* RWLockL::getmutex(){
  return &m_mutex;
}
void RWLock:: startRead(){
  int stat;
  stat = pthread_mutex_lock(&m_mutex);
  if (stat != 0) return;
  if (writersActive){
    readersWaiting++;
    while(writersActive){
      stat = pthread_cond_wait(&readCondVar, &m_mutex);
      if (stat != 0) break;
    }
    readersWaiting--;
  }
  if( stat == 0) readersActive++;
  pthread_mutex_unlock(&m_mutex);
}

void RWLock:: doneRead(){

  pthread_mutex_lock(&m_mutex);
#ifdef RWLOCK
  readersWaiting++;
  while(writersWaiting != 0 || writersActive!=0){
    pthread_cond_wait(&readCondVar, &m_mutex);
  }
  readersWaiting--;
  readersActive++;
  pthread_mutex_unlock(&m_mutex);
#endif
}

void RWLock::startWrite(){
  pthread_mutex_lock(&m_mutex);
#ifdef RWLOCK
  writersWaiting++;
  while(writersActive!=0 || readersActive!=0){
    pthread_cond_wait(&writeCondVar, &m_mutex);
  }
  writersWaiting--;
  writersActive++;
  pthread_mutex_unlock(&m_mutex);
#endif
}

void RWLock:: doneWrite(){
#ifdef RWLOCK
  pthread_mutex_lock(&m_mutex);
  writersActive--;
  if(writersWaiting > 0 ){
    pthread_cond_signal(&writeCondVar);
  }
  else{
    pthread_cond_signal(&readCondVar);
  }
#endif
  pthread_mutex_unlock(&m_mutex);
}


RWLock:: ~RWLock(){
#ifdef RWLOCK
pthread_mutex_unlock(&readCondVar);
pthread_mutex_destroy(&writeCondVar);
#endif
pthread_mutex_destroy(&m_mutex);
}
