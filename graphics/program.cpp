//
// Created by william on 2021/10/30.
//

#include "program.h"

namespace graphics
{
Program::Program()
{
    LOG_INFO("Program constructor called");
}

Program::Program(std::shared_ptr<VertexShader>& vertShader, std::shared_ptr<FragmentShader>& fragShader) :
    m_vertexShader(vertShader), m_fragmentShader(fragShader)
{
    LOG_INFO("Program(std::shared_ptr<VertexShader>& vertShader, std::shared_ptr<FragmentShader>& fragShader) called");
}

Program::~Program()
{
    LOG_INFO("Program destructor called");
}

void Program::attachVertexShader(std::shared_ptr<VertexShader>& vertShader)
{
    m_vertexShader = vertShader;
}
void Program::attachFragmentShader(std::shared_ptr<FragmentShader>& fragShader)
{
    m_fragmentShader = fragShader;
}
} // namespace graphics