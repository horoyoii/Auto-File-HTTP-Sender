#include"Sender.h"

Sender::Sender(std::string server_ip, std::queue<std::string>* eq, Semaphore* sem){
    this->server_ip = server_ip;
    this->event_queue = eq;
    this->sem = sem;
}


void Sender::run(){
    
    while(1){
        
        //TODO:  mutex lock for queue
        sem->wait();
        
        std::string event = event_queue->front(); event_queue->pop();
        std::cout<<"get event : "<<event<<std::endl;
        
        // mutex unlock for queue

        //TODO :  send to server
    }


}



