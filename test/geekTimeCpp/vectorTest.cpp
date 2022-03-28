//
// Created by william on 2021/12/12.
//
#include <iostream>
#include <vector>

using namespace std;

namespace geekTimeTest
{
class Obj1
{
public:
    Obj1()
    {
        cout << "Obj1()\n";
    }
    Obj1(const Obj1&)
    {
        cout << "Obj1(const Obj1&)\n";
    }
    Obj1(const Obj1&&)
    {
        cout << "const Obj1(Obj1&&)\n";
    }
};

class Obj2
{
public:
    Obj2()
    {
        cout << "Obj2()\n";
    }
    Obj2(const Obj2&)
    {
        cout << "Obj2(const Obj2&)\n";
    }
    Obj2(const Obj2&&) noexcept
    {
        cout << "const Obj2(Obj2&&)\n";
    }
};

void vectorTest()
{
    vector<Obj1> v1;
    v1.reserve(2);
    v1.emplace_back();
    v1.emplace_back();
    v1.emplace_back();
    std::cout << "====================" << std::endl;
    vector<Obj2> v2;
    v2.reserve(2);
    v2.emplace_back();
    v2.emplace_back();
    v2.emplace_back();
    std::vector<int> v;
    std::cout << "Default-constructed capacity is " << v.capacity() << '\n';
    v.resize(100);
    std::cout << "Capacity of a 100-element vector is " << v.capacity() << '\n';
    v.resize(50);
    std::cout << "Capacity after resize(50) is " << v.capacity() << '\n';
    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << v.capacity() << '\n';
    v.clear();
    std::cout << "Capacity after clear() is " << v.capacity() << '\n';
    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << v.capacity() << '\n';
    for (int i = 1000; i < 1300; ++i)
        v.push_back(i);
    std::cout << "Capacity after adding 300 elements is " << v.capacity() << '\n';
    v.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit() is " << v.capacity() << '\n';
}
} // namespace geekTimeTest