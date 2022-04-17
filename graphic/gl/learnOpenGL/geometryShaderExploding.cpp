//
// Created by cwb on 2022/4/13.
//

#include "geometryShaderExploding.h"
namespace graphicEngine::gl
{

GeometryShaderExploding::~GeometryShaderExploding() = default;

void GeometryShaderExploding::initPrograms()
{
    m_program = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/geometry2.vert"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/geometry2.frag"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/geometry2.gs.glsl"));
    m_program->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, {0.0f, 0.0f, -10.0f});
    m_program->setMatrix4("model", model);

}

void GeometryShaderExploding::initModel()
{
    m_model = std::make_unique<Model>(GET_CURRENT("/resources/objects/nanosuit/nanosuit.obj"));
}

void GeometryShaderExploding::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    m_program->use();
    m_program->setFloat("time", m_elapseTime);
}

void GeometryShaderExploding::render()
{
    CHECK_GL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    m_model->render(m_program);
}


} // namespace graphicEngine::gl