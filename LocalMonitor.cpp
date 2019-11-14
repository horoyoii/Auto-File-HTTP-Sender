#include"LocalMonitor.h"



LocalMonitor::LocalMonitor(std::string path, std::queue<std::string>* eq, Semaphore* sem){
    this->path = path;
    this->event_queue = eq;
    this->sem = sem;
    //this->mtx_q = mtx;
}

int LocalMonitor::init(){
    fd = inotify_init();

    if(fd < 0)
        setErrorTypeAndReturn();

    printf("watch -> %s\n", path.c_str());

    /**
    * the events to be monitored for path are specified in the bit-mask arguments.
    */
    wd = inotify_add_watch(fd, path.c_str(), IN_CREATE | IN_MODIFY | IN_DELETE);

    if(wd == -1)
        setErrorTypeAndReturn();

    return 1;
}




void LocalMonitor::run(){
    char buffer[BUF_LEN];

    while(1){
        printf("watching...\n");

        int length, i =0;
        length = read(fd, buffer, BUF_LEN); // **blocking unitl at least one event occurs

        while(i < length){
            struct inotify_event *event = (struct inotify_event*)&buffer[i];
            handleEvent(event);


            i += EVENT_SIZE + event->len;
        }

    }


}//end run()

/**
*
*
* * .swp file will not be handled. just ignore...
*/
void LocalMonitor::handleEvent(struct inotify_event* event){

    if(event->mask & IN_CREATE){
        if(event->mask & IN_ISDIR)
            printf("dir %s was created\n", event->name);
        else
            printf("file %s was created\n", event->name);

        //TODO : mutex lock for queue

        event_queue->push(event->name);

        sem->notify();

        // mutex unlock for queue
    }


}


int LocalMonitor::setErrorTypeAndReturn(){
    this->error_type = errno;
    return -1;
}

int LocalMonitor::getErrorType(){
    return this->error_type;
}



const int LocalMonitor::EVENT_SIZE  = (sizeof(struct inotify_event)) ;
const int LocalMonitor::BUF_LEN     = (1024 * (EVENT_SIZE + 16) );


