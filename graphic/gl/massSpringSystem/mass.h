//
// Created by william on 2022/3/26.
//

#ifndef CPP_DEMO_MASS_H
#define CPP_DEMO_MASS_H
#include "vector2.h"

#include <memory>
struct Mass
{
    Mass(const Vector2& _position, float _mass, bool _pinned) :
        starPosition(_position), position(_position), lastPosition(_position), mass(_mass), pinned(_pinned)
    {
    }
    float mass = 0.0f;   // 质量
    bool pinned = false; // 是否固定

    Vector2 starPosition;
    Vector2 position;

    Vector2 lastPosition;
    Vector2 velocity; // 速度
    Vector2 forces;
};

#endif //CPP_DEMO_MASS_H
