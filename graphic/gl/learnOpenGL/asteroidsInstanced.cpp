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
    CHECK_GL(glGenBuffers(1, &m_rockVbo));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_rockVbo));
    CHECK_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_rockAmount, &m_rockModelMatrices[0], GL_STATIC_DRAW));
    CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    // -----------------------------------------------------------------------------------------------------------------------------------
    for (unsigned int i = 0; i < m_rockModel->mesh().size(); i++)
    {
        unsigned int vao = m_rockModel->mesh()[i].m_vao;
        CHECK_GL(glBindVertexArray(vao));
        CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_rockVbo));
        // set attribute pointers for matrix (4 times vec4)
        CHECK_GL(glEnableVertexAttribArray(7));
        CHECK_GL(glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0));
        CHECK_GL(glEnableVertexAttribArray(8));
        CHECK_GL(glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4))));
        CHECK_GL(glEnableVertexAttribArray(9));
        CHECK_GL(glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4))));
        CHECK_GL(glEnableVertexAttribArray(10));
        CHECK_GL(glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4))));

        CHECK_GL(glVertexAttribDivisor(7, 1));
        CHECK_GL(glVertexAttribDivisor(8, 1));
        CHECK_GL(glVertexAttribDivisor(9, 1));
        CHECK_GL(glVertexAttribDivisor(10, 1));

        CHECK_GL(glBindVertexArray(0));
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