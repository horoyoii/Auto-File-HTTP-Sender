#include"LocalMonitor.h"



LocalMonitor::LocalMonitor(std::string path, std::queue<std::string>* eq, Semaphore* sem, Mutex* mtx){
    this->path = path;
    this->event_queue = eq;
    this->sem = sem;
    this->mtx = mtx;
    
}

int LocalMonitor::init(){
    fd = inotify_init();

    if(fd < 0)
        setErrorTypeAndReturn();

    printf("watch -> %s\n", path.c_str());

    /**
    * the events to be monitored for path are specified in the bit-mask arguments.
    */
    wd = inotify_add_watch(fd, path.c_str(), IN_CREATE | IN_MODIFY | IN_MOVE);

    if(wd == -1){
        fprintf(stderr, "Failed to add watch [%s] [%s]\n", path.c_str(), strerror(errno));
        
        setErrorTypeAndReturn();
    }
    return 1;
}




void LocalMonitor::run(){
    char buffer[BUF_LEN];

    while(1){
       
        int length, i =0;
        length = read(fd, buffer, BUF_LEN); // **blocking unitl at least one event occurs

        while(i < length){
            struct inotify_event *event = (struct inotify_event*)&buffer[i];
            handleEvent(event);

            i += EVENT_SIZE + event->len;
        }

    }
}

/**
*
* * .swp file will not be handled. just ignore...
*
* which bit mask is set  
* IN_MOVE : mv to target or ** move target dir to another
* IN_MOVED_FROM : a file in target directory was moved to another 
* IN_MOVED_TO : a file in another directory was moved to target directory
*/
void LocalMonitor::handleEvent(struct inotify_event* event){

    if(event->mask & IN_CREATE | IN_MOVE | IN_MODIFY){
        if(event->mask & IN_ISDIR)
            printf("dir %s was created\n", event->name);
        else if(event->mask & IN_CREATE)
            printf("file %s was created\n", event->name);
        else if(event->mask & IN_MODIFY)
            printf("file %s was modified\n", event->name);
        else if(event->mask & IN_MOVED_FROM)
            printf("file %s was moved from\n", event->name);
        else if(event->mask & IN_MOVED_TO)
            printf("file %s was moved to\n", event->name);

      
        if(event->mask & (IN_CREATE | IN_MOVED_TO | IN_MODIFY)){
            
            // filter the .swp .swx
            std::string e_name(event->name);
            
            mtx->Lock();
            event_queue->push(event->name);
            mtx->UnLock();

            sem->notify();
        }

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


