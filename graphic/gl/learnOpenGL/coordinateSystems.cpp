//
// Created by cwb on 2022/4/7.
//

#include "coordinateSystems.h"
namespace graphicEngine::gl
{

void CoordinateSystems::render()
{
    TexturesCombined::render();
}

CoordinateSystems::~CoordinateSystems()
{
}

void CoordinateSystems::initialize()
{
    m_title = "CoordinateSystems";
    m_vertPath = GET_CURRENT("/resources/shaders/LearnOpenGL/transformations.vert");
    TexturesCombined::initialize();
}

void CoordinateSystems::update(float elapseTime)
{
    TexturesCombined::update(elapseTime);
}

void CoordinateSystems::resize(int width, int height)
{
    // create transformations
    auto model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    auto view = glm::mat4(1.0f);
    auto projection = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    m_program->use();
    m_program->setMatrix4("transform", projection * view * model);
}
} // namespace graphicEngine::gl