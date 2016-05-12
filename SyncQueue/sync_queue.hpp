#ifndef SYNC_QUEUE_HPP
#define SYNC_QUEUE_HPP

#include "isync_queue.hpp"

//class ISyncQueue;
using std::mutex;

template<class T>
class SyncQueue :  public ISyncQueue<T> {
public:
    //SyncQueue() {};

    //~SyncQueue() {};
std::queue<T> operator=( const std::queue<T>& other ) {
    std::lock_guard<mutex> lg(block);
    container=other;
}

T front() {
    std::unique_lock<mutex> ul(block);
		if (container.empty()) {
			signal.wait(ul);
		}
		return container.front();
}

T back() {
     std::unique_lock<mutex> ul(block);
		if (container.empty()) {
			signal.wait(ul);
		}
		return container.back();
}

bool empty() const {
    return container.empty();
}

size_t size() const {
    return container.size();
}

void push(T element) {
    std::lock_guard<mutex> lg(block);
    container.push(element);
}

T pop() {
        std::unique_lock<std::mutex> ul(block);
		if (container.empty()) {
			signal.wait(ul);
		}
		T elem = container.front();
		container.pop();
		return elem;
}

private:
    std::condition_variable signal;
	mutex block;
	std::queue<T> container;
};

#endif
