#include"AutoFileSender.h"

AutoFileSender::AutoFileSender(string server_ip, string path, string port, string dir_path)
    :dir_path(dir_path){

    event_queue = new SafeQueue<string>();
    sem = new Semaphore();

    

    localMonitor = new LocalMonitor(dir_path, event_queue, sem);
    sender = new Sender(server_ip, path, port, dir_path, event_queue, sem);

}

void AutoFileSender::run(){

    localMonitor->init();

    thread monitoring_flow(&LocalMonitor::run, localMonitor);
    thread sending_flow(&Sender::run, sender);

    monitoring_flow.join();
    sending_flow.join();

}
