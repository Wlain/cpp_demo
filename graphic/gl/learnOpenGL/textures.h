//
// Created by cwb on 2022/4/6.
//

#ifndef CPP_DEMO_TEXTURES_H
#define CPP_DEMO_TEXTURES_H

#include "commonInterface.h"
#include "textureGL.h"
#include <array>

namespace graphicEngine::gl
{
class Textures : public CommonInterface
{
public:
    Textures();
    ~Textures() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

private:
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
};
} // namespace graphicEngine::gl
#endif //CPP_DEMO_TEXTURES_H
