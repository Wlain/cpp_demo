//
// Created by william on 2022/4/5.
//

#ifndef CPP_DEMO_HELLOTRIANGLE_H
#define CPP_DEMO_HELLOTRIANGLE_H

#include "gl/common/commonInterface.h"

#include <array>

namespace graphicEngine::gl
{
class HelloTriangle : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~HelloTriangle() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
    std::array<float, 9> m_vertices = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_HELLOTRIANGLE_H
