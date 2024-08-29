#include "Shader.h"


HNCRSP_NAMESPACE_START

Shader::Shader(
    const FileSystem::Path& vertexFile,
    const FileSystem::Path& fragmentFile,
    const FileSystem::Path& geometryFile
) {
    const std::string vertexShaderSource = _ParseShader(vertexFile.string());
    const std::string fragmentShaderSource = _ParseShader(fragmentFile.string());
    const std::string geometryShaderSource = geometryFile.string().size() ? _ParseShader(geometryFile.string()) : "";

    const char* vsSource = vertexShaderSource.c_str();
    const char* fsSource = fragmentShaderSource.c_str();
    const char* gsSource = geometryShaderSource.c_str();

    /* Vertex Shader */
    GLCall(
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
    GLCall(
        glShaderSource(vertexShader, 1, &vsSource, NULL));
    GLCall(
        glCompileShader(vertexShader));

    int success;
    char infoLog[512];  // or 1024

    GLCall(
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GLCall(
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
        std::filesystem::path errorPath = std::filesystem::relative(vertexFile.string(), HNCRSP_PROJECT_DIR);
        HNCRSP_LOG_ERROR(fmt::format("Vertex Shader compilation failed ~{}:\n\t", errorPath.string()), infoLog);
        // HNCRSP_LOG_ERROR(fmt::format("Vertex Shader compilation failed at {}:\n\t", ), infoLog);

        GLCall(
            glDeleteShader(vertexShader));
        HNCRSP_TERMINATE("Vertex Shader compilation error.");
    }

    /* Fragment Shader */
    GLCall(
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(
        glShaderSource(fragmentShader, 1, &fsSource, NULL));
    GLCall(
        glCompileShader(fragmentShader));

    GLCall(
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GLCall(
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
        std::filesystem::path errorPath = std::filesystem::relative(fragmentFile.string(), HNCRSP_PROJECT_DIR);
        HNCRSP_LOG_ERROR(fmt::format("Fragment Shader compilation failed ~{}:\n\t", errorPath.string()), infoLog);

        GLCall(
            glDeleteShader(fragmentShader));
        HNCRSP_TERMINATE("Fragment Shader compilation error.");
    }

    GLCall(
        GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER));
    if (geometryShaderSource.length() > 0)
    {
        GLCall(
            glShaderSource(geometryShader, 1, &gsSource, NULL));
        GLCall(
            glCompileShader(geometryShader));

        GLCall(
            glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            GLCall(
                glGetShaderInfoLog(geometryShader, 512, NULL, infoLog));
            std::filesystem::path errorPath = std::filesystem::relative(geometryFile.string(), HNCRSP_PROJECT_DIR);
            HNCRSP_LOG_ERROR(fmt::format("Geometry Shader compilation failed ~{}:\n\t", errorPath.string()), infoLog);

            GLCall(
                glDeleteShader(fragmentShader));
            HNCRSP_TERMINATE("Geometry Shader compilation error.");
        }
    }

    /* Creating Shader Program */
    GLCall(
        m_shaderID = glCreateProgram();)
    GLCall(
        glAttachShader(m_shaderID, vertexShader));
    GLCall(
        glAttachShader(m_shaderID, fragmentShader));
    if (geometryShaderSource.length() > 0) { GLCall(glAttachShader(m_shaderID, geometryShader)); }

    GLCall(
        glLinkProgram(m_shaderID));
    GLCall(
        glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success));
    if (!success)
    {
        GLCall(
            glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog));
        HNCRSP_LOG_ERROR("Shader Program Linking failed:\n\t", infoLog);

        GLCall(
            glDeleteProgram(m_shaderID));
        m_shaderID = 0;

        HNCRSP_LOG_ERROR(fmt::format("Shader in program:\n\t{}\n\t{}\n\t{}", vertexFile.string(), fragmentFile.string(), geometryFile.string()));

        HNCRSP_TERMINATE("Shader linking error.");
    }
    
    GLCall(
        glValidateProgram(m_shaderID));
    GLCall(
        glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &success));
    if (!success)
    {
        GLCall(
            glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog));
        HNCRSP_LOG_ERROR("Shader Program Validation failed:\n\t", infoLog);

        GLCall(
            glDeleteProgram(m_shaderID));
        m_shaderID = 0;

        HNCRSP_LOG_ERROR(fmt::format("Shader in program:\n\t{}\n\t{}\n\t{}", vertexFile.string(), fragmentFile.string(), geometryFile.string()));

        HNCRSP_TERMINATE("Shader validation error.");
    }

    GLCall(
        glDeleteShader(vertexShader));
    GLCall(
        glDeleteShader(fragmentShader));
    GLCall(
        glDeleteShader(geometryShader));

    HNCRSP_LOG_INFO(fmt::format("{}\n{}\n{}\n\t{}", vertexFile.string(), fragmentFile.string(), geometryFile.string(), m_shaderID));
}

std::string Shader::_ParseShader(std::string_view path)
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

Shader::~Shader()
{
    HNCRSP_CHECK_RENDER_CONTEXT();

    GLCall(glDeleteProgram(m_shaderID));
}

GLuint Shader::GetID() const
{
    return m_shaderID;
}

GLint Shader::_GetUniformLocation(const std::string_view name) const
{
    // Use();
    // TODO: Supposedly a quick & dirty way, find a better way?
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())  // uniform location could be 0
    {
        return m_uniformLocationCache[name];
    }
    GLint location = glGetUniformLocation(m_shaderID, name.data());
    if (location == -1) HNCRSP_LOG_INFO(name, "\t", m_shaderID);
    m_uniformLocationCache[name] = location;
    return location;
}

HNCRSP_NAMESPACE_END