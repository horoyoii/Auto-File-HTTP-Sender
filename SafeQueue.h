#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include<mutex>
#include<queue>


/**
*    The SafeQueue is a std::queue wrapper for thread-safe operations.
*    Especially, concurrency control is obtained via coarse-grained way; a single lock.
*    
*    dequeue() is using a lock_guard, not a unique_lock. 
*    it means that when this function is called, it guarantees that 
*    there are always items in the queue. 
*/
template<class T>
class SafeQueue{
private:
    std::queue<T> q;
    std::mutex m;    

public:
    SafeQueue(void)
        :q(), m(){}
    
    
    void enqueue(T t){
        std::lock_guard<std::mutex> lock(m);
    
        q.push(t);
    }
    
    
    T dequeue(void){
        std::lock_guard<std::mutex> lock(m);
    
        T res = q.front();
        q.pop();
    
        return res;
    }
};
#endif
