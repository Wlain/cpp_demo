//
// Created by william on 2022/4/5.
//

#ifndef CPP_DEMO_ShadersInterpolation_H
#define CPP_DEMO_ShadersInterpolation_H

#include "commonInterface.h"

#include <array>

namespace graphicEngine::gl
{
class ShadersInterpolation : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~ShadersInterpolation() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
    std::array<float, 18> m_vertices = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    };
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_ShadersInterpolation_H
