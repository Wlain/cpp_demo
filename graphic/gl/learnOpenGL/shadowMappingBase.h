//
// Created by cwb on 2022/4/19.
//

#ifndef CPP_DEMO_SHADOWMAPPINGBASE_H
#define CPP_DEMO_SHADOWMAPPINGBASE_H

#include "shadowMappingDepth.h"

namespace graphicEngine::gl
{
class ShadowMappingBase : public ShadowMappingDepth
{
public:
    ~ShadowMappingBase() override;
    void render() override;
    void update(float elapseTime) override;

protected:
    void initVertices() override;
    void initTextures() override;
    void initPrograms() override;
    void initPlaneVertexAttrib() override;
    virtual void renderScene(const std::unique_ptr<ProgramGL>& program);
    void drawLight();

protected:
    std::unique_ptr<TextureGL> m_woodTexture;
    std::unique_ptr<ProgramGL> m_programShadowBase;
    std::unique_ptr<ProgramGL> m_lightingProgram;
    std::vector<float> m_lightCubeVertices;
    glm::vec3 m_lightColor{0.0f, 0.3f, 0.3f};
    GLuint m_lightVao = 0;
    GLuint m_lightVbo = 0;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_SHADOWMAPPINGBASE_H
