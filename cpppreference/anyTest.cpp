//
// Created by william on 2021/11/19.
//
#include <any>
#include <iostream>
#include <memory>
/*
 * any 描述用于任何类型的单个值的类型安全容器
 * any 的对象存储任何满足构造函数要求的类型的一个实例或为空，而这被称为any类对象的状态。存储的实例被称作所含对象。若两个状态均为空，或均为非空且其所含对象等价，则两个状态等价。
 * 非成员 any_cast 函数提供对所含对象的类型安全访问
 */

template <typename Type, int DIM>
class Vec
{
public:
    enum
    {
        VEC_DIM = DIM
    };

    Vec(Type x, Type y)
    {
        m_data[0] = x;
        m_data[1] = y;
        std::cout << "Vec(" << x << "," << y << ") called" << std::endl;
    }
    Vec()
    {
        std::cout << "Vec() called" << std::endl;
    }

    ~Vec()
    {
        std::cout << "~Vec() called" << std::endl;
    }

    inline Type& x()
    {
        return m_data[0];
    }
    inline Type& y()
    {
        return m_data[1];
    }

    inline const Type& operator[](int index) const
    {
        return m_data[index];
    }

public:
    Type m_data[DIM];
};

typedef Vec<float, 2> Vec2fTest;

class Object
{
public:
    Object()
    {
        std::cout << "Object() called" << std::endl;
    }
    ~Object()
    {
        std::cout << "~Object() called" << std::endl;
    }
    template <typename DataType>
    void setData(DataType&& d)
    {
        std::cout << "setData::d:" << &d << std::endl;
        m_data = std::forward<DataType>(d);
        std::cout << "setData::m_data:" << &m_data << std::endl;
        std::cout << "setData::m_data.type():" << m_data.type().name() << std::endl;
    }

    void getData()
    {
        std::cout << "getData::m_data.type():" << m_data.type().name() << std::endl;
        setVec2(*std::any_cast<Vec2fTest>(&m_data));
    }

    void setVec2(const Vec2fTest& test)
    {
        std::cout << "(" << test[0] << "," << test[1] << ")" << std::endl;
    }

private:
    std::any m_data;
};

void anyTest()
{
    std::cout << std::boolalpha;
    // any 类型
    std::any a = 1;
    std::cout << a.type().name() << ": " << std::any_cast<int>(a) << std::endl;
    a = 3.14;
    std::cout << a.type().name() << ": " << std::any_cast<double>(a) << std::endl;
    a = true;
    std::cout << a.type().name() << ": " << std::any_cast<bool>(a) << std::endl;

    // failure test
    try
    {
        a = 1;
        std::cout << std::any_cast<float>(a) << std::endl;
    }
    catch (const std::bad_any_cast& e)
    {
        std::cout << e.what() << std::endl;
    }
    // owner
    a = 1;
    if (a.has_value())
    {
        std::cout << a.type().name() << '\n';
    }
    // reset
    a.reset();
    if (!a.has_value())
    {
        std::cout << "no value\n";
    }
    // 指向所含数据的指针
    a = 1;
    int* i = std::any_cast<int>(&a);
    std::cout << *i << std::endl;

    std::shared_ptr<Object> obj = std::make_shared<Object>();
    obj->setData(Vec2fTest(1.0f, 1.0f));
    obj->getData();
}
