#include"AutoFileSender.h"

AutoFileSender::AutoFileSender(string server_ip, string path)
:server_ip(server_ip), path(path){

    event_queue = new queue<string>();
    sem = new Semaphore();

    localMonitor = new LocalMonitor(path, event_queue, sem);
    sender = new Sender(server_ip, event_queue, sem);

}

void AutoFileSender::run(){

    localMonitor->init();

    thread monitoring_flow(&LocalMonitor::run, localMonitor);
    thread sending_flow(&Sender::run, sender);

    monitoring_flow.join();
    sending_flow.join();

}
