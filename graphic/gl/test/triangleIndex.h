//
// Created by william on 2021/4/5.
//

#ifndef CROSS_PLATFORM_DEMO_TRIANGLEINDEX_H
#define CROSS_PLATFORM_DEMO_TRIANGLEINDEX_H

#include "commonInterface.h"

namespace graphicEngine::gl
{
class TriangleIndex : public CommonInterface
{
public:
    typedef struct Vertex
    {
        vec2 pos;
        vec3 col;
    } Vertex;

public:
    using CommonInterface::CommonInterface;
    ~TriangleIndex() override;

public:
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void display() override;

private:
    Vertex m_vertices[4] = {
        0.5f, 0.5f, 1.f, 0.f, 0.f,
        0.5f, -0.5f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.f, 0.f, 1.f,
        -0.5f, 0.5f, 1.f, 1.f, 1.f
    };
    unsigned int m_indices[6] = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    mat4x4 m_mvpMatrix;
    float m_ratio = 0.0f;
};

} // namespace graphicEngine::gl
#endif //CROSS_PLATFORM_DEMO_TRIANGLE_H
