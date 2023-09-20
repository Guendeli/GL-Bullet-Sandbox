#pragma once
#include <string>
#include <GL/glew.h>

class ShaderLoader
{
public:
    GLuint CreateProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName);

private:

    std::string readShader(const char* fileName);
    GLuint createShader(GLenum shaderType, std::string source, const char* shaderName);
};
