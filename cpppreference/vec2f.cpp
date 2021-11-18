// Created by william on 2020/9/14.
//

#include "vec2f.h"
int Vec2f::constructorCount = 0;
int Vec2f::destructorCount = 0;

Vec2f::Vec2f()
{
    std::cout << "constructor called" << std::endl;
    constructorCount++;
}

Vec2f::Vec2f(float x, float y)
{
    std::cout << "constructor called" << std::endl;
    data[0] = x;
    data[1] = y;
    constructorCount++;
}

Vec2f::Vec2f(const Vec2f&)
{
    std::cout << "copy constructor called" << std::endl;
    constructorCount++;
}

Vec2f::~Vec2f()
{
    destructorCount++;
    std::cout << "destructor called" << std::endl;
}
Vec2f& Vec2f::operator=(const Vec2f& vec)
{
    destructorCount++;
    std::cout << "destructor called" << std::endl;
    if (this == &vec)
    {
        return *this;
    }
    return *this;
}
void Vec2f::test(bool t)
{
}
