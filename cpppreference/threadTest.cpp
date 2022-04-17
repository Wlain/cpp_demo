//
// Created by william on 2021/7/17.
//

#include "base.h"
#include "commonMacro.h"
namespace threadTest
{
/// 此demo用于解决在多线程下，因为悬垂指针导致的崩溃问题
class ThreadTestCheck
{
public:
    ~ThreadTestCheck()
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
    }
    std::shared_ptr<int>& isLive() { return m_checkIsLife; }

private:
    std::shared_ptr<int> m_checkIsLife = std::make_shared<int>();
};

struct ThreadTest : public ThreadTestCheck
{
    // operator=, returns a reference to this
    ~ThreadTest()
    {
        std::cout << "~ThreadTest called" << std::endl;
    }
    std::string& str()
    {
        return m_str;
    }
    void setStr(std::string s)
    {
        m_str = std::move(s);
    }

private:
    std::string m_str = "null";
};

void threadTest()
{
    auto* test = new ThreadTest();
    auto str = test->str();
    LOG_INFO("%s", str.c_str());
    delete test;
    std::weak_ptr<int> isLivePtr = std::weak_ptr<int>(test->isLive());
    auto isLive = isLivePtr.lock();
    if (isLive)
    {
        LOG_INFO("is live");
    }
    else
    {
        LOG_ERROR("is not live");
    }
    /// 此时，test是一个悬垂指针，使用这个指针可能会崩溃也可能不会崩溃
    test->setStr("after");
    LOG_ERROR(test->str().c_str());
}
} // namespace threadTest
