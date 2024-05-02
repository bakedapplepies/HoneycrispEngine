#include "Texture2D.h"
#include "src/opengl/Texture2D.h"


HNCRSP_NAMESPACE_START

Texture2D::Texture2D(const FileSystem::Path& texturePath, ETextureType textureType)
{
    GLCall(glGenTextures(1, &m_textureID));
    m_textureType = textureType;

    int nrChannels;
    int desiredChannels = 0;
    unsigned char* data = stbi_load(
        texturePath.string().data(), &m_pixelWidth, &m_pixelHeight, &nrChannels, desiredChannels);
    
    if (data)
    {
        GLenum format = 0;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        // Generate texture | bind -> buffer -> mipmap -> config
        GLCall(
            glBindTexture(GL_TEXTURE_2D, m_textureID));
        GLCall(
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_pixelWidth, m_pixelHeight, 0, format, GL_UNSIGNED_BYTE, data));
        // GLCall(
        //     glTexStorage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, m_pixelWidth, m_pixelHeight));
        // GLCall(
        //     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pixelWidth, m_pixelHeight, format, GL_UNSIGNED_BYTE, data));
        GLCall(
            glGenerateMipmap(GL_TEXTURE_2D));

        GLCall(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
        GLCall(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        // path = texturePath;
        stbi_image_free(data);
    }
    else
    {
        HNCRSP_LOG_ERROR(texturePath.string());
        HNCRSP_LOG_ERROR(fmt::format("Texture failed to load: {}", stbi_failure_reason()));
        stbi_image_free(data);
        HNCRSP_TERMINATE("Texture failed to load.");
    }
}

Texture2D::Texture2D(const Texture2D& other)
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureType = other.m_textureType;
    m_textureID = other.m_textureID;
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureType = other.m_textureType;
    m_textureID = other.m_textureID;

    return *this;
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureType = other.m_textureType;
    m_textureID = other.m_textureID;
    other.m_textureID = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureType = other.m_textureType;
    m_textureID = other.m_textureID;
    other.m_textureID = 0;

    return *this;
}

const GLuint& Texture2D::getID() const
{
    return m_textureID;
}

GLuint Texture2D::getTextureUnit() const
{
    return static_cast<GLuint>(m_textureType);
}

ETextureType Texture2D::getTextureType() const
{
    return m_textureType;
}

void Texture2D::Bind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + getTextureUnit()));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture2D::Unbind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + getTextureUnit()));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::Delete()
{
    GLCall(glDeleteTextures(1, &m_textureID));
    m_textureID = 0;
}

int Texture2D::getWidth() const
{
    return m_pixelWidth;
}

int Texture2D::getHeight() const
{
    return m_pixelHeight;
}

HNCRSP_NAMESPACE_END