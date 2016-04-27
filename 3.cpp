#include <iostream>
#include <thread>
#include <cstdio>
#include <chrono>
#include <atomic>

const int n = 1e5;
//Выровненные и невыровненные atomic переменные.
#pragma pack(push, 1)
struct Align
{
	char a;
	int b;
	Align() noexcept : a(0), b(0) {}
	Align & operator = (Align const & n)
	{
		a = n.a;
		b = n.b;
		return *this;
	}

};
#pragma pack(pop)

struct NoAlign
{
	char a;
	int b;
	NoAlign() noexcept : a(0), b(0) {}
	NoAlign & operator = (NoAlign const & n)
	{
		a = n.a;
		b = n.b;
		return *this;
	}
};

//Atomic, целиком лежащий в кэш - линии и atomic, пересекающих их границу.
struct InOneCacheLine
{
	char a;
	int b;
	InOneCacheLine() noexcept : a(0), b(0) {}
};

struct InTwoCacheLine
{
	char a; //1 байт
	int d[15]; // 60 байта
	//3 пустых байта
	int b; // 8 байт
	InTwoCacheLine() noexcept : a(0), b(0) {}
};

//Две atomic переменные, лежащие в одной кэш - линии и в разных кэш - линиях.
struct InOneCacheLineAtomic
{
	std::atomic<char> a;
	std::atomic<int> b;
	InOneCacheLineAtomic() noexcept : a(0), b(0) {}
};

struct InTwoCacheLineAtomic
{
	std::atomic<char> a;
	int d[15];
	std::atomic<int> b;
	InTwoCacheLineAtomic() noexcept: a(0), b(0) {}
};



template<typename T>
void checkTime(std::string type)
{
	std::cout << type << " size:" << sizeof(T) << std::endl;
	auto begin_time = std::chrono::system_clock::now();
	std::atomic<T> in;
    T data;
	data.a = 'h';
	data.b = 567897948;

	for (int i = 0; i < n; ++i)
	{
		in.store(data);
	}
	T out = in;
	//std::cout << out.a << ' ' << out.b << std::endl;
	auto end_time = std::chrono::system_clock::now();
	std::cout << "time: " << std::chrono::duration<double>(end_time - begin_time).count() << "s" << std::endl;
}

template<typename T>
void checkTime2(std::string type)
{
	std::cout << type << " size:" << sizeof(T) << std::endl;
	auto begin_time = std::chrono::system_clock::now();
    T in;

	for (int i = 0; i < n; ++i)
	{
		in.a.store('h');
		in.b.store(567897948);
	}

	char outA = in.a;
	int outB = in.b;
	//std::cout << outA << ' ' << outB << std::endl;
	auto end_time = std::chrono::system_clock::now();
	std::cout << "time: " << std::chrono::duration<double>(end_time - begin_time).count() << "s" << std::endl;
}

int main() {
	//Выровненные и невыровненные atomic переменные.
	std::atomic<struct Align> align;
	std::atomic<struct  NoAlign> noAlign;
	//Atomic, целиком лежащий в кэш - линии и atomic, пересекающих их границу.
	std::atomic<struct InOneCacheLine> oneCache;
	std::atomic<struct InTwoCacheLine> twoCache;
	//Две atomic переменные, лежащие в одной кэш - линии и в разных кэш - линиях.
	std::atomic<struct InOneCacheLineAtomic> oneCacheA;
	std::atomic<struct InTwoCacheLineAtomic> twoCacheA;


	checkTime<struct Align>("Align Atomic Object");
	checkTime<struct NoAlign>("Non Align Atomic Object");
	checkTime<struct InOneCacheLine>("One Atomic Object In One Cache Line");
	checkTime<struct InTwoCacheLine>("One Atomic Object In Two Cache Line");
	checkTime2<struct InOneCacheLineAtomic>("Two Atomic Object In One Cache Line");
	checkTime2<struct InTwoCacheLineAtomic>("Two Atomic Object In Two Cache Line");


	//system("pause");
	return 0;
}
