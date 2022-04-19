//
// Created by cwb on 2022/4/13.
//

#ifndef CPP_DEMO_SHADOWMAPPINGDEPTH_H
#define CPP_DEMO_SHADOWMAPPINGDEPTH_H
#include "depthTest.h"
namespace graphicEngine::gl
{
class ShadowMappingDepth : public DepthTest
{
public:
    ~ShadowMappingDepth() override;
    void initialize() override;
    void update(float elapseTime) override;
    void render() override;

protected:
    void initPrograms() override;
    void initVertices() override;
    void initPlaneVertexAttrib() override;
    void initCubesVertexAttrib() override;
    void drawCubes(const std::unique_ptr<ProgramGL>& program) override;
    virtual void initFbo();
    virtual void renderDebugQuad();
    virtual void renderDepthImage(const std::unique_ptr<ProgramGL>& program);

protected:
    std::unique_ptr<ProgramGL> m_quadProgram;
    std::vector<float> m_quadVertices;
    const static unsigned int s_shadowWidth = 1024;
    const static unsigned int s_shadowHeight = 1024;
    glm::vec3 m_lightPos{ -2.0f, 4.0f, -1.0f };
    glm::mat4 m_lightProjection;
    glm::mat4 m_lightView;
    glm::mat4 m_lightSpaceMatrix;
    GLuint m_depthMapFbo = 0;
    GLuint m_depthMapTexture = 0;

    GLuint m_quadVao = 0;
    GLuint m_quadVbo = 0;
    GLuint m_quadTextureID;
    float m_nearPlane = 1.0f;
    float m_farPlane = 7.5f;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_SHADOWMAPPINGDEPTH_H
