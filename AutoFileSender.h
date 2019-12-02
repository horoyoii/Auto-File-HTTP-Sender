#ifndef AUTOFILESENDER
#define AUTOFILESENDER

#include<thread>
#include<queue>
#include<mutex>

#include"LocalMonitor.h"
#include"Sender.h"
#include"Semaphore.h"
#include"Mutex.h"

using namespace std;


class AutoFileSender{
private:
    LocalMonitor*   localMonitor;
    Sender*         sender;
    queue<string>*  event_queue;
    Semaphore*      sem;
    Mutex*          mtx;            // mutex for event queue
    string          dir_path;



public:
    AutoFileSender(string server_ip, string path, string port, string dir_path);

    void run();

};


#endif
