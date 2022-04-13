//
// Created by cwb on 2022/4/13.
//

#include "geometryShaderNormals.h"

#include "camera.h"

namespace graphicEngine::gl
{
void GeometryShaderNormals::initPrograms()
{
    LoadingModel::initPrograms();
    m_normalsProgram = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/geometryNormal.vert"),
                                                 GET_CURRENT("/resources/shaders/LearnOpenGL/geometryNormal.frag"),
                                                 GET_CURRENT("/resources/shaders/LearnOpenGL/geometryNormal.gs.glsl"));
    m_normalsProgram->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    m_normalsProgram->setMatrix4("model", model);
}

void GeometryShaderNormals::update(float elapseTime)
{
    LoadingModel::update(elapseTime);
    m_normalsProgram->use();
    m_normalsProgram->setMatrix4("view", m_camera->viewMatrix());
    m_normalsProgram->setMatrix4("projection", m_camera->projectionMatrix(m_width, m_height, 0.1f, 100.0f));
}

void GeometryShaderNormals::render()
{
    LoadingModel::render();
    m_normalsProgram->use();
    m_model->render(m_normalsProgram);
}

} // namespace graphicEngine::gl