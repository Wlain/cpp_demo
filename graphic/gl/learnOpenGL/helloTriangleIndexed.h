//
// Created by william on 2022/4/5.
//

#ifndef CPP_DEMO_HELLOTRIANGLEINDEXED_H
#define CPP_DEMO_HELLOTRIANGLEINDEXED_H
#include "gl/common/commonInterface.h"

#include <array>
namespace graphicEngine::gl
{
class HelloTriangleIndexed : public CommonInterface
{
public:
    ~HelloTriangleIndexed() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
    std::array<float, 20> m_vertices = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    std::array<short, 6> m_indices = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_HELLOTRIANGLEINDEXED_H
