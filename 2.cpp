
#include <iostream>
#include <thread>
#include <cstdio>
#include <chrono>


struct pingpong {
    long long ping;
    long long pong;
    pingpong() : ping(0), pong(0) {}
};

pingpong pp;

void dosmth(long long &x) {
    for (int i = 0; i < 1e8; ++i) {
        x+=55639798664;
    }
}

struct without {
    long long ping; //8 байт
    int d[13]; // 52 байта
    long long pong; // 8 байт
    without() : ping(0), pong(0){}
};

without w;

int main() {
    std::thread t1, t2;
    auto begin_time = std::chrono::system_clock::now();
    t1 = std::thread(dosmth, std::ref(pp.ping));
    t2 = std::thread(dosmth, std::ref(pp.pong));
    t1.join();
    t2.join();
    auto end_time = std::chrono::system_clock::now();
    std::cout << pp.ping << ' ' << pp.pong << std::endl;
    std::cout << "time pingpong: " << std::chrono::duration<double>(end_time - begin_time).count() << "s" << std::endl;

    begin_time = std::chrono::system_clock::now();
    t1 = std::thread(dosmth, std::ref(w.ping));
    t2 = std::thread(dosmth, std::ref(w.pong));
    t1.join();
    t2.join();
    end_time = std::chrono::system_clock::now();
    std::cout << w.ping << ' ' << w.pong << std::endl;
    std::cout << "time with distance: " << std::chrono::duration<double>(end_time - begin_time).count() << "s" << std::endl;

}
