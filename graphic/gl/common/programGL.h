//
// Created by william on 2020/11/16.
//

#ifndef CROSS_PLATFORM_DEMO_PROGRAM_H
#define CROSS_PLATFORM_DEMO_PROGRAM_H

#include "base.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace graphicEngine
{
class ProgramGL
{
public:
    ProgramGL() = default;
    ~ProgramGL();
    ProgramGL(const ProgramGL& s) = default;
    ProgramGL(ProgramGL&& a) = default;
    ProgramGL& operator=(const ProgramGL& s) = default;
    ProgramGL& operator=(ProgramGL&& s) = default;
    GLuint getProgram() const;
    ProgramGL(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessellationPath = nullptr);
    static GLuint CreateProgram(GLuint vertShader, GLuint fragShader, GLuint geometryShader = 0, GLuint tessShader = 0);
    static GLuint CompileShader(GLenum shaderType, const GLchar* src);
    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVector2(const std::string& name, const glm::vec2& value) const;
    void setVector2(const std::string& name, float x, float y) const;
    void setVector3(const std::string& name, const glm::vec3& value) const;
    void setVector3Arrays(const std::string& name, const std::vector<glm::vec3>& value) const;
    void setVector3(const std::string& name, float x, float y, float z) const;
    void setVector4(const std::string& name, const glm::vec4& value) const;
    void setVector4(const std::string& name, float x, float y, float z, float w) const;
    void setMatrix2(const std::string& name, const glm::mat2& mat) const;
    void setMatrix3(const std::string& name, const glm::mat3& mat) const;
    void setMatrix4(const std::string& name, const glm::mat4& mat) const;

private:
    GLuint mProgram;
};
} // namespace graphicEngine
#endif /* shader_hpp */
