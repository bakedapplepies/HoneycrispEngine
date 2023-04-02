#include "pch/pch.h"

#include "Debug.h"
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

void Shader::CreateShader(
    const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vs = ParseShader(vertexShaderPath);
    const char* vertexShaderSrc = vs.c_str();
    std::string fs = ParseShader(fragmentShaderPath);
    const char* fragmentShaderSrc = fs.c_str();

    /* Vertex Shader */
    GLCall(GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL));
    GLCall(glCompileShader(vertexShader));

    int success;
    char infoLog[512];
    GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));

    if (!success)
    {
        GLCall(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
        std::cout << "Vertex Shader compilation failed:\n\t" << infoLog << '\n';

        GLCall(glDeleteShader(vertexShader));
        m_shaderID = 0;
    }

    /* Fragment Shader */
    GLCall(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL));
    GLCall(glCompileShader(fragmentShader));

    GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GLCall(glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
        std::cout << "Fragment Shader compilation failed:\n\t" << infoLog << '\n';

        GLCall(glDeleteShader(fragmentShader));
        m_shaderID = 0;
    }

    /* Creating Shader Program */
    GLCall(m_shaderID = glCreateProgram());
    GLCall(glAttachShader(m_shaderID, vertexShader));
    GLCall(glAttachShader(m_shaderID, fragmentShader));

    GLCall(glLinkProgram(m_shaderID));
    GLCall(glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success));
    if (!success)
    {
        GLCall(glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog));
        std::cout << "Shader Program Linking failed:\n\t" << infoLog << '\n';

        glDeleteProgram(m_shaderID);
        m_shaderID = 0;
    }
    
    GLCall(glValidateProgram(m_shaderID));
    GLCall(glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &success));
    if (!success)
    {
        GLCall(glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog));
        std::cout << "Shader Program Validation failed:\n\t" << infoLog << '\n';

        glDeleteProgram(m_shaderID);
        m_shaderID = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() const
{
    GLCall(glUseProgram(m_shaderID));
}

void Shader::Delete() const
{
    GLCall(glDeleteProgram(m_shaderID));
}

GLuint Shader::getID() const
{
    return m_shaderID;
}

void Shader::setFloatUniform(const std::string& name, float value) const
{
    GLCall(GLuint uFloatLocation = glGetUniformLocation(m_shaderID, name.c_str()));
    GLCall(glUniform1f(uFloatLocation, value));
}

void Shader::setMatrixfvUniform(const std::string& name, glm::mat4 matrix) const
{
    GLCall(GLuint uMatrixLocation = glGetUniformLocation(m_shaderID, name.c_str()));
    GLCall(glUniformMatrix4fv(
        uMatrixLocation,
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    ));
}