#include "Texture2DArray.h"
#include "src/serialize/ImageSerializer.h"


HNCRSP_NAMESPACE_START

Texture2DArray::Texture2DArray(const std::vector<FileSystem::Path>& texturePaths)
{
    int mipLevelCount = 1;
    int layerCount = texturePaths.size();
    GLenum internal_format = GL_SRGB8_ALPHA8;
    GLenum format = GL_RGB;
    int width, height, nrChannels, desiredChannels = 4;

    GLCall(
        glGenTextures(1, &m_textureID));
    GLCall(
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID));
    GLCall(
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, internal_format, width, height, format));

    uint8_t* data;
    for (int i = 0; i < texturePaths.size(); i++)
    {
        ImageSerializer imageSerializer;
        Serialized::Image* deserialized_image = imageSerializer.GetDeserializedObject(texturePaths[i]);
        
        if (!deserialized_image)
        {
            data = stbi_load(texturePaths[i].string().c_str(), &width, &height, &nrChannels, desiredChannels);

            imageSerializer.AddImage(data, nrChannels, width, height, texturePaths[i]);
        }
        else
        {
            data = deserialized_image->mutable_image_data()->data();
            width = deserialized_image->width();
            height = deserialized_image->height();
        }

        if (data)
        {
            GLCall(
                glTexSubImage3D(
                    GL_TEXTURE_2D_ARRAY,
                    0,
                    0, 0,
                    i,
                    width, height,
                    texturePaths.size(),
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    data
                ));

            GLCall(
                glGenerateMipmap(GL_TEXTURE_2D_ARRAY));
            GLCall(
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
            GLCall(
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            GLCall(
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT));
            GLCall(
                glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT));
        }
        else
        {
            HNCRSP_LOG_ERROR(fmt::format("Texture failed to load: {}.", stbi_failure_reason()));
            HNCRSP_LOG_INFO(fmt::format("Texture path: {}", texturePaths[i].string()));
            stbi_image_free(data);
            HNCRSP_TERMINATE("Cubemap textures failed to load.");
        }
    }
}

HNCRSP_NAMESPACE_END