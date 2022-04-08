//
// Created by william on 2022/4/7.
//

#ifndef CPP_DEMO_MATERIALSTEXTURES_H
#define CPP_DEMO_MATERIALSTEXTURES_H

#include "colors.h"
class TextureGL;
namespace graphicEngine::gl
{
class MaterialsMapTexture : public Colors
{
public:
    using Colors::Colors;
    ~MaterialsMapTexture() override;
    void render() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void initLighting() override;
    void initCube() override;

private:
    std::unique_ptr<TextureGL> m_texture;
    std::unique_ptr<TextureGL> m_texture1;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_MATERIALSTEXTURES_H
