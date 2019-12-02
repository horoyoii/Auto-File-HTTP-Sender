#include<sys/inotify.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>

/**
    File or direcotry monitoring code using the inofity API.
*/

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16) )


int main(void){
    int fd;
    int wd; /*watch desc  */
    const char* path = "/tmp2/";
    
    fd = inotify_init(); // add inotify instance
    if(fd < 0)
        perror("inotify_init() error");
    
    /**
     the events to be monitored for path are specified in the ""bit-mask arguments"".
    */
    wd = inotify_add_watch(fd, path, IN_CREATE | IN_MODIFY | IN_DELETE);

    if(wd == -1){
        if(errno == EACCES)
            printf("%s\n", "not permitted");
        perror("inotify_add_watch error");
    }

    /**
        when events occurs for the monitored, those events are made available
        to the applications as structured data that can be read from the inotify file descriptor
        using **read()** 

           struct inotify_event {
               int      wd;       // Watch descriptor 
               uint32_t mask;     // Mask describing event 
               uint32_t cookie;   // Unique cookie associating related
                                     events (for rename(2)) 
               uint32_t len;      // Size of name field 
               char     name[];   // Optional null-terminated name 
           };
        
        ** Each read() returns a buffer containing one or more of inotify_event struct.
    */
    char buffer[BUF_LEN];
    while(1){
        printf("watching...\n");
        int length, i =0;
        length = read(fd, buffer, BUF_LEN); // **blocking unitl at least one event occurs
        
        while(i < length){
            struct inotify_event *event = (struct inotify_event*)&buffer[i];
         
            if(event->mask & IN_CREATE){
                if(event->mask & IN_ISDIR)
                    printf("dir %s was created\n", event->name);
                else
                    printf("file %s was created\n", event->name);
            }
            
            i += EVENT_SIZE + event->len;
        }


        int num;
        scanf("%d", &num);
      

    }

    return 0;
}






