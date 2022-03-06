//
// Created by william on 2022/3/6.
//

#include "pen.h"

Pen::~Pen() = default;

void Pen::draw(QPainter& painter, QPaintDevice* device)
{
    Shape::draw(painter, device);
    if (m_path.elementCount() == 0)
    {
        m_path.moveTo(m_end);
    }
    else
    {
        m_path.lineTo(m_end);
    }
    painter.drawPath(m_path);
    painter.end();
}
