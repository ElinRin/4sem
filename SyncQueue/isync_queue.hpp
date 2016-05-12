#ifndef ISYNC_QUEUE_HPP
#define ISYNC_QUEUE_HPP

//#include "futex.hpp"
#include <condition_variable>
#include <queue>

template<class T>
class ISyncQueue {
public:
    ISyncQueue() {};
    std::queue<T> operator=( const std::queue<T>& other );
    virtual T front() {};
    virtual T back() {};
    virtual bool empty(){};
    virtual size_t size(){};
    virtual void push(T element){};
    virtual T pop(){};
    virtual ~ISyncQueue() {};


};

#endif
