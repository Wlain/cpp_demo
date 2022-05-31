//
// Created by william on 2022/5/31.
//

/*
C++三大特性之：多态
总结：vtable只是内存中的一个静态块，主要包含指函数指针。当一个多态类派生自另一个多态类时，我们可能有以下几种情况：
1.派生类不会添加新的虚函数，也不会覆盖任何虚函数。在这种情况下，此类与基类共享 vtable。
2.派生类添加和覆盖虚拟方法。在这种情况下，它有自己的 vtable，其中添加的虚函数的索引从最后一个派生的开始。
3.继承中的多个多态类。在这种情况下，我们在第二个和下一个基数之间有一个索引移位，以及它在派生类中的索引
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace cpp
{
/// 模拟实现C++多态：实现的版本一
struct vtable;

class Animal
{
public:
    ~Animal() { destructors(); }

    const std::string& getName() const { return m_name; }

    // 纯虚函数
    bool hasTail() const;

    bool hasWings() const;

    void sound() const;

    void destructors() const;

protected: // we do not want animals to be created directly
    Animal(const vtable* vtable, std::string_view name) :
        m_vtablePtr(vtable), m_name(name)
    {
    }

private:
    const vtable* const m_vtablePtr;
    std::string m_name;
    friend vtable; // just for no-public methods
};

class Cat : public Animal
{
public:
    Cat(std::string_view name);

    // functions to bind dynamically
    bool hasTail() const { return true; }

    bool hasWings() const { return false; }

    void sound() const { std::cout << getName() << " does meow\n"; }

    void destructors() const { std::cout << "~Cat" << std::endl; }
};

class Dog : public Animal
{
public:
    Dog(std::string_view name);

    ~Dog() { std::cout << "~Dog" << std::endl; }

    bool hasTail() const { return true; }

    bool hasWings() const { return false; }

    void sound() const { std::cout << getName() << " does whoof\n"; }

    void destructors() const { std::cout << "~Dog" << std::endl; }
};

// 鹦鹉
class Parrot : public Animal
{
public:
    Parrot(std::string_view name);

    ~Parrot() { std::cout << "~Parrot" << std::endl; }

    bool hasTail() const { return false; }

    bool hasWings() const { return true; }

    void sound() const { std::cout << getName() << " does crrra\n"; }

    void destructors() const { std::cout << "~Parrot" << std::endl; }
};

// now the magic - pointers to member functions!
struct vtable
{
    bool (Animal::*const hasTail)() const;

    bool (Animal::*const hasWings)() const;

    void (Animal::*const sound)() const;

    void (Animal::*const destructors)() const;

    // constructor
    vtable(
        bool (Animal::*const hasTail)() const,
        bool (Animal::*const hasWings)() const,
        void (Animal::*const sound)() const,
        void (Animal::*const destructors)() const) :
        hasTail(hasTail), hasWings(hasWings), sound(sound), destructors(destructors) {}
};

// global vtable objects
const vtable vtableCat(
    static_cast<bool (Animal::*)() const>(&Cat::hasTail),
    static_cast<bool (Animal::*)() const>(&Cat::hasWings),
    static_cast<void (Animal::*)() const>(&Cat::sound),
    static_cast<void (Animal::*)() const>(&Cat::destructors));

const vtable vtableDog(
    static_cast<bool (Animal::*)() const>(&Dog::hasTail),
    static_cast<bool (Animal::*)() const>(&Dog::hasWings),
    static_cast<void (Animal::*)() const>(&Dog::sound),
    static_cast<void (Animal::*)() const>(&Dog::destructors));

const vtable vtableParrot(
    static_cast<bool (Animal::*)() const>(&Parrot::hasTail),
    static_cast<bool (Animal::*)() const>(&Parrot::hasWings),
    static_cast<void (Animal::*)() const>(&Parrot::sound),
    static_cast<void (Animal::*)() const>(&Parrot::destructors));

// set vtable pointers in constructors
Cat::Cat(std::string_view name) :
    Animal(&vtableCat, name)
{}

Dog::Dog(std::string_view name) :
    Animal(&vtableDog, name)
{}

Parrot::Parrot(std::string_view name) :
    Animal(&vtableParrot, name)
{}

// implement dynamic dispatch
bool Animal::hasTail() const
{
    return (this->*(m_vtablePtr->hasTail))();
}

bool Animal::hasWings() const
{
    return (this->*(m_vtablePtr->hasWings))();
}

void Animal::sound() const
{
    (this->*(m_vtablePtr->sound))();
}

void Animal::destructors() const
{
    (this->*(m_vtablePtr->destructors))();
}

/// 模拟实现C++多态：实现的版本二
// 父类实现
struct Foo; // 前向声明
struct slotsFoo
{
    const void* parentVatable; // 单一继承
    void (*destructor)(Foo*);  // virtual destructor Foo::~Foo
    int (*a)(Foo*);            // virtual function Foo::a
};

// class Foo:基类
struct Foo
{
    const struct slotsFoo* vtable; // each instance points to vtable
};

void destructFoo(Foo* self)
{} // Foo::~Foo();
int aFoo(Foo* self)
{
    return 1;
} // Foo::a()
const struct slotsFoo vtableFoo = {
    0, // no parent class
    destructFoo,
    aFoo
};

void constructFoo(Foo* self) // Foo::Foo()
{
    self->vtable = &vtableFoo; // // object points to class vtable
}

void copyConstructFoo(Foo* self, Foo* other) // Foo::Foo(const Foo&)
{
    self->vtable = &vtableFoo; // don't copy from other!
}

// class Bar : 子类
struct Bar // class Bar
{
    Foo base; //  继承了 Foo 的所有成员
};

void destructBar(Bar* self)
{} // Bar::~Bar
int aBar(Bar* self)
{
    return 2;
} // Bar::a()
const struct slotsFoo vtableBar = { // one more constant table
    &vtableFoo,
    (void (*)(Foo*))destructBar, // must cast type to avoid errors
    (int (*)(Foo*))aBar
};

void constructBar(Bar* self)
{
    self->base.vtable = &vtableBar;
}

void func(Foo* arg)
{ // same functionality as above
    Foo x;
    constructFoo(&x);
    aFoo(&x);
    Bar y;
    constructBar(&y);
    aBar(&y);
    arg->vtable->a(arg); // virtual function call
    Foo z;
    copyConstructFoo(&z, arg);
    aFoo(&z);
    destructFoo(&z);
    destructBar(&y);
    destructFoo(&x);
}

void virtualSimulation()
{
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Cat>("grumpy"));
    animals.push_back(std::make_unique<Cat>("nyan"));
    animals.push_back(std::make_unique<Dog>("doge"));
    animals.push_back(std::make_unique<Parrot>("party"));
    for (const auto& a : animals)
        a->sound();

    // 当然，我们也可以手动调用虚函数表，示例如下
    class Base
    {
    public:
        virtual ~Base()
        {
            std::cout << "Base::Base called" << std::endl;
        }
        virtual void func1()
        {
            std::cout << "Base::func1 called" << std::endl;
        }
        virtual void func2()
        {
            std::cout << "Base::func2 called" << std::endl;
        }
    };

    class Base2
    {
    public:
        virtual ~Base2()
        {
            std::cout << "Base2::Base called" << std::endl;
        }
        virtual void func3()
        {
            std::cout << "Base2::func3 called" << std::endl;
        }
        virtual void func4()
        {
            std::cout << "Base2::func4 called" << std::endl;
        }
    };

    class Derived : public Base2, public Base
    {
    public:
        ~Derived() override
        {
            std::cout << "~Derived::Derived called" << std::endl;
        }
        void func1() override
        {
            std::cout << "Derived::func1 called" << std::endl;
        }
        void func2() override
        {
            std::cout << "Derived::func2 called" << std::endl;
        }
        void func3() override
        {
            std::cout << "Derived::func3 called" << std::endl;
        }
        void func4() override
        {
            std::cout << "Derived::func4 called" << std::endl;
        }
    };

    class Derived2 : public Base
    {
    public:
        ~Derived2() override
        {
            std::cout << "~Derived2::Derived2 called" << std::endl;
        }
        void func1() override
        {
            std::cout << "Derived2::func1 called" << std::endl;
        }
        void func2() override
        {
            std::cout << "Derived2::func2 called" << std::endl;
        }
    };

    Derived* d = new Derived();
    // 虚表指针
    std::cout << "vatable addr:" << (int64_t*)d << std::endl;
    using Func = void (*)();
    std::cout << "Base::~Base:" << (int64_t*)(*(int64_t*)(d)) << std::endl;
    std::cout << "Base2::~Base2:" << (int64_t*)(*(int64_t*)d + 1) << std::endl;
    std::cout << "Derived::func1 addr:" << (*((int64_t*)(*(int64_t*)d) + 2)) << std::endl;
    std::cout << "Derived::func2 addr:" << (*((int64_t*)(*(int64_t*)d) + 3)) << std::endl;
    std::cout << "Derived::func3 addr:" << (*((int64_t*)(*(int64_t*)d) + 4)) << std::endl;
    std::cout << "Derived::func4 addr:" << (*((int64_t*)(*(int64_t*)d) + 5)) << std::endl;
    std::cout << "=================================" << std::endl;
    for (int i = 2; i <= 5; ++i)
    {
        Func func = (Func)(*((int64_t*)(*(int64_t*)d) + i));
        func();
    }
    Base* b = d;
    // 虚表指针
    std::cout << "vatable addr:" << (int64_t*)b << std::endl;

    std::cout << "Base::~Base:" << (int64_t*)(*(int64_t*)(b)) << std::endl;
    std::cout << "Base2::~Base2:" << (int64_t*)(*(int64_t*)b + 1) << std::endl;
    std::cout << "Derived::func1 addr:" << (*((int64_t*)(*(int64_t*)b) + 2)) << std::endl;
    std::cout << "Derived::func2 addr:" << (*((int64_t*)(*(int64_t*)b) + 3)) << std::endl;
    std::cout << "Derived::func3 addr:" << (*((int64_t*)(*(int64_t*)b) + 4)) << std::endl;
    std::cout << "Derived::func4 addr:" << (*((int64_t*)(*(int64_t*)b) + 5)) << std::endl;
    for (int i = 2; i <= 3; ++i)
    {
        Func func = (Func)(*((int64_t*)(*(int64_t*)b) + i));
        func();
    }
    std::cout << "=================================" << std::endl;
    Base2* b2 = d;
    // 虚表指针
    std::cout << "vatable addr:" << (int64_t*)b2 << std::endl;
    std::cout << "Base::~Base:" << (int64_t*)(*(int64_t*)(b2)) << std::endl;
    std::cout << "Base2::~Base2:" << (int64_t*)(*(int64_t*)b2 + 1) << std::endl;
    std::cout << "Derived::func1 addr:" << (*((int64_t*)(*(int64_t*)b2) + 2)) << std::endl;
    std::cout << "Derived::func2 addr:" << (*((int64_t*)(*(int64_t*)b2) + 3)) << std::endl;
    std::cout << "Derived::func3 addr:" << (*((int64_t*)(*(int64_t*)b2) + 4)) << std::endl;
    std::cout << "Derived::func4 addr:" << (*((int64_t*)(*(int64_t*)b2) + 5)) << std::endl;
    for (int i = 2; i <= 5; ++i)
    {
        Func func = (Func)(*((int64_t*)(*(int64_t*)b2) + i));
        func();
    }
    std::cout << "=================================" << std::endl;

    Derived2* d2 = new Derived2();
    // 虚表指针
    std::cout << "vatable addr:" << (int64_t*)d2 << std::endl;
    using Func = void (*)();
    std::cout << "Base::~Base:" << (int64_t*)(*(int64_t*)(d2)) << std::endl;
    std::cout << "Derive2::func1 addr:" << (*((int64_t*)(*(int64_t*)d2) + 2)) << std::endl;
    std::cout << "Derive2::func2 addr:" << (*((int64_t*)(*(int64_t*)d2) + 3)) << std::endl;
    for (int i = 2; i <= 3; ++i)
    {
        Func func = (Func)(*((int64_t*)(*(int64_t*)d2) + i));
        func();
    }
    std::cout << "=================================" << std::endl;
    Base* b3 = d2;
    // 虚表指针
    std::cout << "vatable addr:" << (int64_t*)b3 << std::endl;
    using Func = void (*)();
    std::cout << "Base::~Base:" << (int64_t*)(*(int64_t*)(b3)) << std::endl;
    std::cout << "Derive2::func1 addr:" << (*((int64_t*)(*(int64_t*)b3) + 2)) << std::endl;
    std::cout << "Derive2::func2 addr:" << (*((int64_t*)(*(int64_t*)b3) + 3)) << std::endl;
    for (int i = 2; i <= 3; ++i)
    {
        Func func = (Func)(*((int64_t*)(*(int64_t*)b3) + i));
        func();
    }

    std::cout << "=================================" << std::endl;
    delete d;
    delete d2;
}
} // namespace cpp