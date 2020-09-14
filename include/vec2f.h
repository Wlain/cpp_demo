//
// Created by william on 2020/9/14.
//

#ifndef CPP_DEMO_VEC2F_H
#define CPP_DEMO_VEC2F_H

#include "base.h"

class Vec2f
{
public:
    Vec2f();
    Vec2f(float x, float y);
    Vec2f(const Vec2f &);
    virtual ~Vec2f();
    Vec2f &operator=(const Vec2f &) = default;

    inline void setX(int x) { data[0] = x; }

    inline void setY(int y) { data[1] = y; }

    inline float getX() const { return data[0]; }

    inline float getY() const { return data[1]; }

    inline Vec2f operator-(const Vec2f &v) const
    {
        return Vec2f(getX() - v.getX(), getY() - v.getY());
    }
    static inline int getDestructorCount()
    {
        return destructorCount;
    }

    static inline int getConstructorCount()
    {
        return constructorCount;
    }

private:
    float data[2] = {0, 0};
    static int constructorCount;
    static int destructorCount;
};

#endif // CPP_DEMO_VEC2F_H
