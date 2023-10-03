#include "../Debug.h"
#include "Shader.h"


std::string Shader::parseShader(const std::string& path)
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer))
    {
        Debug::Error("Shader file not found: ", path);
        assert(false);
    }

    std::ifstream file(path);
    std::string line;
    std::stringstream ss;
    while (getline(file, line)) {
        ss << line << '\n';
    }

    return ss.str();
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vs = parseShader(vertexShaderPath);
    const char* vertexShaderSrc = vs.c_str();
    std::string fs = parseShader(fragmentShaderPath);
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
        Debug::Error("Vertex Shader compilation failed:\n\t", infoLog);

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
        Debug::Error("Fragment Shader compilation failed:\n\t", infoLog);

        GLCall(glDeleteShader(fragmentShader));
        m_shaderID = 0;
    }

    /* Creating Shader Program */
    GLCall(m_shaderID = glCreateProgram();)
    GLCall(glAttachShader(m_shaderID, vertexShader));
    GLCall(glAttachShader(m_shaderID, fragmentShader));

    GLCall(glLinkProgram(m_shaderID));
    GLCall(glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success));
    if (!success)
    {
        GLCall(glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog));
        Debug::Error("Shader Program Linking failed:\n\t", infoLog);

        glDeleteProgram(m_shaderID);
        m_shaderID = 0;
    }
    
    GLCall(glValidateProgram(m_shaderID));
    GLCall(glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &success));
    if (!success)
    {
        GLCall(glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog));
        Debug::Error("Shader Program Validation failed:\n\t", infoLog);

        glDeleteProgram(m_shaderID);
        m_shaderID = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(Shader&& other) noexcept
{
    m_shaderID = other.m_shaderID;
    other.m_shaderID = 0;  // glDeleteProgram(0); will be ignored
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    m_shaderID = other.m_shaderID;
    other.m_shaderID = 0;

    return *this;
}

Shader::Shader(const Shader& other)
{

}

Shader& Shader::operator=(const Shader& other)
{
    return *this;
}

Shader::~Shader()
{
    if (m_shaderID)
    {
        GLCall(glDeleteProgram(m_shaderID));
    }
}

// template <>
// struct std::hash<Shader>
// {
//     size_t operator()(std::weak_ptr<Shader> shader) const
//     {
//         if (auto sp = shader.lock())
//         {
//             return sp->getID();
//         }
//         assert(false);
//     }
// };

void Shader::Use() const
{
    GLCall(glUseProgram(m_shaderID));
}

GLuint Shader::getID() const
{
    return m_shaderID;
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())  // uniform location could be 0
    {
        return m_uniformLocationCache[name];
    }

    GLint location = glGetUniformLocation(m_shaderID, name.c_str());
    m_uniformLocationCache[name] = location;
    return location;
}

void Shader::setIntUniform(const std::string& name, int value) const
{
    GLCall(glProgramUniform1i(m_shaderID, getUniformLocation(name), value));
}

void Shader::setFloatUniform(const std::string& name, float value) const
{
    GLCall(glProgramUniform1f(m_shaderID, getUniformLocation(name), value));
}

void Shader::setMatrix4Uniform(const std::string& name, const glm::mat4& matrix) const
{
    GLCall(glProgramUniformMatrix4fv(
        m_shaderID,
        getUniformLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    ));
}

void Shader::setMatrix3Uniform(const std::string& name, const glm::mat3& matrix) const
{
    GLCall(glProgramUniformMatrix3fv(
        m_shaderID,
        getUniformLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    ));
}

void Shader::setVector3Uniform(const std::string& name, const glm::vec3& vector) const
{
    GLCall(glProgramUniform3fv(
        m_shaderID,
        getUniformLocation(name),
        1,
        glm::value_ptr(vector)
    ));
}