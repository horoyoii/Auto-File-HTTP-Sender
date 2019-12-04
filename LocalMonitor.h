#ifndef LOCALMONITOR
#define LOCALMONITOR

#include<sys/inotify.h>
#include<errno.h>
#include<string>
#include<unistd.h>
#include<mutex>

#include"Semaphore.h"
#include"SafeQueue.h"

#include<cstring>

/**
* Created by Horoyoii on 2019.11.12..
*/

/**
* LocalMonitor monitors the target directory with inofity API.
* Events detected from this module may be file creation, modification etc...
* When event occurs, event will be inserted into the event_queue and  
* 'Sender' will get the event from the queue and send it to the remote server. 
*/
class LocalMonitor{
private:
    static const int EVENT_SIZE;
    static const int BUF_LEN;
    
    std::string     path; // target directory to watch   
    int             fd;
    int             wd;
    int             error_type;

    SafeQueue<std::string>*     event_queue;
    Semaphore*                  sem;
public:
    LocalMonitor(std::string path, SafeQueue<std::string>* eq, Semaphore* sem);

    int init();
    void run();

    /**
    *  .swp file will not be handled. just ignore...
    */
    void handleEvent(struct inotify_event* event);

    int setErrorTypeAndReturn();
    int getErrorType();
};


#endif
