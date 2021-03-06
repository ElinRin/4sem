# 4.1
**Компиляция** g++ -std=c++11 -pthread
* **simple:** 9.83063s
* **volatile** 9.07654s
* **barrier** 8.37942s
*   barrier: Использование аргументов memory_order_acquire и memory_order_release необходимая гарантия, что пара тредов скоординированно меняют значение мьютекса. 
Дело в том, что процессоры со слабо-упорядоченной обработкой могут оптимизировать очередь запросов, так что инструкции будут выполнять в не правильном порядке.В момент, когда один поток только освободил мьютекс, а второй готов его заблокировать, CPU может переупорядочить очереди запросов.
Избежать этого можно, с помощью барьеров памяти, которые разделяют соседние инструкций и гарантируют, что они не поменяются местами. Вот для чего нужны аргументы memory_order_acquire и memory_order_release. Их будет достаточно, так как компилятор вставляет нужные инструкции, которые работают как барьеры памяти.

*   volatile:В случае с volatile доступ к переменной не будет оптимизироваться компилятором. Так как сам volatile ставит барьеры и запрещает любую оптимизацию.

# 4.2
**Компиляция** g++ -std=c++11 -pthread
* **time pingpong:** 1.88422s
* **time with distance:** 0.296807s

**Модель процессора** *A10-5750M*

**Размер L1 кэша:** 

* 2х64 (память инструкций)

* 4х16 (память данных)

**Размер кэш-линии** *64 байта*

# 4.3

**Компиляция** g++ -std=c++11 -latomic

**Вывод**
* Align atomic object. size:5

    time: 0.169093s
 
* Non align atomic object. size:8
 
    time: 0.004037s
 
* One atomic object in one cache line. size:8
 
    time: 0.004002s

* One atomic object in two cache line. size:68
 
    time: 0.334122s

* Two atomic object in one cache line. size:8
 
    time: 0.003998s

* Two atomic object in two cache line. size:68
 
     time: 0.003089s
