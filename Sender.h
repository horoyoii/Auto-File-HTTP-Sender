#ifndef SENDER
#define SENDER

#include<iostream>
#include<string>
#include<unistd.h>
#include<queue>
#include"Semaphore.h"

/**
* Created by Horoyoii on 2019.11.12..
*/

/**
* if Sender gets event of file creation or modification from queue, send it to the remote server.
* 
*
*/

class Sender{
private:
    std::string                 server_ip;      
    std::queue<std::string>*    event_queue;
    Semaphore*                  sem;


public:
    Sender(std::string server_ip, std::queue<std::string>* eq, Semaphore* sem);

    void run();

};


#endif
