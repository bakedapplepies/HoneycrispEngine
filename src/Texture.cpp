#include "pch/pch.h"

#include "Debug.h"
#include "Texture.h"


GLuint Texture::sm_textureUnitCounter = 0;
std::unordered_map<GLuint, GLint> Texture::sm_textureUnits;  // key: ID -> texture unit
std::vector<Texture*> Texture::s_textureRefs;

Texture::Texture()
{
    s_textureRefs.push_back(this);
}

Texture::Texture(const char* texturePath, uint32_t textureResolutionWidth, uint32_t textureResolutionHeight)
    : m_textureResolutionWidth(textureResolutionWidth), m_textureResolutionHeight(textureResolutionHeight)
{
    GLCall(glGenTextures(1, &m_textureID));

    sm_textureUnits[m_textureID] = sm_textureUnitCounter;
    sm_textureUnitCounter++;

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

        // Generate texture | bind -> buffer -> mimmap -> config
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
        Debug::Error("Texture failed to load.");
        Debug::Error(stbi_failure_reason());
    }

    stbi_image_free(data);

    GenerateTextureCoords();
}

Texture::Texture(Texture&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureResolutionWidth = other.m_textureResolutionWidth;
    m_textureResolutionHeight = other.m_textureResolutionHeight;

    m_textureCoords = std::move(other.m_textureCoords);

    m_textureID = other.m_textureID;
    other.m_textureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureResolutionWidth = other.m_textureResolutionWidth;
    m_textureResolutionHeight = other.m_textureResolutionHeight;

    m_textureCoords = std::move(other.m_textureCoords);

    m_textureID = other.m_textureID;
    other.m_textureID = 0;

    return *this;
}

Texture::~Texture()
{
    if (m_textureID)
    {
        GLCall(glDeleteTextures(1, &m_textureID));
    }
}

GLuint Texture::getID() const
{
    return m_textureID;
}

GLint Texture::getTextureUnit() const
{
    return sm_textureUnits[m_textureID];
}

void Texture::Bind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + sm_textureUnits[m_textureID]));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture::Unbind() const  // could be static
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Delete()
{
    GLCall(glDeleteTextures(1, &m_textureID));
    m_textureID = 0;  // so OpenGL deletes 0 when exiting stack
    sm_textureUnitCounter--;
}

/* texture maps */
Texture Textures::mainTextureMap;
Texture Textures::mainTextureSpecularMap;
void Texture::LoadTextures()
{
    Textures::mainTextureMap = Texture(
        "../resources/textures/grass_textures.png",
        16, 16
    );
    Textures::mainTextureSpecularMap = Texture(
        "../resources/textures/grass_textures_specular_map.png",
        16, 16
    );
}

void Texture::DeleteAllTextures()
{
    for (Texture* texture : s_textureRefs)
    {
        texture->Delete();
    }
}

void Texture::GenerateTextureCoords()
{
    int nRow_textures = m_pixelHeight / m_textureResolutionHeight;
    int nCol_textures = m_pixelWidth / m_textureResolutionWidth;
    m_textureCoords.reserve(nRow_textures);

    // Reminder: OpenGL has (0, 0) at Bottom left
    for (int row = 0; row < nRow_textures; row++)
    {
        m_textureCoords.push_back(std::vector<TextureCoords>(nCol_textures));
        for (int col = 0; col < nCol_textures; col++)
        {
            float topRow = (float)(row)/(nRow_textures);
            topRow += (topRow - 0.5f) * -2.0f;
            float bottomRow = (float)(row + 1)/(nRow_textures);
            bottomRow += (bottomRow - 0.5f) * -2.0f;
            
            m_textureCoords[row][col] = {
                glm::vec2((float)(col    )/(nCol_textures), topRow),  // tl
                glm::vec2((float)(col + 1)/(nCol_textures), topRow),  // tr
                glm::vec2((float)(col    )/(nCol_textures), bottomRow),  // bl
                glm::vec2((float)(col + 1)/(nCol_textures), bottomRow)   // br
            };
        }
    }
}

TextureCoords& Texture::GetTextureCoords(uint32_t x, uint32_t y)
{
    return m_textureCoords[y][x];
}