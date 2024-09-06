#include "Texture2D.h"
#include "src/utils/Timer.h"
#include "src/serialize/ImageSerializer.h"


HNCRSP_NAMESPACE_START

Texture2D::Texture2D(const FileSystem::Path& texturePath, ETextureType textureType)
{
    GLCall(glGenTextures(1, &m_textureID));
    m_textureType = textureType;

    int width, height;

    ImageSerializer imageSerializer;
    Serialized::Image* deserializedImage = imageSerializer.GetDeserializedObject(texturePath);
    uint8_t* data;

    // ----- Load data -----
    if (!deserializedImage)
    {
        data = stbi_load(
            texturePath.string().data(), &width, &height, &m_channels, 0);

        if (!data)
        {
            HNCRSP_LOG_ERROR(texturePath.string());
            HNCRSP_LOG_ERROR(fmt::format("Texture failed to load: {}", stbi_failure_reason()));
            stbi_image_free(data);
            HNCRSP_TERMINATE("Texture failed to load.");
        }
        imageSerializer.AddImage(data, m_channels, width, height, texturePath);
    }
    else
    {
        data = deserializedImage->mutable_image_data()->data();
        m_channels = deserializedImage->channels();
        width = deserializedImage->width();
        height = deserializedImage->height();
    }
    
    // ----- Create texture object -----
    if (data)
    {
        GLenum format;
        if (m_channels == 4)
            format = GL_RGBA;
        else if (m_channels == 3)
            format = GL_RGB;
        else if (m_channels == 1)
            format = GL_R;
        else
            HNCRSP_TERMINATE("Inappropriate number of channels for Texture2D.");

        // Generate texture | bind -> buffer -> mipmap -> config
        GLCall(
            glBindTexture(GL_TEXTURE_2D, m_textureID));
        GLCall(  // internalformat has to be sized
            glTexStorage2D(GL_TEXTURE_2D, static_cast<int>(std::log2f(std::max(width, height))) + 1, GL_SRGB8_ALPHA8, width, height));
        GLCall(
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data));

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

        if (!deserializedImage) stbi_image_free(data);
    }
    else
    {
        HNCRSP_LOG_ERROR(texturePath.string());
        HNCRSP_LOG_ERROR(fmt::format("Texture failed to load: {}", stbi_failure_reason()));
        stbi_image_free(data);
        HNCRSP_TERMINATE("Texture failed to load.");
    }
}

const GLuint& Texture2D::GetID() const
{
    return m_textureID;
}

GLuint Texture2D::GetTextureUnit() const
{
    return static_cast<GLuint>(m_textureType);
}

ETextureType Texture2D::GetTextureType() const
{
    return m_textureType;
}

int32_t Texture2D::GetChannels() const
{
    return m_channels;
}

Texture2D::~Texture2D()
{
    HNCRSP_CHECK_RENDER_CONTEXT();

    GLCall(glDeleteTextures(1, &m_textureID));
}

HNCRSP_NAMESPACE_END