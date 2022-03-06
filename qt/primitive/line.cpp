//
// Created by cwb on 2022/3/2.
//

#include "line.h"

Line::~Line() = default;

void Line::draw(QPainter& painter, QPaintDevice* device)
{
    Shape::draw(painter, device);
    painter.drawLine(m_start, m_end);
    painter.end();
}
