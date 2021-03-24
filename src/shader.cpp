#include "shader.h"
#include <fstream>
#include <sstream>
#include <exception>
#include <iostream>

Shader::Shader(FilePath vertexPath, FilePath fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
#ifdef DEBUG
    std::cout << "comile " << vertexCode << std::endl;
    std::cout << "compile " << fragmentCode << std::endl;
#endif

    GLuint vShader = compileShader(GL_VERTEX_SHADER, vShaderCode);
    GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fShaderCode);
    this->program = linkProgram(vShader, fShader);
}

GLuint Shader::compileShader(GLuint type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
#ifdef DEBUG
    std::cout << "compoile:" << type << std::endl;
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return 0;
    }
#endif
    return shader;
}
GLuint Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
#ifdef DEBUG
    int success;
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);
    std::cout << "hello debug" << std::endl;
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::Program::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return 0;
    }
#endif
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void Shader::use()
{
    glUseProgram(this->program);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), value);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(this->program, name.c_str()), value);
}