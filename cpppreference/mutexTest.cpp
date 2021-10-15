//
// Created by william on 2021/7/17.
//

#include "base.h"

// 用来加锁
// mutex类是一个同步原语，可以用来保护共享数据不被多个线程同时访问。
// mutex提供独占的、非递归的所有权语义。
// 一个调用的线程拥有一个mutex，从它成功调用lock或try_lock直到它调用unlock。
// 当一个线程拥有一个mutex时，所有其他线程如果试图要求该mutex的所有权，将阻塞（对于调用lock）或收到一个错误的返回值（对于try_lock）。
// 调用线程在调用lock或try_lock之前必须不拥有mutex。
// 如果一个mutex在被任何线程拥有时被销毁，或者一个线程在拥有一个mutex时终止，那么程序的行为将无法定义。mutex类满足了Mutex和StandardLayoutType的所有要求。
namespace mutexTest
{
// lock:locks the mutex, blocks if the mutex is not available
int gNum = 0; // 受 gNumMutex保护
std::mutex gNumMutex;
void slowIncrement(int id)
{
    for (int i = 0; i < 3; ++i)
    {
        gNumMutex.lock();
        ++gNum;
        std::cout << id << " => " << gNum << '\n';
        gNumMutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void lockTest()
{
    std::thread t1(slowIncrement, 0);
    std::thread t2(slowIncrement, 1);
    t1.join();
    t2.join();
}

// try_lock: tries to lock the mutex, returns if the mutex is not available
std::chrono::milliseconds interval(100);
std::mutex mutex;
int jobShared = 0;
int jobExclusive = 0;
void job1()
{
    std::this_thread::sleep_for(interval); // let 'job_2' take a lock
    while (true)
    {
        if (mutex.try_lock())
        {
            std::cout << "job shared(" << jobShared << ")\n";
            mutex.unlock();
            return;
        }
        else
        {
            // can't get lock to modify 'job_shared'
            // but there is some other work to do
            ++jobExclusive;
            std::cout << "job exclusive(" << jobExclusive << "）\n";
            std::this_thread::sleep_for(interval);
        }
    }
}

void job2()
{
    mutex.lock();
    std::this_thread::sleep_for(5 * interval);
    ++jobShared;
    mutex.unlock();
}

void tryLockTest()
{
    std::thread thread_1(job1);
    std::thread thread_2(job2);

    thread_1.join();
    thread_2.join();
}

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;

void save_page(const std::string& url)
{
    // simulate a long page fetch
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::string result = "fake content";

    std::lock_guard<std::mutex> guard(g_pages_mutex);
    g_pages[url] = result;
}

void mutexTest()
{
    std::thread t1(save_page, "http://foo");
    std::thread t2(save_page, "http://bar");
    t1.join();
    t2.join();

    // safe to access g_pages without lock now, as the threads are joined
    for (const auto& pair : g_pages)
    {
        std::cout << pair.first << " => " << pair.second << std::endl;
    }
}

} // namespace mutexTest
