#include"AutoFileSender.h"

AutoFileSender::AutoFileSender(string server_ip, string path, string port, string dir_path)
    :dir_path(dir_path){

    event_queue = new queue<string>();
    sem = new Semaphore();
    mtx = new Mutex();

    localMonitor = new LocalMonitor(dir_path, event_queue, sem, mtx);
    sender = new Sender(server_ip, path, port, dir_path, event_queue, sem, mtx);

}

void AutoFileSender::run(){

    localMonitor->init();

    thread monitoring_flow(&LocalMonitor::run, localMonitor);
    thread sending_flow(&Sender::run, sender);

    monitoring_flow.join();
    sending_flow.join();

}
