//
// Created by cwb on 2022/4/12.
//

#include "loadingModel.h"

#include "camera.h"

namespace graphicEngine::gl
{

LoadingModel::~LoadingModel() = default;


void LoadingModel::initModel()
{
    m_model = std::make_unique<Model>(GET_CURRENT("/resources/objects/backpack/backpack.obj"));
}

void LoadingModel::initPrograms()
{
    m_program = std::make_unique<Program>(GET_CURRENT("/resources/shaders/LearnOpenGL/modelLoading.vert"),
                                          GET_CURRENT("/resources/shaders/LearnOpenGL/modelLoading.frag"));
    m_program->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    m_program->setMatrix4("model", model);
}

void LoadingModel::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_model->render(m_program);
}
} // namespace graphicEngine::gl