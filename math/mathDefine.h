//
// Created by william on 2021/11/29.
//

#ifndef CPP_DEMO_MATHDEFINE_H
#define CPP_DEMO_MATHDEFINE_H

#include "vec3.h"
#include "vec2.h"
#include "mat4.h"
#include "vector2.h"
#include "vector3.h"
inline int getFrameBufferIndex(int i, int j, int width, int height)
{
    assert(i >= 0 && i < width && j >= 0 && j < height);
    return j * width + i;
}

#endif //CPP_DEMO_MATHDEFINE_H
