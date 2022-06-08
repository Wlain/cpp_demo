//
// Created by william on 2022/5/31.
//
#include <iostream>
/*
C++三大特性之：继承
 */
namespace cpp
{

class A
{
public:
    int m_a{ 0 };
};

class B
{
public:
    int m_b{ 1 };
};

class C : public A, public B
{
public:
    int getA() { return A::m_a; }
    int m_a{ 2 };
};

int inheritsTest()
{
    C* c = new C();
    A* a = c;
    B* b = c;
    /// 注意：a的地址和c的地址是一样的，因为他们的第一个类成员变量是一样的，该地址都指向m_a变量的地址,但是由于类C中重定义了，
    /// 通过a->m_a 是0,通过c->m_a是2

    std::cout << "sizeof a = " << sizeof(A) << std::endl;
    std::cout << "a = " << a << std::endl;
    std::cout << "sizeof b = " << sizeof(B) << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "sizeof c = " << sizeof(C) << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "m_a(A) = " << a->m_a << std::endl;
    std::cout << "m_b(B)= " << c->m_b << std::endl;
    std::cout << "m_a(C) = " << c->m_a << std::endl;
    std::cout << "m_a(C->A) = " << c->getA() << std::endl;
    int tempA = (int)(*(int*)(c));
    std::cout << "tempA = " << tempA << std::endl;
    int tempB = (int)(*(int*)(c) + 1);
    std::cout << "tempB = " << tempB << std::endl;
    int tempC = (int)(*(int*)(c) + 2);
    std::cout << "tempC = " << tempC << std::endl;
    /* output
    sizeof a = 4
	a = 0x600000cb4030
	sizeof b = 4
	b = 0x600000cb4034
	sizeof c = 12
	c = 0x600000cb4030
	m_a(A) = 0
	m_b(B)= 1
	m_a(C) = 2
	m_a(C->A) = 0
	tempA = 0
	tempB = 1
	tempC = 2
    */
}
} // namespace cpp