//
// Created by william on 2021/7/17.
//

#include <thread>
#include <iostream>
#include <vector>

std::atomic<long> g_count = 0;
long g_count1 = 0;
std::mutex g_mutex;

void threadFnc()
{
    for (int i = 0; i < 100000; ++i)
    {
        g_count++;
    }
}

void threadFnc1()
{
    for (int i = 0; i < 100000; ++i)
    {
        g_count1++;
    }
}

void threadFnc2()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    for (int i = 0; i < 100000; ++i)
    {
        g_count1++;
    }
}

void atomicTest()
{
    std::vector<std::thread> threads;
    auto starTime = std::chrono::system_clock::now();
    threads.reserve(10);
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back(threadFnc);
    }
    for (int i = 0; i < 10; ++i)
    {
        threads[i].join();
    }
    auto endTime = std::chrono::system_clock::now();
    std::cout << "g_count:" << g_count << std::endl;
    std::cout << "g_count1:" << g_count1 << std::endl;
    std::cout << "during Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - starTime).count() << std::endl;
}