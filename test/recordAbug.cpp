//
// Created by william on 2022/3/10.
//
#include <iostream>
#include <string>

std::string getAString()
{
    return "hello";
}

void recordABug()
{
    std::cout << getAString() << std::endl; /// 这是正确的写法
    auto test = getAString().c_str();       /// Object backing the pointer will be destroyed at the end of the full-expression
    std::cout << test << std::endl;         /// 这是错误的写法，因为getAString()调用完这个对象就析构了，在调用c_str(),拿到的东西可能是一个空的值
}