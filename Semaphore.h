#ifndef SEMAPHORE
#define SEMAPHORE

#include<mutex>
#include<condition_variable>

/**
*   Simple Semaphore for handling event queue between Local_monitor and Sender.
*
*/

class Semaphore{
public:
    Semaphore();
    void notify();
    void wait();

private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};


#endif
