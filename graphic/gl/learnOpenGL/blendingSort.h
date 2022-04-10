//
// Created by cwb on 2022/4/8.
//

#ifndef CPP_DEMO_BLENDINGSORT_H
#define CPP_DEMO_BLENDINGSORT_H
#include "depthTest.h"
namespace graphicEngine::gl
{
class BlendingSort : public DepthTest
{
public:
    ~BlendingSort() override;
    void update(float elapseTime) override;
    void render() override;

protected:
    void initTextures() override;
    void initVertexAttrib() override;
    void initVertices() override;
    void initGLStatus() override;

private:
    std::vector<float> m_transparentVertices;
    std::unique_ptr<TextureGL> m_texture2;
    GLuint m_transparentVao = 0;
    GLuint m_transparentVbo = 0;
    std::vector<glm::vec3> m_vegetation; /// 植被
    std::map<float, glm::vec3> m_sorted;
};
} // namespace graphicEngine::gl

#endif //CPP_DEMO_BLENDINGDISCARD_H
