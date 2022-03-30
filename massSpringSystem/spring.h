//
// Created by william on 2022/3/26.
//

#ifndef CPP_DEMO_SPRING_H
#define CPP_DEMO_SPRING_H
#include "mass.h"

struct Spring
{
    Spring(Mass* a, Mass* b, float _k) :
        m1(a), m2(b), k(_k), restLength(a->position.distance(b->position))
    {
    }

    float k;          // 弹性劲度系数
    float restLength; // 弹簧原长
    Mass* m1;
    Mass* m2;
};

#endif //CPP_DEMO_SPRING_H
