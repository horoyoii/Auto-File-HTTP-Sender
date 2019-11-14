#ifndef AUTOFILESENDER
#define AUTOFILESENDER

#include<thread>
#include<queue>
#include<mutex>

#include"LocalMonitor.h"
#include"Sender.h"
#include"Semaphore.h"


using namespace std;


class AutoFileSender{
private:
    LocalMonitor*   localMonitor;
    Sender*         sender;
    queue<string>*  event_queue;
    Semaphore*      sem;
    
    string          server_ip;
    string          path;



public:
    AutoFileSender(string server_ip, string path);

    void run();

};


#endif AUTOFILESENDER
