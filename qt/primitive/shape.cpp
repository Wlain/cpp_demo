//
// Created by cwb on 2022/3/2.
//

#include "shape.h"

Shape::Shape() = default;
Shape::~Shape() = default;

void Shape::setStart(const QPoint& p)
{
    m_start = p;
}
void Shape::setEnd(const QPoint& p)
{
    m_end = p;
}
Shape::Shape(Shape&& s) = default;
Shape& Shape::operator=(Shape&&) = default;
