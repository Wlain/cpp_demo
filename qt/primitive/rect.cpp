//
// Created by cwb on 2022/3/2.
//

#include "rect.h"

Rect::~Rect() = default;

void Rect::Draw(QPainter& painter)
{
    painter.drawRect(m_start.x(), m_start.y(), m_end.x() - m_start.x(), m_end.y() - m_start.y());
}
