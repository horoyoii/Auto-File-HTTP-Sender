
template<typename T>
SafeQueue::SafeQueue(void)
    :q(), m(){}

template<typename T>
void SafeQueue::enqueue(T t){
    std::lock_guard<std::mutex> lock(m);

    q.push(t);
}


template<typename T>
T SafeQueue::dequeue(void){
    std::lock_guard<std::mutex> lock(m);

    T res = q.front();
    q.top();

    return res;
}


