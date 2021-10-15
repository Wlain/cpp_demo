//
// Created by william on 2021/1/29.
//
#include "base.h"

class Base
{
public:
    virtual ~Base() = default;
};

class Derived : public Base
{
public:
    ~Derived() override = default;
    int getN() { return n; }

private:
    int n;
};

void testSmartPointer()
{
    std::shared_ptr<Base> base = std::make_shared<Derived>();
    auto&& temp =  std::dynamic_pointer_cast<Derived>(base);
    std::cout << temp->getN() << std::endl;
}