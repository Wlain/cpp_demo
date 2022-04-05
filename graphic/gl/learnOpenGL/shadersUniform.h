//
// Created by william on 2022/4/5.
//

#ifndef CPP_DEMO_SHADERSUNIFORM_H
#define CPP_DEMO_SHADERSUNIFORM_H


#include "commonInterface.h"

#include <array>

namespace graphicEngine::gl
{
class ShadersUniform : public CommonInterface
{
public:
    using CommonInterface::CommonInterface;
    ~ShadersUniform() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
    std::array<float, 15> m_vertices = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_SHADERSUNIFORM_H
