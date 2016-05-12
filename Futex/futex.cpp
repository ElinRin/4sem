#include "futex.hpp"
#include <iostream>
#include <mutex>

std::mutex io;
Futex::Futex() : id(0) {}

int Futex::get_thread_id()
{
    assert(sizeof(std::thread::id)==sizeof(int));
    auto id = std::this_thread::get_id();
    int* ptr=(int*) &id;
    return (*ptr);
}

void Futex::lock() {
        int x = get_thread_id();
        int y = 0;
        while (!id.compare_exchange_strong(y, x)) {
            y = 0;
            std::this_thread::yield();
        }
}

/*bool Futex::try_lock() {
    int x = get_thread_id();
    int y = 0;
    if(id.compare_exchange_weak(y, x)) {
        return true;
    }
    return false;
}*/

void Futex::unlock() {
    int x = get_thread_id();
    int y = 0;
    assert(id.compare_exchange_weak(x, y));

}

