#include "futex.hpp"
#include <vector>
#include <string>
#include <mutex>
#include <iostream>

const int THREADS = 10;
volatile int value_thr[THREADS + 1];
int value = 0;

template<class Futex>
void inc(int n, int &required_value, Futex & mf)
{

    while (1)
    {
        std::lock_guard<Futex> lg(mf);
        if(value < required_value) {
        (value)++;
        (value_thr[n])++;
        //std::cout << value_thr[n] << '!' << n <<'\n';
        }
            else
            break;
    }

}

template<class Futex>
void thread_inc(int required_value )
{
    Futex mf;
    std::vector<std::thread> thr(THREADS);
    for(int i = 0 ;  i < THREADS ; ++i) {
        thr[i] = std::thread(inc<Futex> , i, std::ref(required_value), std::ref(mf));
    }

    for(int i = 0 ;  i < THREADS ; ++i) {
        thr[i].join();
    }

    int sum = 0;
    for(int i = 0 ;  i < THREADS ; ++i) {
        sum += value_thr[i];

        //std::cout << value_thr[i] << '!' << i <<'\n';
    }

    assert(value == required_value && value == sum);

    //std::cout << value << '!' << required_value <<'!' << sum << std::endl;

}

template<class Futex>
void task(std::string task_number, int required_value )
{

    std::cout << "Task " << task_number << std::endl;
    auto begin_time = std::chrono::system_clock::now();
    thread_inc<Futex> (required_value);
    auto end_time = std::chrono::system_clock::now();

    for(int i = 0 ;  i < THREADS ; ++i) {
        std::cout << "value in thread_" << i << ": " << value_thr[i] << std::endl;
    }
    std::cout << "time: " << std::chrono::duration<double>(end_time - begin_time).count() << "s" << std::endl;
}
int main()
{
    int required_value;

    for (int i = 0; i <= THREADS; i++) {
        value_thr[i] = 0;
    }

    task<Futex>("volatile", 35*1e5);

    return 0;
}
