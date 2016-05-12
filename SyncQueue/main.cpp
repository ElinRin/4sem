#include "sync_queue.hpp"
#include <thread>
#include <iostream>



using namespace std;

template <class T>
void data_push(SyncQueue<T> &data)
{

	for (int i = 0; i < 10; i++)
	{

        data.push((2 * i));
		data.push((10 * i));
		data.pop();
	}

};

int main()
{
	SyncQueue<int> d;
	data_push(d);
	std::vector<std::thread> thr(3);
	for (int i = 0; i < 3; ++i) {
		thr[i] = std::thread(data_push<int>, std::ref(d));
	}
	for (int i = 0; i < 3; ++i) {
		thr[i].join();
	}
	for (int i = 0; i < 10; i++)
	{
		std::cout << d.front() << std::endl;
		d.pop();
	}
	//system("pause");
    return 0;
}
