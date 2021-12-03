//
// Created by ytech on 2021/12/3.
//

#include <iostream>

class Func
{
public:
    Func()
    {
        std::cout << "Func constructor call" << std::endl;
    }

    void print()
    {
        std::cout << "Func print" << std::endl;
    }
};

class Test
{
public:
    void begin()
    {
        if (get())
        {
            get()->print();
        }
    }

    void end()
    {
        if (get())
        {
            get()->print();
        }
    }

    static Func* get()
    {
        static std::shared_ptr<Func> s_func = std::make_shared<Func>();
        return s_func.get();
    }
};

void staticTest2()
{
    Test t;
    t.begin();
    t.begin();
    t.begin();
    t.begin();
    t.begin();
    t.end();
}
