//
// Created by william on 2021/5/3.
//

#include "vector3.h"

#include "base.h"
#include "commonMacro.h"

#include <cfloat>

Vector3::Vector3() :
    x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float x_, float y_, float z_) :
    x(x_), y(y_), z(z_)
{
}

Vector3::Vector3(float x_) :
    x(x_), y(x_), z(x_)
{
}

Vector3::Vector3(const float* array)
{
    set(array);
}

Vector3::Vector3(const Vector3& p1, const Vector3& p2)
{
    set(p1, p2);
}

Vector3::Vector3(const Vector3& copy)
{
    set(copy);
}

Vector3::~Vector3() = default;

void Vector3::cross(const Vector3& v)
{
    cross(*this, v, this);
}

void Vector3::cross(const Vector3& v1, const Vector3& v2, Vector3* dst)
{
    ASSERT(dst);
    float x = (v1.y * v2.z) - (v1.z * v2.y);
    float y = (v1.z * v2.x) - (v1.x * v2.z);
    float z = (v1.x * v2.y) - (v1.y * v2.x);
    dst->x = x;
    dst->y = y;
    dst->z = z;
}

float Vector3::length() const
{
    return sqrtf(lengthSquared());
}

float Vector3::lengthSquared() const
{
    return (powf(x, 2) + powf(y, 2) + powf(z, 2));
}

float Vector3::dot(const Vector3& v) const
{
    return dot(*this, v);
}

float Vector3::dot(const Vector3& v1, const Vector3& v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

void Vector3::subtract(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vector3::subtract(const Vector3& v1, const Vector3& v2, Vector3* dst)
{
    ASSERT(dst);

    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
    dst->z = v1.z - v2.z;
}

Vector3& Vector3::normalize()
{
    normalize(this);
    return *this;
}

void Vector3::normalize(Vector3* dst) const
{
    ASSERT(dst);

    if (dst != this)
    {
        dst->x = x;
        dst->y = y;
        dst->z = z;
    }
    float n = length();
    if (n == 1.0f || n < FLT_MIN)
    {
        return;
    }
    n = 1.0f / n;
    dst->x *= n;
    dst->y *= n;
    dst->z *= n;
}

void Vector3::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3::set(const float* array)
{
    ASSERT(array);

    x = array[0];
    y = array[1];
    z = array[2];
}

void Vector3::set(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

void Vector3::set(const Vector3& p1, const Vector3& p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
    z = p2.z - p1.z;
}
const Vector3& Vector3::zero()
{
    static Vector3 value(0.0f, 0.0f, 0.0f);
    return value;
}

const Vector3& Vector3::one()
{
    static Vector3 value(1.0f, 1.0f, 1.0f);
    return value;
}
const Vector3& Vector3::unitX()
{
    static Vector3 value(1.0f, 0.0f, 0.0f);
    return value;
}

const Vector3& Vector3::unitY()
{
    static Vector3 value(0.0f, 1.0f, 0.0f);
    return value;
}

const Vector3& Vector3::unitZ()
{
    static Vector3 value(0.0f, 0.0f, 1.0f);
    return value;
}

bool Vector3::isZero() const
{
    return x == 0.0f && y == 0.0f && z == 0.0f;
}

bool Vector3::isOne() const
{
    return x == 1.0f && y == 1.0f && z == 1.0f;
}

float Vector3::angle(const Vector3& v1, const Vector3& v2)
{
    //    Func one
    //    float dx = v1.y * v2.z - v1.z * v2.y;
    //    float dy = v1.z * v2.x - v1.x * v2.z;
    //    float dz = v1.x * v2.y - v1.y * v2.x;
    //
    //    return atan2f(sqrt(dx * dx + dy * dy + dz * dz) + MATH_EPSILON, dot(v1, v2));

    /// Func tow
    float angle = dot(v1, v2);
    angle /= (v1.length() - v2.length());
    return angle = acosf(angle);
}

void Vector3::add(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vector3::add(const Vector3& v1, const Vector3& v2, Vector3* dst)
{
    ASSERT(dst);

    dst->x = v1.x + v2.x;
    dst->y = v1.y + v2.y;
    dst->z = v1.z + v2.z;
}

void Vector3::clamp(const Vector3& min, const Vector3& max)
{
    clamp(min, max, this);
}

void Vector3::clamp(const Vector3& min, const Vector3& max, Vector3* dst)
{
    ASSERT(dst);
    ASSERT(!(min.x > max.x || min.y > max.y || min.z > max.z));
    std::clamp(dst->x, min.x, max.x);
    std::clamp(dst->y, min.y, max.y);
    std::clamp(dst->z, min.z, max.z);
}

float Vector3::distance(const Vector3& v) const
{
    return sqrt(distanceSquard(v));
}

float Vector3::distanceSquard(const Vector3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return (dx * dx + dy * dy + dz * dz);
}

void Vector3::negate()
{
    x = -x;
    y = -y;
    z = -z;
}

void Vector3::scale(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

void Vector3::smooth(const Vector3& target, float elapsedTime, float responseTime)
{
    if (elapsedTime > 0)
    {
        *this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
    }
}

Vector3 Vector3::projection(const Vector3& a, const Vector3& b)
{
    Vector3 vec = b / b.length();
    return vec * dot(a, vec);
}

bool notSameSide(Vector3& p1, Vector3 p2, Vector3 a, Vector3 b)
{
    Vector3 cp1, cp2;
    Vector3::cross(b - a, p1 - a, &cp1);
    Vector3::cross(b - a, p2 - a, &cp2);
    /// 判断两个叉积结果的方向是否同向，使用点乘的方式
    return Vector3::dot(cp1, cp2) >= 0;
}

/// 判断点是否在三角形内部
bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3)
{
    // 测试点是否在三角形所在平面,具体原理如下：如果一个点在三角形内部，则点必然在AB下面， BC左面，AC右边。
    return notSameSide(point, tri1, tri2, tri3) && notSameSide(point, tri2, tri1, tri3) && notSameSide(point, tri3, tri1, tri2);
}