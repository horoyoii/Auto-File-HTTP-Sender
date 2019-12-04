#ifndef SENDER
#define SENDER

#include<iostream>
#include<string>
#include<unistd.h>
#include<queue>
#include"Semaphore.h"
#include"HttpClient.h"
#include"SafeQueue.h"


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
    SafeQueue<std::string>*     event_queue;
    Semaphore*                  sem;
    HttpClient*                 httpClient;
    std::string                 dir_path;
public:
    Sender(std::string server_ip, std::string path, std::string port, std::string dir_path, SafeQueue<std::string>* eq, Semaphore* sem);

    void run();

};


#endif
