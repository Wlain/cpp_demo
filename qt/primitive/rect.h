//
// Created by cwb on 2022/3/2.
//

#ifndef CPP_DEMO_RECT_H
#define CPP_DEMO_RECT_H
#include "shape.h"

class Rect : public Shape
{
public:
    using Shape::Shape;
    ~Rect() override;
    void draw(QPainter& painter, QPaintDevice* device) override;
};

#endif //CPP_DEMO_RECT_H
