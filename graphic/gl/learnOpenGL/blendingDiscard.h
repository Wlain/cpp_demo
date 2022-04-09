//
// Created by cwb on 2022/4/8.
//

#ifndef CPP_DEMO_BLENDINGDISCARD_H
#define CPP_DEMO_BLENDINGDISCARD_H
#include "depthTest.h"
namespace graphicEngine::gl
{
class BlendingDiscard : public DepthTest
{
public:
    ~BlendingDiscard() override;
    void initialize() override;
    void update(float elapseTime) override;
    void resize(int width, int height) override;
    void render() override;
protected:
    void initVertices() override;
    void initVertexAttrib() override;
    void initTextures() override;

private:
    std::vector<float> m_transparentVertices;
    std::unique_ptr<TextureGL> m_texture2;
    GLuint m_transparentVao = 0;
    GLuint m_transparentVbo = 0;
    std::vector<glm::vec3> m_vegetation; /// 植被
};
}

#endif //CPP_DEMO_BLENDINGDISCARD_H
