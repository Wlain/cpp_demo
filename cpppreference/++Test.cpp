//
// Created by william on 2021/9/3.
//
#include "base.h"

class Number
{
public:
    explicit Number(int m) : m_data(m)
    {
    }

    Number& operator++() /// 前置++
    {
        ++m_data;
        return *this;
    }

    Number operator++(int n) /// 后置++
    {
        Number s = *this;
        ++(*this);
        return s;
    }

    int operator()()
    {
        return m_data;
    }
private:
    int m_data;
};

void addAddTest()
{

}