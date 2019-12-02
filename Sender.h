#ifndef SENDER
#define SENDER

#include<iostream>
#include<string>
#include<unistd.h>
#include<queue>
#include"Semaphore.h"
#include"Mutex.h"
#include"HttpClient.h"

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
    std::queue<std::string>*    event_queue;
    Semaphore*                  sem;
    Mutex*                      mtx;
    HttpClient*                 httpClient;
    
    std::string                 dir_path;
public:
    Sender(std::string server_ip, std::string path, std::string port, std::string dir_path, std::queue<std::string>* eq, Semaphore* sem, Mutex* mtx);

    void run();

};


#endif
