#include "rwlock.h"

RWLock:: RWLock(){
#ifdef RWLOCK
  this->readersWaiting = 0;
  this->readersActive = 0;
  this->writersActive = 0;
  int stat;
  stat = pthread_mutex_init(&m_mutex, NULL);
  if(stat != 0) return;
  stat = pthread_cond_init(&readCondVar,NULL);
  if( stat != 0){
    pthread_mutex_destroy(&m_mutex);
    return;
  }
  stat = pthread_cond_init(&writeCondVar, NULL);
  if( stat != 0){
    pthread_cond_destroy(&readCondVar);
    pthread_mutex_destroy(&m_mutex);
    return;
  }
  flag = 1;

#else
  pthread_mutex_init(&m_mutex,NULL);
#endif
}

#ifdef RWLOCK


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

  int stat;
  int stat1;

  stat = pthread_mutex_lock(&m_mutex);
  if(stat != 0) return;
  readersActive--;
  if (readersActive == 0 && writersWaiting > 0){
    stat = pthread_cond_signal(&writeCondVar);
  }
  stat1 = pthread_mutex_unlock(&m_mutex);
}

void RWLock::startWrite(){
  int stat;
  stat = pthread_mutex_lock(&m_mutex);
  if(stat != 0) return;
  if(writersActive || readersActive > 0){
    writersWaiting++;
    while(writersActive || readersActive > 0){
      stat = pthread_cond_wait(&writeCondVar, &m_mutex);
      if(stat != 0) break;
    }
    writersWaiting--;  
  }
  if (stat == 0) writersActive = 1;
  pthread_mutex_unlock(&m_mutex);
}

void RWLock:: doneWrite(){
  int stat;
  stat = pthread_mutex_lock(&m_mutex);
  if (stat != 0) return;
  writersActive = 0;
  if(readersWaiting > 0){
    stat = pthread_cond_broadcast(&readCondVar);
    if(stat != 0){
      pthread_mutex_unlock(&m_mutex);
      return;
    }
  }
  else if (writersWaiting > 0){
    stat = pthread_cond_signal(&writeCondVar);
    if(stat != 0){
      pthread_mutex_unlock(&m_mutex);
      return;
    }
  }
  stat = pthread_mutex_unlock(&m_mutex);
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
