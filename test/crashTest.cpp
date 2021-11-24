//
// Created by ytech on 2021/11/24.
//
#include <iostream>

class Foo
{
public:
    void func()
    {
        std::cout << m_name << std::endl;
    }

private:
    std::string m_name;
};

class Test
{
public:
    void func()
    {
        m_foo->func();
    }
private:
    std::shared_ptr<Foo> m_foo = std::make_shared<Foo>();
};

void crashTest()
{
    Test* test = nullptr;
    test->func();
}