#ifndef MUTEX
#define MUTEX

#include<pthread.h>

class Mutex{
public:
    Mutex();
    
    virtual ~Mutex();

    int Lock();

    int UnLock();

private:
    pthread_mutex_t m_plock;
};





#endif
