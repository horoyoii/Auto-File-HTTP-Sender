#ifndef AUTOFILESENDER
#define AUTOFILESENDER

#include<thread>
#include<queue>
#include<mutex>

#include"LocalMonitor.h"
#include"Sender.h"
#include"Semaphore.h"
#include"SafeQueue.h"

using namespace std;


class AutoFileSender{
private:
    LocalMonitor*       localMonitor;
    Sender*             sender;
    SafeQueue<string>*  event_queue;
    Semaphore*          sem;
    string              dir_path;



public:
    AutoFileSender(string server_ip, string path, string port, string dir_path);

    void run();

};


#endif
