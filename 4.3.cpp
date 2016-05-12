#include <iostream>
#include <thread>
#include <cstdio>
#include <chrono>
#include <atomic>

const int n = 1e5;
//Âûðîâíåííûå è íåâûðîâíåííûå atomic ïåðåìåííûå.
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

//Atomic, öåëèêîì ëåæàùèé â êýø - ëèíèè è atomic, ïåðåñåêàþùèõ èõ ãðàíèöó.
struct InOneCacheLine
{
	char a;
	int b;
	InOneCacheLine() noexcept : a(0), b(0) {}
};

struct InTwoCacheLine
{
	char a; //1 áàéò
	int d[15]; // 60 áàéòà
	//3 ïóñòûõ áàéòà
	int b; // 4 áàéò
	InTwoCacheLine() noexcept : a(0), b(0) {}
};

//Äâå atomic ïåðåìåííûå, ëåæàùèå â îäíîé êýø - ëèíèè è â ðàçíûõ êýø - ëèíèÿõ.
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
	//Âûðîâíåííûå è íåâûðîâíåííûå atomic ïåðåìåííûå.
	std::atomic<struct Align> align;
	std::atomic<struct  NoAlign> noAlign;
	//Atomic, öåëèêîì ëåæàùèé â êýø - ëèíèè è atomic, ïåðåñåêàþùèõ èõ ãðàíèöó.
	std::atomic<struct InOneCacheLine> oneCache;
	std::atomic<struct InTwoCacheLine> twoCache;
	//Äâå atomic ïåðåìåííûå, ëåæàùèå â îäíîé êýø - ëèíèè è â ðàçíûõ êýø - ëèíèÿõ.
	std::atomic<struct InOneCacheLineAtomic> oneCacheA;
	std::atomic<struct InTwoCacheLineAtomic> twoCacheA;


	checkTime<struct Align>("Align atomic object.");
	checkTime<struct NoAlign>("Non align atomic object.");
	checkTime<struct InOneCacheLine>("One atomic object in one cache line.");
	checkTime<struct InTwoCacheLine>("One atomic object in two cache line.");
	checkTime2<struct InOneCacheLineAtomic>("Two atomic object in one cache line.");
	checkTime2<struct InTwoCacheLineAtomic>("Two atomic object in two cache line.");


	//system("pause");
	return 0;
}
