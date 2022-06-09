//
// Created by william on 2021/6/8.
//

#include "../cpppreference/templateTest.h"
#include "base.h"

// Q1写一个自增类
class Number
{
public:
    explicit Number(int m) :
        m_data(m)
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

// 写一个String类
class String1
{
public:
    explicit String1(const std::size_t sizeOfString) :
        m_data(new char[sizeOfString])
    {
    }
    String1(const char* charArray, const std::size_t sizeOfString)
    {
        m_data = new char[sizeOfString];
        strcpy(m_data, charArray);
    }
    virtual ~String1()
    {
        delete[] m_data;
    }

    char& operator[](const std::size_t index)
    {
        return m_data[index];
    }

    const char& operator[](const std::size_t index) const
    {
        return m_data[index];
    }

    auto getAddr()
    {
        return &m_data;
    }

private:
    char* m_data;
};

class String2
{
};

class BaseImp
{
public:
    BaseImp()
    {
        std::cout << "BaseImp" << std::endl;
    }
};

class DerivedImp1 : public BaseImp
{
public:
    DerivedImp1()
    {
        std::cout << "DerivedImp1" << std::endl;
    }
};

class DerivedImp2 : public BaseImp
{
public:
    DerivedImp2()
    {
        std::cout << "DerivedImp2" << std::endl;
    }
};

// 实现一个atoi函数,并且自测，需要注意什么
/// 1.输入正负号
/// 2.开头有空格
/// 3.转换后的数值超出int的表示范围
/// 4.出错时返回0与正确转换0的区别
/// 5.输入非数字
/// 6.空字符串

// "100",
//"-1"
//"+1"
//"  "
//"111111111111"
//""
//"1aab"

int atoiTest(const char* str)
{
    int flag = 1, result = 0;
    if (str == nullptr)
    {
        return result;
    }
    /// 检测字符串是否只由数字组成
    if (isdigit(*str))
    {
        return -1;
    }
    if ('-' == *str)
    {
        flag = -1;
        str++;
    }
    else if ('+' == *str)
    {
        str++;
    }
    while (*str != ' ')
    {
        str++;
    }
    while (*str <= '9' && *str >= '0')
    {
        result = 10 * result + (*str++ - '0');
        str++;
        if (result > flag ? -(long long)INT_MIN : INT_MAX)
        {
            return flag ? INT_MIN : INT_MAX;
        }
    }
    return flag + result;
}

// 实现一个strcat函数,将字符串str2添加到字符串str1的末端，并且自测
const char* testStrcat(char* dest, char* src)
{
    assert(dest != nullptr && src != nullptr);
    char* p = dest;
    // 找到dest串结束符的位置
    while (*dest != '\0')
    {
        ++dest;
    }
    while (*src != '\0')
    {
        *dest = *src;
        ++dest;
        ++src;
    }
    // 加入串结束符
    *dest = '\0';
    return p;
}

// 实现一个strcpy函数,后面的字符串会拷贝到一个字符数组中，要求拷贝好的字符串在字符数组的首地址，并且自测
char* testStrcpy(char* dest, const char* src)
{
    //注意细节
    //判断是否为空 为空直接抛出异常
    assert(dest != nullptr && src != nullptr);

    //判断是否为自我复制
    if (dest == src) return dest;

    char* p = dest; //存储目标数组起点地址
    while (*src != '\0')
    {
        *dest = *src;
        ++dest;
        ++src;
    }
    *dest = '\0';
    return p; //支持链式表达式
}

void interviews()
{
    float* t1, t2;
    /// 注意此时声明的是：float* t1, float t2;

    unsigned int a = -1; //std::pow(2, 31);
    int b = (int&)a;
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
    int value = 2048;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    int max = MAX(12, ++value);

    auto n = 1024 * 1024 * 5;
    char* buf = new char[n];
    // 问题1：创建出来的是一个什么样的数组
    std::cout << n << std::endl;
    std::vector<int> bigVector(n);
    atoi("hello");
    //    for (unsigned i = 0; i < bigVector.size(); ++i)
    //    {
    //        std::cout << i << std::endl;
    //    }
    /// 这段diamante有问题吗？n过大，将引起无限循环

    /// 迭代器异常

    /// 写一个类
    String1 str1("Test", 4);
    String1 str2{ str1 };
    std::cout << "&str1:" << str1.getAddr() << std::endl;
    std::cout << "&str2:" << str2.getAddr() << std::endl;
    //    TemplateTest<DerivedImp1> t1;
    //    TemplateTest<DerivedImp2> t1;
}