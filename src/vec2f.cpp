//
// Created by william on 2020/9/14.
//

#include "vec2f.h"

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

[[maybe_unused]] Vec2f::Vec2f(const Vec2f&)
{
    std::cout << "copy constructor called" << std::endl;
    constructorCount++;
}

Vec2f::~Vec2f()
{
    destructorCount++;
    std::cout << "destructor called" << std::endl;
}
