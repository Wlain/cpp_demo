//
// Created by william on 2022/4/6.
//

#ifndef CPP_DEMO_TEXTURESCOMBINED_H
#define CPP_DEMO_TEXTURESCOMBINED_H

#include "commonInterface.h"

class TextureGL;

namespace graphicEngine::gl
{
class TexturesCombined : public CommonInterface
{
public:
    TexturesCombined();
    void render() override;
    ~TexturesCombined() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
protected:
    std::array<float, 36> m_vertices = {
        // positions         // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left
    };
    std::array<unsigned int, 6> m_indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    std::unique_ptr<TextureGL> m_texture;
    std::unique_ptr<TextureGL> m_texture1;
private:
    float m_elapseTime = 0.0f;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_TEXTURESCOMBINED_H
