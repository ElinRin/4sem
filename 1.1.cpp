#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <random>
#include <mutex>

const int N = 1e8;
const int numberThread = 20;



void sum(int l, int r, double &answer, std::vector<double> & data, std::mutex& m)
{
	double temp = 0;
	for(int i = l; i < r; ++i){
		//temp += data[i];
		temp += sin(data[i]);
	}
	m.lock();
	answer += temp;
	m.unlock();

}

double check(int num, double &result, std::vector<double> & data)
{
    std::mutex mtx;

	auto begin_time = std::chrono::system_clock::now();

	std::vector<std::thread> threads(num);

	double vector_sum = 0;
	int current_boarder = 0;
	for(int i = 0; i < num; ++i)
    {
		threads[i] = std::thread(sum, current_boarder, i == num - 1 ? N : current_boarder + N / num,
                           std::ref(vector_sum), std::ref(data), std::ref(mtx));
		current_boarder += N / num;
	}

	for(int i = 0; i < num; ++i) { threads[i].join(); }

	auto end_time = std::chrono::system_clock::now();

	result = vector_sum;
	return std::chrono::duration<double>(end_time - begin_time).count();
}


int main(){
    std::vector<double> data(N);
    double ans = 0;

    for(int i = 0; i < N; ++i) { data[i] = rand(); }

	for(int i = 1; i <= numberThread; ++i)
    {
		std::cout << i << ": " << ans << ", " << check(i, ans, data) << "s " << std::endl;
	}

	return 0;
}
