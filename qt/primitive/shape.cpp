//
// Created by cwb on 2022/3/2.
//

#include "shape.h"

Shape::Shape() = default;
Shape::~Shape() = default;
Shape::Shape(Shape&& s) = default;
Shape& Shape::operator=(Shape&&) = default;

void Shape::setStart(const QPoint& p)
{
    m_start = p;
}
void Shape::setEnd(const QPoint& p)
{
    m_end = p;
}

void Shape::setPen(const QPen& pen)
{
    m_pen = pen;
}

void Shape::draw(QPainter& painter, QPaintDevice* device)
{
    painter.begin(device);
    painter.setPen(m_pen);
}
