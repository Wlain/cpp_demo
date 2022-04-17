//
// Created by cwb on 2022/4/14.
//

#include "asteroidsInstanced.h"

#include "common/model.h"
/// 100000个rock:fps:8.2

namespace graphicEngine::gl
{
void AsteroidsInstanced::initVertexAttrib()
{
    DepthTest::initVertexAttrib();
    glGenBuffers(1, &m_rockVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_rockVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_rockAmount, &m_rockModelMatrices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // -----------------------------------------------------------------------------------------------------------------------------------
    for (unsigned int i = 0; i < m_rockModel->mesh().size(); i++)
    {
        unsigned int vao = m_rockModel->mesh()[i].m_vao;
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_rockVbo);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(10);
        glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);
        glVertexAttribDivisor(10, 1);

        glBindVertexArray(0);
    }
}

void AsteroidsInstanced::initPrograms()
{
    Asteroids::initPrograms();
    m_rockProgram = std::make_unique<ProgramGL>(GET_CURRENT("/resources/shaders/LearnOpenGL/instancing2.vert"),
                                              GET_CURRENT("/resources/shaders/LearnOpenGL/modelLoading.frag"));
}

void AsteroidsInstanced::drawRocks()
{
    m_rockProgram->use();
    m_rockModel->render(m_rockProgram, m_rockAmount);
}
} // namespace graphicEngine::gl