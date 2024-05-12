#include "Texture2D.h"
#include "src/serialize/ImageSerializer.h"


HNCRSP_NAMESPACE_START

Texture2D::Texture2D(const FileSystem::Path& texturePath, ETextureType textureType)
{
    GLCall(glGenTextures(1, &m_textureID));
    m_textureType = textureType;

    int width, height, nrChannels;
    int desiredChannels = 4;

    ImageSerializer imageSerializer(desiredChannels);
    Serialized::Image* deserialized_image = imageSerializer.GetDeserializedObject(texturePath);
    uint8_t* data;

    if (!deserialized_image)
    {
        data = stbi_load(
            texturePath.string().data(), &width, &height, &nrChannels, desiredChannels);

        if (!data)
        {
            HNCRSP_LOG_ERROR(texturePath.string());
            HNCRSP_LOG_ERROR(fmt::format("Texture failed to load: {}", stbi_failure_reason()));
            stbi_image_free(data);
            HNCRSP_TERMINATE("Texture failed to load.");
        }
        imageSerializer.AddImage(data, width, height, texturePath);
    }
    else
    {
        data = deserialized_image->mutable_image_data()->data();
        width = deserialized_image->width();
        height = deserialized_image->height();
    }
    
    if (data)
    {
        GLenum format = GL_RGBA;

        // Generate texture | bind -> buffer -> mipmap -> config
        GLCall(
            glBindTexture(GL_TEXTURE_2D, m_textureID));
        GLCall(  // internalformat has to be sized
            glTexStorage2D(GL_TEXTURE_2D, 4, GL_SRGB8_ALPHA8, width, height));
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

        if (!deserialized_image) stbi_image_free(data);
    }
    else
    {
        HNCRSP_LOG_ERROR(texturePath.string());
        HNCRSP_LOG_ERROR(fmt::format("Texture failed to load: {}", stbi_failure_reason()));
        stbi_image_free(data);
        HNCRSP_TERMINATE("Texture failed to load.");
    }
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

Texture2D::~Texture2D()
{
    // TODO: Preferably use this somehow
    // HNCRSP_CHECK_RENDER_CONTEXT();

    // HNCRSP_LOG_INFO(glfwGetCurrentContext(), "   ", m_textureID);
    // GLCall(glDeleteTextures(1, &m_textureID));
}

void Texture2D::Delete()
{
    HNCRSP_CHECK_RENDER_CONTEXT();

    GLCall(glDeleteTextures(1, &m_textureID));
    m_textureID = 0;
}

HNCRSP_NAMESPACE_END