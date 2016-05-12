#include <atomic>
#include <exception>
#include <thread>
#include <assert.h>

class Futex {
public:
    Futex();
    void lock();
    void unlock();
    //bool try_lock();
private:
    std::atomic<int> id;
    int get_thread_id();
};
