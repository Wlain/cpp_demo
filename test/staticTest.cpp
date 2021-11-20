//
// Created by william on 2021/11/20.
//

/// 可以做为面试题
#include <iostream>
#include <string>

class Empty
{

};

class Test
{
public:
    std::string name = "hello world";
    static std::string str;
};
std::string Test::str = std::string(1000, 'c');

void staticTest()
{
    Test t;
    std::cout << sizeof("hello world") << std::endl;
    std::cout << sizeof(Empty) << std::endl;
    std::cout << sizeof(t) << std::endl;
    std::cout << sizeof(Test) << std::endl;
}


