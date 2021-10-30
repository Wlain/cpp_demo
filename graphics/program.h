//
// Created by william on 2021/10/30.
//

#ifndef CPP_DEMO_PROGRAM_H
#define CPP_DEMO_PROGRAM_H
#include "shader.h"

#include <eigen3/Eigen/Eigen>

namespace graphics
{
class Program
{
public:
    Program();
    explicit Program(std::shared_ptr<VertexShader>& vertShader, std::shared_ptr<FragmentShader>& fragmentShader);
    virtual ~Program();
    void attachVertexShader(std::shared_ptr<VertexShader>& vertShader);
    void attachFragmentShader(std::shared_ptr<FragmentShader>& fragShader);
    inline std::shared_ptr<VertexShader>& vertexShader() { return m_vertexShader; }
    inline std::shared_ptr<FragmentShader>& fragmentShader() { return m_fragmentShader; };
    inline void setVertexShaderFunc(std::function<Eigen::Vector4f(const VertexShader&)> func) { m_vertexShaderFunc = std::move(func); }
    inline void setFragmentShaderFunc(std::function<Eigen::Vector3f(const FragmentShader&)> func) { m_fragmentShaderFunc = std::move(func); }
    inline Eigen::Vector4f processVertex() { return m_vertexShaderFunc(*m_vertexShader); }
    inline Eigen::Vector3f processFragment() { return m_fragmentShaderFunc(*m_fragmentShader); }

private:
    std::shared_ptr<VertexShader> m_vertexShader;
    std::function<Eigen::Vector4f(const VertexShader&)> m_vertexShaderFunc;
    std::shared_ptr<FragmentShader> m_fragmentShader;
    std::function<Eigen::Vector3f(const FragmentShader&)> m_fragmentShaderFunc;
};
} // namespace graphics
#endif //CPP_DEMO_PROGRAM_H
