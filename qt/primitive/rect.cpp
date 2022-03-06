//
// Created by cwb on 2022/3/2.
//

#include "rect.h"

Rect::~Rect() = default;

void Rect::draw(QPainter& painter, QPaintDevice* device)
{
    Shape::draw(painter, device);
    painter.drawRect(m_start.x(), m_start.y(), m_end.x() - m_start.x(), m_end.y() - m_start.y());
    painter.end();
}
