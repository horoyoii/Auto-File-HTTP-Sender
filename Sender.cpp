#include"Sender.h"

Sender::Sender(std::string server_ip, std::string path, std::string port, std::string dir_path, std::queue<std::string>* eq, Semaphore* sem, Mutex* mtx){

    httpClient = new HttpClient(server_ip, path, port);
    this->event_queue = eq;
    this->sem = sem;
    this->mtx = mtx;
    this->dir_path = dir_path;
}


void Sender::run(){
    
    while(1){        
        sem->wait();        

        // lock 
        mtx->Lock();
        
        std::string event = event_queue->front(); event_queue->pop();
        std::cout<<"get event : "<<event<<std::endl;
        
        mtx->UnLock();
        /** 
        *   Need to time for filesystem...
            
            When get the signal from monitor,
            httpClient may read a file before 
            the original file was wriiten to filesystem completely!!

        */
        sleep(1);
         
        // ignore the .swp and .swx files.
        if(event.find(".swp") != std::string::npos || event.find(".swx") != std::string::npos)
            continue;
        
        
        std::cout<<"send to server : "<<event<<std::endl;
        httpClient->sendFile(dir_path+ "/" + event);
        // file not exist 시에 문제 발생        
    }


}



