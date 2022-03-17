//
// Created by cwb on 2022/3/16.
//

#include <chrono>
#include <iostream>
#include <map>
#include <thread>

bool running = true;

std::string convertVersion(int version)
{
    /// bad
//    const static std::map<int, std::string> versionMap{
//        { 1, "123456" },
//        { 2, "abcdef" },
//        { 3, "+-*/()" },
//    };
    /// good 由于versionMap被声明为引用类型了，C++不会再为对变量进行析构了。
    const static auto& versionMap = *new std::map<int, std::string> {
        {1, "123456"},
        {2, "abcdef"},
        {3, "+-*/()"},
    };

    const auto& it = versionMap.find(version);
    if (it != versionMap.end())
    {
        return it->second;
    }

    return "";
}

void test()
{
    while (running)
    {
        convertVersion(2);
    }
}


void staticExternThreadCrashDemo()
{
    /// 版本一：不崩溃
    //    std::thread t[100];
    //    for (int i = 0; i < 100; i++)
    //    {
    //        t[i] = std::thread(test);
    //    }
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //    std::cout << "exit\n";
    //    running = false;
    //    for (int i = 0; i < 100; i++)
    //    {
    //        t[i].join();
    //    }
    /// 分析一下是由于最后我们通过join函数（join函数会阻塞当前线程，直到子线程结束运行）确保子线程在主函数退出之前已经结束，从而确保在主线程析构静态变量的时候，子线程不可能会再访问静态变量了。
    /// 版本二：崩溃
    std::thread t[100];
    for (int i = 0; i < 100; i++)
    {
        t[i] = std::thread(test);
        t[i].detach();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "exit\n";
    running = false;
    /// 这次我们去掉线程的join函数，运行果然崩溃了。

    /// 为了修复这个问题，一种方法是在主线程退出前加上join函数确保子线程正确退出，另一种则是修改静态变量，避免静态变量在退出时析构。
    /// chromium的文档中给出一个比较好的静态变量创建方法：https://chromium.googlesource.com/chromium/src/+/HEAD/styleguide/c++/c++-dos-and-donts.md#static-variables
}