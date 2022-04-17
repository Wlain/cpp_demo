//
// Created by cwb on 2022/4/14.
//

#include "asteroids.h"

#include "common/model.h"
#include "common/camera.h"

/// 100000个rock，fps是:6.7

namespace graphicEngine::gl
{
Asteroids::~Asteroids() = default;

void Asteroids::initModel()
{
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 55.0f));
    m_model = std::make_unique<Model>(GET_CURRENT("/resources/objects/planet/planet.obj"));
    m_rockModel = std::make_unique<Model>(GET_CURRENT("/resources/objects/rock/rock.obj"));
}

void Asteroids::initPrograms()
{
    LoadingModel::initPrograms();
    m_rockProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/modelLoading.vert"),
                                              GET_CURRENT("/resources/shaders/LearnOpenGL/modelLoading.frag"));
    m_rockModelMatrices = new glm::mat4[m_rockAmount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
    float radius = 50.0;
    float offset = 2.5f;
    for (unsigned int i = 0; i < m_rockAmount; ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)m_rockAmount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        m_rockModelMatrices[i] = model;
    }
}

void Asteroids::update(float elapseTime)
{
    DepthTest::update(elapseTime);
    m_rockProgram->use();
    m_rockProgram->setMatrix4("view", m_viewMatrix);
}

void Asteroids::resize(int width, int height)
{
    DepthTest::resize(width, height);
    m_rockProgram->use();
    m_rockProgram->setMatrix4("projection", m_projectionMatrix);
}

void Asteroids::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    m_program->setMatrix4("model", model);
    m_model->render(m_program);
    drawRocks();
}

void Asteroids::drawRocks()
{
    m_rockProgram->use();
    // draw meteorites
    for (unsigned int i = 0; i < m_rockAmount; i++)
    {
        m_rockProgram->setMatrix4("model", m_rockModelMatrices[i]);
        m_rockModel->render(m_rockProgram);
    }
}
} // namespace graphicEngine::gl