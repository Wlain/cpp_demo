//
// Created by william on 2022/4/22.
//
#include <functional>
#include <memory>

// 防止多线程控制崩溃
class CallBackClass
{
public:
    void setFunc(std::function<void()> f)
    {
        m_func = std::make_shared<std::function<void()>>(std::move(f));
    }

    void runFunc()
    {
        auto func = m_func;
        if (func)
        {
            (*func)();
        }
    }

private:
    /// shared_ptr是线程安全的
    std::shared_ptr<std::function<void()>> m_func;
};

void setCallback()
{
    CallBackClass test;
    test.setFunc([]() {});
    test.runFunc();
}
