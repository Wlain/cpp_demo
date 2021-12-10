//
// Created by william on 2021/12/10.
//

#include <iostream> // std::cout/endl
#include <utility>  // std::move
using namespace std;

namespace geekTimeTest
{
class Obj
{
public:
    Obj()
    {
        cout << "Obj()" << endl;
    }
    Obj(const Obj&)
    {
        cout << "Obj(const Obj&)" << endl;
    }
    Obj(Obj&&) noexcept
    {
        cout << "Obj(Obj&&)" << endl;
    }
};

Obj simple()
{
    Obj obj;
    return obj;
}

Obj simpleWithMove()
{
    Obj obj;
    return move(obj);
}

Obj complicated(int n)
{
    Obj obj1;
    Obj obj2;
    return (n % 2 == 0) ? obj1 : obj2;
}

void nrvoTest()
{
    cout << "*** 1 ***" << endl;
    auto obj1 = simple();
    cout << "*** 2 ***" << endl;
    auto obj2 = simpleWithMove();
    cout << "*** 3 ***" << endl;
    auto obj3 = complicated(42);
}
} // namespace geekTimeTest