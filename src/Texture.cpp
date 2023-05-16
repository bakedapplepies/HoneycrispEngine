#include "pch/pch.h"

#include "Debug.h"
#include "Texture.h"

void Texture::LoadTexture(const char* texturePath)
{
    GLCall(glGenTextures(1, &m_textureID));

    int nrChannels;
    unsigned char* data = stbi_load(
        texturePath, &m_pixelWidth, &m_pixelHeight, &nrChannels, 4);
    
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        // Generate texture
        GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pixelWidth, m_pixelHeight, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }
    else
    {
        std::cout << "Texture failed to load." << '\n';
        std::cout << stbi_failure_reason() << '\n';
    }

    stbi_image_free(data);

    GenerateTextureCoords();
}

GLuint Texture::getID() const
{
    return m_textureID;
}

void Texture::Bind(GLenum texture_unit) const
{
    GLCall(glActiveTexture(texture_unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Delete() const
{
    GLCall(glDeleteTextures(1, &m_textureID));
}

void Texture::GenerateTextureCoords()
{
    int nRow_textures = m_pixelHeight / m_textureResolution;
    int nCol_textures = m_pixelWidth / m_textureResolution;

    unsigned int textureType = 0;

    for (int row = nRow_textures; row >= 0; row++)
    {
        for (int col = 0; col < nCol_textures; col++)
        {
            m_textureCoords[textureType] = {
                glm::vec2(col/(nCol_textures + 1), row/(nRow_textures + 1)),
                glm::vec2((col + 1)/(nCol_textures+1), row/(nRow_textures + 1)),
                glm::vec2(col/(nCol_textures + 1), (row - 1)/(nRow_textures + 1)),
                glm::vec2((col + 1)/(nCol_textures + 1), (row - 1)/(nRow_textures + 1))
            };
            textureType++;
            if (textureType == Textures::END) return;
        }
    }
}

TextureCoords Texture::GetTextureCoords(unsigned int textureType) const
{
    
}