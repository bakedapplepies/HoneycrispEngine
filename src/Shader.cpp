#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Shader.h"


std::string Shader::ParseShader(const std::string& path)
{
    std::ifstream file(path);
    std::string line;
    std::stringstream ss;
    while (getline(file, line)) {
        ss << line << '\n';
    }

    return ss.str();
}

Shader::Shader(
    const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vs = ParseShader(vertexShaderPath);
    const char* vertexShaderSrc = vs.c_str();
    std::string fs = ParseShader(fragmentShaderPath);
    const char* fragmentShaderSrc = fs.c_str();

    /* Vertex Shader */
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex Shader compilation failed:\n\t" << infoLog << '\n';

        glDeleteShader(vertexShader);
        this->shaderID = 0;
    }

    /* Fragment Shader */
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment Shader compilation failed:\n\t" << infoLog << '\n';

        glDeleteShader(fragmentShader);
        this->shaderID = 0;
    }

    /* Creating Shader Program */
    this->shaderID = glCreateProgram();
    glAttachShader(this->shaderID, vertexShader);
    glAttachShader(this->shaderID, fragmentShader);

    glLinkProgram(this->shaderID);
    glGetProgramiv(this->shaderID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->shaderID, 512, NULL, infoLog);
        std::cout << "Shader Program Linking failed:\n\t" << infoLog << '\n';

        glDeleteProgram(this->shaderID);
        this->shaderID = 0;
    }
    
    glValidateProgram(this->shaderID);
    glGetProgramiv(this->shaderID, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->shaderID, 512, NULL, infoLog);
        std::cout << "Shader Program Validation failed:\n\t" << infoLog << '\n';

        glDeleteProgram(this->shaderID);
        this->shaderID = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::useShader() const
{
    glUseProgram(this->shaderID);
}

void Shader::deleteShader() const
{
    glDeleteProgram(this->shaderID);
}

GLuint Shader::getID() const
{
    return this->shaderID;
}

void Shader::setFloatUniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(this->shaderID, name.c_str()), value);
}