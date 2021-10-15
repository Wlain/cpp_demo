//
// Created by william on 2021/7/17.
//

#include "base.h"

// lock_guard是mutex包装器，用于在作用域块的持续时间内拥有互斥体。当一个lock_guard对象被创建时，
// 它试图获得它所给的互斥锁的所有权。当控制离开lock_guard创建对象的作用域时，对象被lock_guard破坏并释放互斥锁。

int g_i;
std::mutex g_i_mutex;

void saveIncrement()
{
    const std::lock_guard<std::mutex> lock(g_i_mutex);
    ++g_i;
    std::cout << "g_i:" << g_i << " in this thread# " << std::this_thread::get_id() << std::endl;
}
void lockGuardTest()
{
    std::cout << "g_i: " << g_i << "; in main()\n";
    std::thread t1(saveIncrement);
    std::thread t2(saveIncrement);
    t1.join();
    t2.join();
    std::cout << "g_i: " << g_i << "; in main()\n";
}