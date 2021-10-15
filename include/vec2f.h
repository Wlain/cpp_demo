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
    virtual ~Vec2f();
    Vec2f(float x, float y);
     Vec2f(const Vec2f&);
    Vec2f& operator=(const Vec2f&); // 拷贝赋值函数
    Vec2f(const Vec2f&&) = delete;           // 移动构造函数
    Vec2f& operator=(Vec2f&&) = default;      // 移动赋值函数
    [[maybe_unused]] inline void setX(float x) { data[0] = x; }
    [[maybe_unused]] inline void setY(float y) { data[1] = y; }
    [[nodiscard]] inline float getX() const { return data[0]; }

    [[nodiscard]] inline float getY() const { return data[1]; }

    inline Vec2f operator-(const Vec2f& v) const
    {
        return Vec2f(getX() - v.getX(), getY() - v.getY());
    }

    [[maybe_unused]] static inline int getDestructorCount()
    {
        return destructorCount;
    }

    [[maybe_unused]] static inline int getConstructorCount()
    {
        return constructorCount;
    }
    static void test(bool t = false);

private:
    float data[2] = { 0, 0 };
    static int constructorCount;
    static int destructorCount;
};

#endif // CPP_DEMO_VEC2F_H
