#include "api/graphics/core/Shader.h"

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
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];  // or 1024

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::filesystem::path errorPath = std::filesystem::relative(vertexFile.string(), HNCRSP_PROJECT_DIR);
        HNCRSP_ERROR("Vertex Shader compilation failed ~{}:\n\t{}", errorPath.string(), infoLog);
        
        glDeleteShader(vertexShader);
        HNCRSP_TERMINATE("Vertex Shader compilation error.");
    }

    /* Fragment Shader */
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::filesystem::path errorPath = std::filesystem::relative(fragmentFile.string(), HNCRSP_PROJECT_DIR);
        HNCRSP_ERROR("Fragment Shader compilation failed ~{}:\n\t{}", errorPath.string(), infoLog);
        
        glDeleteShader(fragmentShader);
        HNCRSP_TERMINATE("Fragment Shader compilation error.");
    }

    
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    if (geometryShaderSource.length() > 0)
    {
        glShaderSource(geometryShader, 1, &gsSource, NULL);
        glCompileShader(geometryShader);
    
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
            std::filesystem::path errorPath = std::filesystem::relative(geometryFile.string(), HNCRSP_PROJECT_DIR);
            HNCRSP_ERROR("Geometry Shader compilation failed ~{}:\n\t{}", errorPath.string(), infoLog);

            glDeleteShader(fragmentShader);
            HNCRSP_TERMINATE("Geometry Shader compilation error.");
        }
    }

    /* Creating Shader Program */
    m_shaderID = glCreateProgram();
    glAttachShader(m_shaderID, vertexShader);
    glAttachShader(m_shaderID, fragmentShader);
    if (geometryShaderSource.length() > 0) { glAttachShader(m_shaderID, geometryShader); }

    glLinkProgram(m_shaderID);
    
    glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success);
    if (!success)
    {
        
        glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog);
        HNCRSP_ERROR("Shader Program Linking failed:\n\t", infoLog);

        
        glDeleteProgram(m_shaderID);
        m_shaderID = 0;

        HNCRSP_ERROR("Shader in program:\n\t{}\n\t{}\n\t{}", vertexFile.file_name(), fragmentFile.file_name(), geometryFile.file_name());

        HNCRSP_TERMINATE("Shader linking error.");
    }
    
    glValidateProgram(m_shaderID);
    
    glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_shaderID, 512, NULL, infoLog);
        HNCRSP_FATAL("Shader Program Validation failed:\n\t", infoLog);

        glDeleteProgram(m_shaderID);
        m_shaderID = 0;

        HNCRSP_FATAL("Shader in program:\n\t{}\n\t{}\n\t{}", vertexFile.string(), fragmentFile.string(), geometryFile.string());

        HNCRSP_TERMINATE("Shader validation error.");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    HNCRSP_INFO("\n\t{}\n\t{}\n\t{}\n\t{}", vertexFile.file_name(), fragmentFile.file_name(), geometryFile.file_name(), m_shaderID);
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

    glDeleteProgram(m_shaderID);
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
    if (location == -1) HNCRSP_INFO("{}\t{}", name, m_shaderID);
    m_uniformLocationCache[name] = location;
    return location;
}

HNCRSP_NAMESPACE_END