//
// Created by william on 2021/5/3.
//

#ifndef CPP_DEMO_VECTOR3_H
#define CPP_DEMO_VECTOR3_H

class Vector3
{
public:
    Vector3();
    Vector3(float x, float y, float z);
    explicit Vector3(const float* array);
    Vector3(const Vector3& p1, const Vector3& p2);
    Vector3(const Vector3& copy);
    ~Vector3();
    static const Vector3& zero();
    static const Vector3& one();
    static const Vector3& unitX();
    static const Vector3& unitY();
    static const Vector3& unitZ();
    bool isZero() const;
    bool isOne() const;
    static float angle(const Vector3& v1, const Vector3& v2);
    void add(const Vector3& v);
    static void add(const Vector3& v1, const Vector3& v2, Vector3* dst);
    void clamp(const Vector3& min, const Vector3& max);
    static void clamp(const Vector3& min, const Vector3& max, Vector3* dst);
    void cross(const Vector3& v);
    static void cross(const Vector3& v1, const Vector3& v2, Vector3* dst);
    float distance(const Vector3& v) const;
    float distanceSquard(const Vector3& v) const;
    float dot(const Vector3& v) const;
    static float dot(const Vector3& v1, const Vector3& v2);
    float length() const;
    float lengthSquared() const;
    void negate();
    Vector3& normalize();
    void normalize(Vector3* dst) const;
    void scale(float scalar);
    void set(float x, float y, float z);
    void set(const float* array);
    void set(const Vector3& v);
    void set(const Vector3& p1, const Vector3& p2);
    void subtract(const Vector3& v);
    static void subtract(const Vector3& v1, const Vector3& v2, Vector3* dst);
    void smooth(const Vector3& target, float elapsedTime, float responseTime);
    inline Vector3 operator+(const Vector3& v) const;
    inline Vector3& operator+=(const Vector3& v);
    inline Vector3 operator-(const Vector3& v) const;
    inline Vector3& operator-=(const Vector3& v);
    inline Vector3 operator-() const;
    inline Vector3 operator*(float v) const;
    inline Vector3& operator*=(const Vector3& v);
    inline Vector3 operator/(float x) const;
    inline bool operator<(const Vector3& v) const;
    inline bool operator==(const Vector3& v) const;
    inline bool operator!=(const Vector3& v) const;
    /// 计算向量a到向量b的投影
    static Vector3 projection(const Vector3& a, const Vector3& b);

public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

inline Vector3 operator*(float x, const Vector3& v);
/// 测试点p1和点p2是都在直线ab的同一边，https://www.cnblogs.com/icmzn/p/7273677.html
bool notSameSide(Vector3& p1, Vector3 p2, Vector3 a, Vector3 b);
/// 生成一个三角形的法线
inline Vector3 genTriangleNormal(Vector3 t1, Vector3 t2, Vector3 t3);
bool inTriangle(Vector3 point, Vector3 tri1, Vector3 tri2, Vector3 tri3);

#include "vector3.inl"
#endif //CPP_DEMO_VECTOR3_H
