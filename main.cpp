#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>



static int counter = 0;

struct Spinlock {
    std::atomic<bool> lock_ = { false }; // є1

    void lock() { while (lock_.exchange(true)); } // є2

    void unlock() { lock_.store(false); } // є3
};

Spinlock mutex1;
std::mutex mutex;

void add()
{
    for (int i = 0; i < 100000000; i++)
    {
        mutex.lock();
       // std::this_thread::sleep_for(std::chrono::seconds(1));
        counter++;
        mutex.unlock();
    }
}

void subtraction()
{
    for (int i = 0; i < 100000000; i++)
    {
        mutex.lock();
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        counter--;
        mutex.unlock();
    }
}

void add1()
{
    for (int i = 0; i < 100000000; i++)
    {
        mutex1.lock();
        //std::this_thread::sleep_for(std::chrono::seconds(3));
        counter++;
        mutex1.unlock();
    }
}

void subtraction1()
{
    for (int i = 0; i < 100000000; i++)
    {
        mutex1.lock();
        //std::this_thread::sleep_for(std::chrono::seconds(3));
        counter--;
        mutex1.unlock();
    }
}

int main()
{
    
    auto start = std::chrono::high_resolution_clock::now();
    std::thread th1(add);
    std::thread th2(subtraction);
    th1.join();
    th2.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << counter << std::endl;
    std::chrono::duration<float> duration = end - start;     
    std::cout << "Duration mutex: " << duration.count() << std::endl;

    auto start1 = std::chrono::high_resolution_clock::now();
    std::thread th3(add1);
    std::thread th4(subtraction1);
    th3.join();
    th4.join();
    auto end1 = std::chrono::high_resolution_clock::now();
    std::cout << counter << std::endl;
    std::chrono::duration<float> duration1 = end1 - start1;
    std::cout << "Duration spinlock: " << duration1.count() << std::endl;



    return 0;
}