#include "rwlock.h"

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

void RWLock:: startRead(){
    pthread_mutex_lock(&m_mutex);
  #ifdef RWLOCK
    readersWaiting++;
    while(writersActive != 0 || writersWaiting != 0){
      pthread_cond_wait(&readCondVar, &m_mutex);
    }
    readersWaiting--;
    readersActive++;
    pthread_mutex_unlock(&m_mutex);
  #endif
}

void RWLock:: doneRead(){

#ifdef RWLOCK
  pthread_mutex_lock(&m_mutex);
  readersActive--;
  if(readersActive == 0 && writersWaiting > 0){
    pthread_cond_signal(&writeCondVar);
  }
#endif
  pthread_mutex_unlock(&m_mutex);
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
pthread_cond_destroy(&readCondVar);
pthread_cond_destroy(&writeCondVar);
#endif
pthread_mutex_destroy(&m_mutex);
}
