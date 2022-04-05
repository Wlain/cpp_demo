//
// Created by william on 2022/4/5.
//

#ifndef CPP_DEMO_HELLOTRIANGLEEXEREISE1_H
#define CPP_DEMO_HELLOTRIANGLEEXEREISE1_H
#include "commonInterface.h"

#include <array>

namespace graphicEngine::gl
{

class HelloTriangleExereise1 : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~HelloTriangleExereise1() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
    std::array<float, 30> m_vertices = {
        // first triangle
        -0.9f, -0.5f, 0.0f, // left
        -0.0f, -0.5f, 0.0f, // right
        -0.45f, 0.5f, 0.0f, // top
                            // second triangle
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
    };
};
} // namespace graphicEngine::gl
#endif //CPP_DEMO_HELLOTRIANGLEEXEREISE1_H
