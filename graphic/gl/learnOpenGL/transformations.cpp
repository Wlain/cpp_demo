//
// Created by cwb on 2022/4/7.
//

#include "transformations.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace graphicEngine::gl
{

Transformations::~Transformations() = default;

void Transformations::initialize()
{
    m_title = "Transformations";
    m_vertPath = GET_CURRENT("/resources/shaders/LearnOpenGL/transformations.vert");
    TexturesCombined::initialize();
}

void Transformations::update(float elapseTime)
{
    // create transformations
    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, elapseTime, glm::vec3(0.0f, 0.0f, 1.0f));
    m_program->use();
    m_program->setMatrix4("transform", transform);
}

void Transformations::resize(int width, int height)
{
}

void Transformations::render()
{
    TexturesCombined::render();
}

} // namespace graphicEngine::gl