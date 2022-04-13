//
// Created by cwb on 2022/4/12.
//

#include "loadingModel.h"

#include "camera.h"

namespace graphicEngine::gl
{

LoadingModel::~LoadingModel() = default;

void LoadingModel::initialize()
{
    DepthTest::initialize();
    m_modelProgram = std::make_shared<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/modelLoading.vert"),
                                               GET_CURRENT("/resources/shaders/LearnOpenGL/modelLoading.frag"));
    m_modelProgram->use();
    m_modelProgram->setInt("textureDiffuse1", 0);
    m_modelProgram->setInt("textureSpecular1", 1);
    m_model = std::make_unique<Model>(GET_CURRENT("/resources/objects/backpack/backpack.obj"));
}

void LoadingModel::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    m_modelProgram->use();
    m_modelProgram->setMatrix4("view", m_camera->viewMatrix());
    m_modelProgram->setMatrix4("projection", m_camera->projectionMatrix(m_width, m_height, 0.1f, 100.0f));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    m_modelProgram->setMatrix4("model", model);
}

void LoadingModel::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_model->render(m_modelProgram);
}
} // namespace graphicEngine::gl