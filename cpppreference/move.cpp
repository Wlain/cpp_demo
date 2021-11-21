//
// Created by william on 2021/3/28.
//

#include "base.h"

#include <utility>
#include <vector>

/// 可作为面试题：可参考链接：https://segmentfault.com/q/1010000008984307
/// move的语义是经由移动构造函数创建的副本，本质上是强转，如果没有移动构造函数，还是会走拷贝构造函数。

class StringTest
{
public:
    StringTest()
    {
        std::cout << "constructor" << std::endl;
    }

    explicit StringTest(std::string s) :
        m_s(std::move(s))
    {
        std::cout << "constructor()" << std::endl;
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
    std::cout << "1========v1.push_back(test)=========" << std::endl;
    v1.push_back(test);
    std::cout << "1========v1.emplace_back(test)=========" << std::endl;
    v1.emplace_back(test);
    std::cout << "1========v1.push_back(std::move(test))" << std::endl;
    v1.push_back(std::move(test));
    std::cout << "1========v1.emplace_back(test2)=========" << std::endl;
    v1.emplace_back(test2);
    std::cout << "1========v1.emplace_back(std::move(test2))" << std::endl;
    v1.emplace_back(std::move(test2));
    std::cout << "1========v1.emplace_back(\"hello\")=========" << std::endl;
    v1.emplace_back("hello");
    /// 注意v1.reserve(10);这一句很关键，vector扩容，一开始容量是0，第一次操作扩容到1，第二次是翻倍为2，会造成拷贝
    /// 所以在你执行第一次push_back时，检查了容量不够，执行__push_back_slow_path函数，vector扩容到1，并且执行了一次移动构造函数，因为原来的vector是空的，所以不需要进一步处理。
    /// 此时第一次打印 copy constructor
    /// 在执行第二次emplace_back时，检查了容量不够，执行__push_back_slow_path函数，vector扩容到2，并且执行了一次拷贝构造函数。
    v1.reserve(10);
    std::cout << "2========v1.push_back(test)=========" << std::endl;
    v1.push_back(test);
    std::cout << "2========v1.emplace_back(test)=========" << std::endl;
    v1.emplace_back(test);
    std::cout << "2========v1.push_back(std::move(test))::good=========" << std::endl;
    v1.push_back(std::move(test));
    std::cout << "2========v1.emplace_back(test2)=========" << std::endl;
    v1.emplace_back(test2);
    std::cout << "2========v1.emplace_back(std::move(test2))::good=========" << std::endl;
    v1.emplace_back(std::move(test2));
    std::cout << "2========v1.emplace_back(\"hello\")=========" << std::endl;
    v1.emplace_back("hello");
    std::cout << "2========end=========" << std::endl;
    /// 需要指的注意的是：在代码36行有没有加noexcept区别是很大的,这是是没加noexcept之前的结果
    /**
     * /Users/william/git/sample/cpp_demo/cmake-build-debug/cpp_demo
     * After copy, str is "hello"
     * After move, str is ""
     * The contents of the vector are "hello", "hello"
     * constructor
     * constructor
     * 1========v1.push_back(test)=========
     * copy constructor
     * 1========v1.emplace_back(test)=========
     * copy constructor
     * copy constructor
     * distructor
     * 1========v1.push_back(std::move(test))
     * move constructor
     * copy constructor
     * copy constructor
     * distructor
     * distructor
     * 1========v1.emplace_back(test2)=========
     * copy constructor
     * 1========v1.emplace_back(std::move(test2))
     * move constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * distructor
     * distructor
     * distructor
     * distructor
     * 1========v1.emplace_back("hello")=========
     * constructor()
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * 2========v1.push_back(test)=========
     * copy constructor
     * 2========v1.emplace_back(test)=========
     * copy constructor
     * 2========v1.push_back(std::move(test))::good=========
     * move constructor
     * 2========v1.emplace_back(test2)=========
     * copy constructor
     * 2========v1.emplace_back(std::move(test2))::good=========
     * move constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * copy constructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * 2========v1.emplace_back("hello")=========
     * constructor()
     * 2========end=========
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * Process finished with exit code 0
     */

    /// 这是加了noexcept之后的结果，对比可以通过beyong compare对比一下,会发现move构造加了noexcept之后vector扩容打印出来的才是move构造
    /**
     *
     * /Users/william/git/sample/cpp_demo/cmake-build-debug/cpp_demo
     * After copy, str is "hello"
     * After move, str is ""
     * The contents of the vector are "hello", "hello"
     * constructor
     * constructor
     * 1========v1.push_back(test)=========
     * copy constructor
     * 1========v1.emplace_back(test)=========
     * copy constructor
     * move constructor
     * distructor
     * 1========v1.push_back(std::move(test))
     * move constructor
     * move constructor
     * move constructor
     * distructor
     * distructor
     * 1========v1.emplace_back(test2)=========
     * copy constructor
     * 1========v1.emplace_back(std::move(test2))
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * distructor
     * distructor
     * distructor
     * distructor
     * 1========v1.emplace_back("hello")=========
     * constructor()
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * 2========v1.push_back(test)=========
     * copy constructor
     * 2========v1.emplace_back(test)=========
     * copy constructor
     * 2========v1.push_back(std::move(test))::good=========
     * move constructor
     * 2========v1.emplace_back(test2)=========
     * copy constructor
     * 2========v1.emplace_back(std::move(test2))::good=========
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * move constructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * 2========v1.emplace_back("hello")=========
     * constructor()
     * 2========end=========
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * distructor
     * Process finished with exit code 0
     *
     */
}
