//
// Created by william on 2022/4/5.
//

#ifndef CPP_DEMO_HELLOTRIANGLEEXEREISE2_H
#define CPP_DEMO_HELLOTRIANGLEEXEREISE2_H
#include "commonInterface.h"

#include <array>

namespace graphicEngine::gl
{

class HelloTriangleExereise2 : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~HelloTriangleExereise2() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
    std::array<float, 9> m_verticesTriangle1 = {
        // first triangle
        -0.9f, -0.5f, 0.0f, // left
        -0.0f, -0.5f, 0.0f, // right
        -0.45f, 0.5f, 0.0f, // top
    };

    std::array<float, 9> m_verticesTriangle2 = {
        // second triangle
        0.0f, -0.5f, 0.0f, // left
        0.9f, -0.5f, 0.0f, // right
        0.45f, 0.5f, 0.0f  // top
    };
    GLuint m_vbos[2];
    GLuint m_vaos[2];
};
} // namespace graphicEngine::gl
#endif //CPP_DEMO_HELLOTRIANGLEEXEREISE2_H
