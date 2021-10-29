//
// Created by ytech on 2021/10/29.
//

#ifndef CPP_DEMO_BUFFER_H
#define CPP_DEMO_BUFFER_H
#include <stdint.h>

namespace graphics
{
enum class Buffers : uint32_t
{
    Color = 1,
    Depth = 2,
};

inline Buffers operator|(Buffers a, Buffers b)
{
    return Buffers((uint32_t)a | (uint32_t)b);
}

inline Buffers operator&(Buffers a, Buffers b)
{
    return Buffers((uint32_t)a & (uint32_t)b);
}

enum class Primitive : uint32_t
{
    Line,
    Triangle,
    Triangle_Line
};

struct PositionBufferHandle
{
    int posHandle = 0;
};

struct IndicesBufferHandle
{
    int indicesHandle = 0;
};

struct ColorBufferHandle
{
    int colorHandle = 0;
};

} // namespace graphics

#endif //CPP_DEMO_BUFFER_H
