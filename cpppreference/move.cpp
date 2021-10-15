//
// Created by william on 2021/3/28.
//

#include "base.h"

#include <utility>
#include <vector>

/// move的语义是经由移动构造函数创建的副本，本质上是强转，如果没有移动构造函数，还是会走拷贝构造函数。

class StringTest
{
public:
    StringTest()
    {
        std::cout << "constructor" << std::endl;
    }

    explicit StringTest(std::string s): m_s(std::move(s))
    {
        std::cout << "constructor" << std::endl;
    }

    ~StringTest()
    {
        std::cout << "distructor" << std::endl;
    }

    StringTest(const StringTest&)
    {
        std::cout << "copy constructor" << std::endl;
    }

    StringTest(const StringTest&&) noexcept
    {
        std::cout << "move constructor" << std::endl;
    }

private:
    [[maybe_unused]] std::string m_s;
};

[[maybe_unused]] void moveTest()
{
    std::string str = "hello";
    std::vector<std::string> v;
    v.push_back(str);
    // 使用push_back(const T&)重载
    // 表示我们将带来复制str的成本
    std::cout << "After copy, str is \"" << str << "\"\n";

    // 使用右值引用 push_back(T&&)重载
    // 表示不复制字符串，而是
    // str的内容被移动进vector
    // 这个开销可能比较低，但是也意味着str现在可能为空

    v.emplace_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
    std::cout << "The contents of the vector are \"" << v[0]
              << "\", \"" << v[1] << "\"\n";

    StringTest test;
    StringTest test2;
    std::vector<StringTest> v1;
//    std::cout << "========v1.push_back(test)=========" << std::endl;
//    v1.push_back(test);
//    std::cout << "========v1.emplace_back(test)=========" << std::endl;
//    v1.emplace_back(test);
//    std::cout << "========v1.push_back(std::move(test))=========" << std::endl;
//    v1.push_back(std::move(test));
//    std::cout << "========v1.emplace_back(test2)=========" << std::endl;
//    v1.emplace_back(test2);
//    std::cout << "========v1.emplace_back(std::move(test2))=========" << std::endl;
//    v1.emplace_back(std::move(test2));
//    std::cout << "========end=========" << std::endl;
}
