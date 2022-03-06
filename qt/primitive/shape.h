//
// Created by cwb on 2022/3/2.
//

#ifndef CPP_DEMO_SHAPE_H
#define CPP_DEMO_SHAPE_H
#include <QPainter>
#include <QPoint>

class Shape
{
public:
    enum class Primitive
    {
        None,
        Line,
        Rect,
        Oval,
        Pen
    };

public:
    Shape();
    virtual ~Shape();
    Shape(Shape&& s);
    Shape& operator=(Shape&&);
    Shape(const Shape& s) = delete;
    Shape& operator=(const Shape&&) = delete;

    void setStart(const QPoint& p);
    void setEnd(const QPoint& p);
    void setPen(const QPen& pen);
    virtual void draw(QPainter& painter, QPaintDevice* device);

protected:
    QPoint m_start;
    QPoint m_end;
    QPen m_pen;
};

#endif //CPP_DEMO_SHAPE_H
