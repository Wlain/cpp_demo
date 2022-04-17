//
// Created by cwb on 2022/4/7.
//

#include "transformationsExercise2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace graphicEngine::gl
{

TransformationsExercise2::~TransformationsExercise2() = default;

void TransformationsExercise2::initialize()
{
    m_title = "TransformationsExercise2";
    m_vertPath = GET_CURRENT("/resources/shaders/LearnOpenGL/transformations.vert");
    TexturesCombined::initialize();
}

void TransformationsExercise2::update(float elapseTime)
{
    m_elapseTime = elapseTime;
}

void TransformationsExercise2::resize(int width, int height)
{
}

void TransformationsExercise2::render()
{
    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, m_elapseTime, glm::vec3(0.0f, 0.0f, 1.0f));
    m_program->use();
    m_program->setMatrix4("transform", transform);
    TexturesCombined::render();
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    auto scale = std::sin(m_elapseTime);
    transform = glm::scale(transform, glm::vec3(scale, scale, scale));
    m_program->setMatrix4("transform", transform);
    CHECK_GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

} // namespace graphicEngine::gl