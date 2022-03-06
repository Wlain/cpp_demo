//
// Created by william on 2022/3/6.
//

#ifndef CPP_DEMO_OVAL_H
#define CPP_DEMO_OVAL_H
#include "shape.h"
class Oval : public Shape
{
public:
    using Shape::Shape;
    ~Oval() override;
    void draw(QPainter& painter, QPaintDevice* device) override;
};

#endif //CPP_DEMO_OVAL_H
