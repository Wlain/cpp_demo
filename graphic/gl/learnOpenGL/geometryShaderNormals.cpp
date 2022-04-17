//
// Created by cwb on 2022/4/13.
//

#include "geometryShaderNormals.h"

#include "common/camera.h"

namespace graphicEngine::gl
{
void GeometryShaderNormals::initPrograms()
{
    LoadingModel::initPrograms();
    m_normalsProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/geometryNormal.vert"),
                                                 GET_CURRENT("/resources/shaders/LearnOpenGL/geometryNormal.frag"),
                                                 GET_CURRENT("/resources/shaders/LearnOpenGL/geometryNormal.gs.glsl"));
    m_normalsProgram->use();
    m_normalsProgram->setMatrix4("model", m_modelMatrix);
}

void GeometryShaderNormals::update(float elapseTime)
{
    LoadingModel::update(elapseTime);
    m_normalsProgram->use();
    m_normalsProgram->setMatrix4("view", m_viewMatrix);
}

void GeometryShaderNormals::render()
{
    LoadingModel::render();
    m_normalsProgram->use();
    m_model->render(m_normalsProgram);
}

void GeometryShaderNormals::resize(int width, int height)
{
    DepthTest::resize(width, height);
    m_normalsProgram->use();
    m_normalsProgram->setMatrix4("projection", m_projectionMatrix);
}

} // namespace graphicEngine::gl