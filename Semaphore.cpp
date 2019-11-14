#include"Semaphore.h"

Semaphore::Semaphore(){
    count =0;
}

void Semaphore::notify(){
        std::unique_lock<std::mutex> lock(mtx); // RAII
        
        count++;
        
        cv.notify_one(); // wake waiting one, if no one, do nothing
    }

void Semaphore::wait(){
        std::unique_lock<std::mutex> lock(mtx);

        while(count == 0){
            cv.wait(lock);
        }

        count--;
    }

