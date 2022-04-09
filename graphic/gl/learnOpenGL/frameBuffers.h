//
// Created by william on 2022/4/9.
//

#ifndef CPP_DEMO_FRAMEBUFFERS_H
#define CPP_DEMO_FRAMEBUFFERS_H
#include "depthTest.h"
class TextureGL;

namespace graphicEngine::gl
{
class FrameBuffers : public DepthTest
{
public:
    ~FrameBuffers() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;

protected:
    void initVertices() override;
    void initFramebuffer();
    void initTextures() override;
    void initPrograms() override;
    void initVertexAttrib() override;
protected:
    void drawFrameBuffersScreen();

private:
    std::unique_ptr<graphicEngine::Program> m_quadProgram;
    std::vector<float> m_quadVertices;
    GLuint m_quadVao;
    GLuint m_quadVbo;
    GLuint m_fbo = 0;
    GLuint m_rbo = 0;
    GLuint m_textureID = 0;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_FRAMEBUFFERS_H
