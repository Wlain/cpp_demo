//
// Created by william on 2022/3/6.
//

#ifndef CPP_DEMO_PEN_H
#define CPP_DEMO_PEN_H
#include "line.h"
#include <QPainterPath>

class Pen : public Shape
{
public:
    using Shape::Shape;
    ~Pen() override;
    void draw(QPainter& painter, QPaintDevice* device) override;

protected:
    QPainterPath m_path;
};

#endif //CPP_DEMO_PEN_H
