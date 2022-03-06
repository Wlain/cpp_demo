//
// Created by cwb on 2022/3/2.
//

#ifndef CPP_DEMO_LINE_H
#define CPP_DEMO_LINE_H

#include "shape.h"

class Line : public Shape
{
public:
    using Shape::Shape;
    ~Line() override;
    void draw(QPainter& painter, QPaintDevice* device) override;
};

#endif //CPP_DEMO_LINE_H
