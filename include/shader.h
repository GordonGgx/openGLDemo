#pragma once

#include "glad/glad.h"
#include <string>

typedef const GLchar *FilePath;

class Shader
{
    //程序ID
    unsigned int program;

    ///编译着色器
    GLuint compileShader(GLuint type, const char *source);

    //链接着色器
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

public:
    Shader(FilePath vertexPath, FilePath fragmentPath);

    //激活程序
    void use();

    //uniform 工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};