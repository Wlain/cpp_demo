//
// Created by william on 2020/11/16.
//

#include "programGL.h"

#include "commonMacro.h"

#include <fstream>
#include <sstream>

namespace graphicEngine
{

ProgramGL::~ProgramGL()
{
    glDeleteProgram(mProgram);
}

GLuint ProgramGL::CreateProgram(GLuint vertShader, GLuint fragShader, GLuint geometryShader, GLuint tessShader)
{
    GLuint program = 0;
    GLint linked = GL_FALSE;
    program = glCreateProgram();
    if (!program)
    {
        goto exit;
    }
    // 绑定
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    if (geometryShader != 0)
        glAttachShader(program, geometryShader);
    if (tessShader != 0)
        glAttachShader(program, tessShader);
    glLinkProgram(program);
    // 解绑定
    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);
    if (geometryShader != 0)
        glDetachShader(program, geometryShader);
    if (tessShader != 0)
        glDetachShader(program, tessShader);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        printf("could not link program\n");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen)
        {
            auto infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog)
            {
                glGetProgramInfoLog(program, infoLogLen, nullptr, infoLog);
                printf("could not link program\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }
    return program;
exit:
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return program;
}

GLuint ProgramGL::CompileShader(GLenum shaderType, const GLchar* src)
{
    GLuint shader = glCreateShader(shaderType);
    if (!shader)
    {
        return 0;
    }
    glShaderSource(shader, 1, &src, nullptr);
    GLint compiled = GL_TRUE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        GLsizei infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0)
        {
            auto* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog)
            {
                glGetShaderInfoLog(shader, infoLogLen, nullptr, infoLog);
                printf("Cound not compile %s shader:\n%s\nshader Code: \n%s\n",
                       shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog, src);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

ProgramGL::ProgramGL(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tessellationPath)
{
    // 1. 从文件中读取顶点和片元的文本文件
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::string tessellationCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    std::ifstream tShaderFile;
    // 确保ifstream objects会抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    tShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
        if (tessellationPath != nullptr)
        {
            gShaderFile.open(tessellationPath);
            std::stringstream tShaderStream;
            tShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            tessellationCode = tShaderStream.str();
        }
    }
    catch (std::ifstream::failure& e)
    {
        LOG_ERROR("ERROR::Program::file not successfully read");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vShaderCode);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fShaderCode);
    GLuint geometricShader = 0;
    GLuint tessellationShader = 0;
    if (geometryPath != nullptr)
    {
        const char* gShaderCode = geometryCode.c_str();
        geometricShader = CompileShader(GL_GEOMETRY_SHADER, gShaderCode);
    }
    if (tessellationPath != nullptr)
    {
        const char* tShaderCode = tessellationCode.c_str();
        tessellationShader = CompileShader(GL_TESS_CONTROL_SHADER, tShaderCode);
    }
    mProgram = CreateProgram(vertexShader, fragmentShader, geometricShader, tessellationShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint ProgramGL::getProgram() const
{
    return mProgram;
}

void ProgramGL::use() const
{
    glUseProgram(mProgram);
}

void ProgramGL::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(mProgram, name.c_str()), (int)value);
}

void ProgramGL::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(mProgram, name.c_str()), value);
}

void ProgramGL::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(mProgram, name.c_str()), value);
}

void ProgramGL::setVector2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
}

void ProgramGL::setVector2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(mProgram, name.c_str()), x, y);
}

void ProgramGL::setVector3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
}

void ProgramGL::setVector3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(mProgram, name.c_str()), x, y, z);
}

void ProgramGL::setVector4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(mProgram, name.c_str()), 1, &value[0]);
}

void ProgramGL::setVector4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(mProgram, name.c_str()), x, y, z, w);
}

void ProgramGL::setMatrix2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ProgramGL::setMatrix3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ProgramGL::setMatrix4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ProgramGL::setVector3Arrays(const std::string& name, const std::vector<glm::vec3>& value) const
{
    glUniform3fv(glGetUniformLocation(mProgram, name.c_str()), value.size(), &value[0][0]);
}
} // namespace graphicEngine