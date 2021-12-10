//
// Created by william on 2021/12/9.
//

#ifndef CPP_DEMO_SHAPE_H
#define CPP_DEMO_SHAPE_H
#include <cstdio>
#include <stdexcept>

namespace geekTimeTest
{
enum class ShapeType
{
    Circle,
    Triangle,
    Rectangle
};

class Shape
{
public:
    Shape() { puts("Shape()"); }
    virtual ~Shape() { puts("~Shape()"); }
};

class Circle : public Shape
{
public:
    Circle() { puts("Circle()"); }
    ~Circle() override { puts("~Circle()"); }
};

class Triangle : public Shape
{
public:
    Triangle() { puts("Triangle()"); }
    ~Triangle() override { puts("~Triangle()"); }
};

class Rectangle : public Shape
{
public:
    Rectangle() { puts("Rectangle()"); }
    ~Rectangle() override { puts("~Rectangle()"); }
};

inline Shape* createShape(ShapeType type)
{
    switch (type)
    {
    case ShapeType::Circle:
        return new Circle();
    case ShapeType::Triangle:
        return new Triangle();
    case ShapeType::Rectangle:
        return new Rectangle();
    }
    throw std::logic_error("shape type is invalid");
}

} // namespace geekTimeTest
#endif //CPP_DEMO_SHAPE_H
