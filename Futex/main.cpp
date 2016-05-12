#include "futex.hpp"
#include <vector>
#include <string>
#include <mutex>
#include <iostream>


template<class MFutex>
void inc(int & value, int & thread_value, int &required_value, MFutex & mf)
{
    while (1)
    {
        std::lock_guard<MFutex> lg(mf);
        if(value < required_value) {
        (value)++;
        (thread_value)++;
        }
            else
            break;
       // mf.unlock();
    }
}

template <class MFutex>
std::vector<int> thread_inc(int required_value, int count_threads )
{
    MFutex mf;
    int value = 0;
    std::vector<std::thread> thr(count_threads);
    std::vector<int> value_thr(count_threads , 0);
    for(int i = 0 ;  i < count_threads ; ++i) {
        thr[i] = std::thread(inc<MFutex> , std::ref(value)  , std::ref(value_thr[i]), std::ref(required_value), std::ref(mf));
    }

    for(int i = 0 ;  i < count_threads ; ++i) {
        thr[i].join();
    }

    int sum = 0;
    for(int i = 0 ;  i < count_threads ; ++i) {
        sum += value_thr[i];
    }

    assert(value == required_value && value == sum);

    return value_thr;

}
template <class MFutex>
void task(std::string task_number, int count_threads, int required_value )
{

    std::vector<int> value_thr;

    std::cout << "Task " << task_number << std::endl;
    auto begin_time = std::chrono::system_clock::now();
    value_thr = thread_inc<MFutex> (required_value, count_threads);
    auto end_time = std::chrono::system_clock::now();

    for(int i = 0 ;  i < count_threads ; ++i) {
        std::cout << "value in thread_" << i << ": " << value_thr[i] << std::endl;
    }
    std::cout << "time: " << std::chrono::duration<double>(end_time - begin_time).count() << "s" << std::endl;
}
int main()
{
    int count_threads;
    int required_value;

    //task<Futex>("A", 10, 10);
    task<Futex>("B", 10, 35*1e5);
    task<Futex>("C", 10, 20*1e6);
    task<Futex>((std::string)"D.a Futex",std::thread::hardware_concurrency() / 2, 10e5);
    task<std::mutex>("D.a Mutex", std::thread::hardware_concurrency() / 2, 10e5);
    task<Futex>("D.b Futex", std::thread::hardware_concurrency() * 2, 10e5);
    task<std::mutex>("D.b Mutex", std::thread::hardware_concurrency() * 2, 10e5);

    return 0;
}
