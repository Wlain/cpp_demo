//
// Created by william on 2021/2/15.
//
#include <iostream>
#include <memory>
#include <thread>

using namespace std;

struct Base
{
    Base() { cout << "  Base::Base()\n"
                  << endl; }
    // 注意：此处非虚析构函数 OK
    ~Base() { cout << "  Base::~Base()\n"
                   << endl; }
};

struct Derived : public Base
{
    Derived() { cout << "  Derived::Derived()\n"
                     << endl; }
    ~Derived() { cout << "  Derived::~Derived()\n"
                      << endl; }
};

void thr(shared_ptr<Base> p)
{
    this_thread::sleep_for(chrono::seconds(1));
    shared_ptr<Base> lp = p; // 线程安全，虽然自增共享的 use_count
    {
        static mutex ioMutex;
        lock_guard<mutex> lk(ioMutex);
        cout << "local pointer in a thread:\n"
             << "  lp.get() = " << lp.get()
             << ", lp.use_count() = " << lp.use_count() << '\n';
    }
}

void sharePtrTest()
{
    shared_ptr<Base> p = make_shared<Derived>();
    std::cout << "Created a shared Derived (as a pointer to Base)\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';

    std::thread t1(thr, p), t2(thr, p), t3(thr, p);
    p.reset(); // 从 main 释放所有权
    std::cout << "Shared ownership between 3 threads and released\n"
              << "ownership from main:\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';
    t1.join(); t2.join(); t3.join();
    std::cout << "All threads completed, the last one deleted Derived\n";
}