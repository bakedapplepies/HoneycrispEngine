#include "Shader.h"


HNCRSP_NAMESPACE_START

std::string Shader::parseShader(const std::string_view& path)
{
    std::string line;
    std::stringstream ss;
    std::ifstream infile(path);
    while (std::getline(infile, line))
    {
        ss << line << '\n';
    }

    return ss.str();
}

Shader::Shader(
    const FileSystem::Path& vertexFile,
    const FileSystem::Path& fragmentFile,
    const FileSystem::Path& geometryFile)
{
    const std::string vertexShaderSource = parseShader(vertexFile.string());
    const std::string fragmentShaderSource = parseShader(fragmentFile.string());
    const std::string geometryShaderSource = geometryFile.string().size() ? parseShader(geometryFile.string()) : "";

    const char* vsSource = vertexShaderSource.c_str();
    const char* fsSource = fragmentShaderSource.c_str();
    const char* gsSource = geometryShaderSource.c_str();

    /* Vertex Shader */
    GLCall(GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertexShader, 1, &vsSource, NULL));
    GLCall(glCompileShader(vertexShader));

    int success;
    char infoLog[1024];

    GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GLCall(glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog));
        HNCRSP_LOG_ERROR(fmt::format("Vertex Shader compilation failed at {}:\n\t", vertexFile.string()), infoLog);
        // HNCRSP_LOG_ERROR(fmt::format("Vertex Shader compilation failed at {}:\n\t", ), infoLog);

        GLCall(glDeleteShader(vertexShader));
        HNCRSP_TERMINATE("Vertex Shader compilation error.");
    }

    /* Fragment Shader */
    GLCall(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragmentShader, 1, &fsSource, NULL));
    GLCall(glCompileShader(fragmentShader));

    GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GLCall(glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog));
        HNCRSP_LOG_ERROR(fmt::format("Fragment Shader compilation failed at {}:\n\t", fragmentFile.string()), infoLog);

        GLCall(glDeleteShader(fragmentShader));
        HNCRSP_TERMINATE("Fragment Shader compilation error.");
    }

    GLCall(GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER));
    if (geometryShaderSource.length() > 0)
    {
        GLCall(glShaderSource(geometryShader, 1, &gsSource, NULL));
        GLCall(glCompileShader(geometryShader));

        GLCall(glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            GLCall(glGetShaderInfoLog(geometryShader, 1024, NULL, infoLog));
            HNCRSP_LOG_ERROR(fmt::format("Geometry Shader compilation failed at {}:\n\t", geometryFile.string()), infoLog);

            GLCall(glDeleteShader(fragmentShader));
            HNCRSP_TERMINATE("Geometry Shader compilation error.");
        }
    }

    /* Creating Shader Program */
    GLCall(m_shaderID = glCreateProgram();)
    GLCall(glAttachShader(m_shaderID, vertexShader));
    GLCall(glAttachShader(m_shaderID, fragmentShader));
    if (geometryShaderSource.length() > 0) { GLCall(glAttachShader(m_shaderID, geometryShader)); }

    GLCall(glLinkProgram(m_shaderID));
    GLCall(glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success));
    if (!success)
    {
        GLCall(glGetProgramInfoLog(m_shaderID, 1024, NULL, infoLog));
        HNCRSP_LOG_ERROR("Shader Program Linking failed:\n\t", infoLog);

        glDeleteProgram(m_shaderID);
        m_shaderID = 0;

        HNCRSP_LOG_ERROR(fmt::format("Shader in program:\n\t{}\n\t{}\n\t{}", vertexFile.string(), fragmentFile.string(), geometryFile.string()));

        HNCRSP_TERMINATE("Shader linking error.");
    }
    
    GLCall(glValidateProgram(m_shaderID));
    GLCall(glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &success));
    if (!success)
    {
        GLCall(glGetProgramInfoLog(m_shaderID, 1024, NULL, infoLog));
        HNCRSP_LOG_ERROR("Shader Program Validation failed:\n\t", infoLog);

        glDeleteProgram(m_shaderID);
        m_shaderID = 0;

        HNCRSP_LOG_ERROR(fmt::format("Shader in program:\n\t{}\n\t{}\n\t{}", vertexFile.string(), fragmentFile.string(), geometryFile.string()));

        HNCRSP_TERMINATE("Shader validation error.");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
}

Shader::Shader(Shader&& other) noexcept
{
    m_shaderID = other.m_shaderID;  //        v
    other.m_shaderID = 0;  // glDeleteProgram(0); will be ignored
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    m_shaderID = other.m_shaderID;
    other.m_shaderID = 0;

    return *this;
}

Shader::~Shader()
{
    if (m_shaderID != 0)
    {
        GLCall(glDeleteProgram(m_shaderID));
    }
}

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

void Shader::setIntUnf(const std::string& name, int value) const
{
    GLCall(glProgramUniform1i(m_shaderID, getUniformLocation(name), value));
}

void Shader::setFloatUnf(const std::string& name, float value) const
{
    GLCall(glProgramUniform1f(m_shaderID, getUniformLocation(name), value));
}

void Shader::setMat4Unf(const std::string& name, const glm::mat4& matrix) const
{
    GLCall(glProgramUniformMatrix4fv(
        m_shaderID,
        getUniformLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    ));
}

void Shader::setMat3Unf(const std::string& name, const glm::mat3& matrix) const
{
    GLCall(glProgramUniformMatrix3fv(
        m_shaderID,
        getUniformLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    ));
}

void Shader::setVec3Unf(const std::string& name, const glm::vec3& vector) const
{
    GLCall(glProgramUniform3fv(
        m_shaderID,
        getUniformLocation(name),
        1,
        glm::value_ptr(vector)
    ));
}

HNCRSP_NAMESPACE_END