//
// Created by cwb on 2021/12/30.
//

#include <iostream> // cout
#include <memory>   // shared_ptr
#include <thread>   // sleep_for/lock_guard/mutex

class Test1
{
public:
    Test1()
    {
        std::cout << "Test1 called" << std::endl;
        m_checkIsLife = std::make_shared<int>();
    }

    ~Test1()
    {
        std::weak_ptr<int> isLife = m_checkIsLife;
        m_checkIsLife = nullptr;
        int n = 0;
        while (!isLife.expired())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (++n > 1000)
            {
                break;
            }
        }
        std::cout << "~Test1 called" << std::endl;
    }

    void func()
    {
        std::lock_guard<std::mutex> lck(m_effectLock);
        std::cout << "func called" << std::endl;
    }

    std::shared_ptr<int>& isLive() { return m_checkIsLife; }

public:
    std::shared_ptr<int> m_checkIsLife;
    std::mutex m_effectLock;
};

class Foo1
{
public:
    Foo1()
    {
        std::cout << "Foo1 called" << std::endl;
        m_t = std::make_shared<Test1>();
    }

    ~Foo1()
    {
        std::cout << "~Foo1 called" << std::endl;
    }

    std::shared_ptr<Test1> m_t;
};

void asyncTestDemo()
{
    std::shared_ptr<Foo1> f = std::make_shared<Foo1>();
    const std::shared_ptr<Test1>& t = f->m_t;
    std::cout << "t.use_count:" << t.use_count() << "--------" << t.get() << std::endl;
    t->func();
    f = nullptr;
    std::cout << "t.use_count:" << t.use_count() << "--------" << t.get() << std::endl;
    std::weak_ptr<int> isLivePtr = std::weak_ptr<int>(t->isLive());
    auto isLive = isLivePtr.lock();
    /// 此时会崩溃
    if (t)
    {
        t->func();
    }
    /// 正确的写法是
    //    if(isLive)
    //    {
    //        t->func();
    //    }
}