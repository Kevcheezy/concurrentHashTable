/**
 * Code is based on
 *http://www.algolist.net/Data_structures/Hash_table/Chaining
 *
 **/



#include <iostream>
#include "phash.h"
#include <unistd.h>
#include "rwlock.h"

LinkedHashEntry:: LinkedHashEntry(int key, int value) {
            this->key = key;
            this->value = value;
            this->next = NULL;
}

int LinkedHashEntry:: getKey() {
  return key;
}

int 
LinkedHashEntry:: getValue() {
            return value;
}
 
void LinkedHashEntry:: setValue(int value) {
  this->value = value;
}
 

LinkedHashEntry * 
LinkedHashEntry:: getNext() {
            return next;
}
 
void LinkedHashEntry:: setNext(LinkedHashEntry *next) {
  this->next = next;
}


const int TABLE_SIZE = 128;
 
HashMap::HashMap() {
  rwMapLock = RWLock();
  table = new LinkedHash2Entry*[TABLE_SIZE];
  for (int i = 0; i < TABLE_SIZE; i++)
    table[i] = NULL;
}

int HashMap:: get(int key) {
  // usleep(1);
#ifdef RWLOCK
  rwMapLock.startRead(); //(2) begin reading
#else
  pthread_mutex_lock(rwMapLock.getmutex()); //(1) lock
  #endif
  int hash = (key % TABLE_SIZE);
  if (table[hash] == NULL){
    #ifdef RWLOCK
    rwMapLock.doneRead(); //(2) done reading
    #else
    pthread_mutex_unlock(rwMapLock.getmutex()); //(1) unlock
    #endif
    return -1;
  }
  else {
    LinkedHashEntry *entry = table[hash];
    while (entry != NULL && entry->getKey() != key)
      entry = entry->getNext();
    if (entry == NULL){
      #ifdef RWLOCK
      rwMapLock.doneRead(); //(2) done reading
      #else
      pthread_mutex_unlock(rwMapLock.getmutex()); //(1) unlock
      #endif
      return -1;
    }
    else{
      #ifdef RWLOCK
      rwMapLock.doneRead(); //(2) done reading
      #else
      pthread_mutex_unlock(rwMapLock.getmutex()); //(1) unlock
      #endif
	return entry->getValue();
    }
  }
 // @@ Mutex unlock here @@
  #ifdef RWLOCK
  rwMapLock.doneRead(); //(2) done reading
  #else
pthread_mutex_unlock(rwMapLock.getmutex()); // (1) unlock
  #endif
}
  
void HashMap::put(int key, int value) {
  #ifdef RWLOCK
  rwMapLock.startWrite(); //(2) begin write
  #else
  pthread_mutex_lock(rwMapLock.getmutex()); //(1) lock
  #endif
  int hash = (key % TABLE_SIZE);
  if (table[hash] == NULL)
    table[hash] = new LinkedHashEntry(key, value);
  else {
    LinkedHashEntry *entry = table[hash];
    while (entry->getNext() != NULL)
      entry = entry->getNext();
    if (entry->getKey() == key)
      entry->setValue(value);
    else
      entry->setNext(new LinkedHashEntry(key, value));
  }
  #ifdef RWLOCK
  rwMapLock.doneWrite(); //(2) done writing
  #else
  pthread_mutex_unlock(rwMapLock.getmutex()); //(1)
  #endif
}
 

void HashMap:: remove(int key) {
  #ifdef RWLOCK
  rwMapLock.startWrite(); //(2) begin writing
  #else
  pthread_mutex_unlock(rwMapLock.getmutex()); // (1) lock
  #endif
  int hash = (key % TABLE_SIZE);
  if (table[hash] != NULL) {
    LinkedHashEntry *prevEntry = NULL;
    LinkedHashEntry *entry = table[hash];
    while (entry->getNext() != NULL && entry->getKey() != key) {
      prevEntry = entry;
      entry = entry->getNext();
    }
    if (entry->getKey() == key) {
      if (prevEntry == NULL) {
        LinkedHashEntry *nextEntry = entry->getNext();
        delete entry;
        table[hash] = nextEntry;
      } 
      else {
        LinkedHashEntry *next = entry->getNext();
        delete entry;
        prevEntry->setNext(next);
      }
    }
  }
  #ifdef RWLOCK
  rwMapLock.doneWrite(); //(2) done writing
  #else
  pthread_mutex_unlock(rwMapLock.getmutex()); //(1) unlock
  #endif
}
 
HashMap:: ~HashMap() {
            for (int i = 0; i < TABLE_SIZE; i++)
                  if (table[i] != NULL) {
                        LinkedHashEntry *prevEntry = NULL;
                        LinkedHashEntry *entry = table[i];
                        while (entry != NULL) {
                             prevEntry = entry;
                             entry = entry->getNext();
                             delete prevEntry;
                        }
                  }
            delete[] table;
      }


