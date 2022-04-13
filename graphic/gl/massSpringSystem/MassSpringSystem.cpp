//
// Created by william on 2022/3/25.
//

#include "massSpringSystem.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

#include <utility>
namespace graphicEngine::gl
{
MassSpringSystem::MassSpringSystem():
    m_config(std::move(Config()))
{
}

MassSpringSystem::~MassSpringSystem()
{
    delete m_verletRope;
    delete m_eulerRope;
}

MassSpringSystem::MassSpringSystem(Config config) :
    m_config(std::move(config))
{
}

void MassSpringSystem::initialize()
{
    m_verletRope = new Rope({ 0, 900 }, { -1200, 900 }, 10, m_config.mass, m_config.ks, { 0 });
    m_eulerRope = new Rope({ 0, 900 }, { -1200, 900 }, 10, m_config.mass, m_config.ks, { 0 });
    initWithProperty(std::make_tuple("mass spring system", GET_CURRENT("/resources/shaders/massSpring.gl.vert"), GET_CURRENT("/resources/shaders/massSpring.gl.frag")));
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), nullptr, GL_DYNAMIC_DRAW);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (void*)(sizeof(vec2)));
    glEnableVertexAttribArray(1);
}

void MassSpringSystem::update(float elapseTime)
{
}

void MassSpringSystem::resize(int width, int height)
{
    glViewport(0.0f, 0.0f, width, height);
}

void MassSpringSystem::render()
{
    for (int i = 0; i < m_config.stepsPerFrame; i++)
    {
        m_eulerRope->simulateEuler(1 / m_config.stepsPerFrame, m_config.gravity, Rope::EulerType::ExplicitImplicit);
        m_verletRope->simulateVerlet(1 / m_config.stepsPerFrame, m_config.gravity);
    }
    int index = 0;
    for (const auto& m : m_eulerRope->m_masses)
    {
        const auto& p = m->position;
        m_vertices[index++] = { { p.x / 1000.0f, p.y / 1000.0f }, { 1.f, 0.f, 0.f } };
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program->use();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
    glBindVertexArray(m_vao);
    glPointSize(5.0f);
    glDrawArrays(GL_POINTS, 0, 10);
    index = 0;
    for (const auto& m : m_verletRope->m_masses)
    {
        const auto& p = m->position;
        m_vertices[index++] = { { p.x / 1000.0f, p.y / 1000.0f }, { 0.f, 1.f, 0.f } };
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), m_vertices);
    glPointSize(3.0f);
    glDrawArrays(GL_POINTS, 0, 10);
}
} // namespace graphicEngine::gl