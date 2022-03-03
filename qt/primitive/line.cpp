//
// Created by cwb on 2022/3/2.
//

#include "line.h"

Line::~Line() = default;

void Line::Draw(QPainter& painter)
{
    painter.drawLine(m_start, m_end);
}
