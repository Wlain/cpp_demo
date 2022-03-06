//
// Created by william on 2022/3/6.
//

#include "oval.h"

Oval::~Oval() = default;

void Oval::draw(QPainter& painter, QPaintDevice* device)
{
    Shape::draw(painter, device);
    painter.drawEllipse(m_start.x(), m_start.y(), m_end.x() - m_start.x(), m_end.y() - m_start.y());
    painter.end();
}
