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
        Default,
        Line,
        Rect
    };

public:
    Shape();
    virtual ~Shape();
    Shape(Shape&& s);
    Shape(const Shape& s) = delete;
    Shape& operator=(Shape&&);
    Shape& operator=(const Shape&&) = delete;


    void setStart(const QPoint& p);
    void setEnd(const QPoint& p);
    virtual void Draw(QPainter& painter) = 0;

protected:
    QPoint m_start;
    QPoint m_end;
};

#endif //CPP_DEMO_SHAPE_H
