#include"Mutex.h"


Mutex::Mutex(){
    m_plock = PTHREAD_MUTEX_INITIALIZER;
}

Mutex::~Mutex(){
    pthread_mutex_unlock(&m_plock);
}

int Mutex::Lock(){
    return pthread_mutex_lock(&m_plock);
}

int Mutex::UnLock(){
    return pthread_mutex_unlock(&m_plock);
}
