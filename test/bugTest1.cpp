//
// Created by cwb on 2022/3/29.
//
// CPP program to illustrate calling virtual methods in constructor/destructor
#include <iostream>
using namespace std;

class Dog
{
public:
    Dog()
    {
        cout << "Constructor called" << endl;
        bark();
    }

    virtual ~Dog() = default;

    virtual void bark()
    {
        cout << "Virtual method called" << endl;
    }

    void seeCat()
    {
        bark();
    }
};

class YellowDog : public Dog
{
public:
    YellowDog()
    {
        cout << "Derived class Constructor called" << endl;
        bark();
    }

    ~YellowDog() override = default;

    void bark() override
    {
        cout << "Derived class Virtual method called" << endl;
    }
};

void virtualTest()
{
    YellowDog d;
}
