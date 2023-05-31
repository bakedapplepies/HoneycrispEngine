#include "pch/pch.h"

#include "Debug.h"
#include "Texture.h"


GLint Texture::sm_textureUnitCounter = GL_TEXTURE0;
std::unordered_map<GLuint, GLint> Texture::sm_textureUnits;  // key: ID -> texture unit

Texture::Texture(const char* texturePath, NumTexturesInMap numTotalTextures)
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
        std::cout << "Texture failed to load." << '\n';
        std::cout << stbi_failure_reason() << '\n';
    }

    stbi_image_free(data);

    GenerateTextureCoords(numTotalTextures);
}

Texture::Texture(Texture&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureResolution = other.m_textureResolution;

    m_textureCoords = std::move(other.m_textureCoords);

    m_textureID = other.m_textureID;
    other.m_textureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureResolution = other.m_textureResolution;

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

// subtract by GL_TEXTURE0 to get uniform index
GLint Texture::getTextureUnit() const
{
    return sm_textureUnits[m_textureID];
}

void Texture::Bind() const
{
    GLCall(glActiveTexture(sm_textureUnits[m_textureID]));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Delete()
{
    GLCall(glDeleteTextures(1, &m_textureID));
    m_textureID = 0;
}

/* texture maps */

Texture Texture::s_mainTextureMap;
Texture Texture::s_mainTextureSpecularMap;
void Texture::LoadTextures()
{
    s_mainTextureMap = Texture(
        "../resources/textures/grass_textures.png",
        NumTexturesInMap::MainTextureMap
    );
    s_mainTextureSpecularMap = Texture(
        "../resources/textures/grass_textures_specular_map.png",
        NumTexturesInMap::MainTextureMap
    );
}

// Note: Automate this, maybe create a hashmap or sth
void Texture::UnloadTextures()
{
    s_mainTextureMap.Delete();
    s_mainTextureSpecularMap.Delete();
}

void Texture::GenerateTextureCoords(NumTexturesInMap numTotalTextures)
{
    int nRow_textures = m_pixelHeight / m_textureResolution;
    int nCol_textures = m_pixelWidth / m_textureResolution;

    unsigned int textureType = 0;

    for (int row = nRow_textures; row > 0; row++)
    {
        for (int col = 0; col < nCol_textures; col++)
        {
            m_textureCoords[textureType] = {
                glm::vec2((float)(col    )/(nCol_textures), (float)(row    )/(nRow_textures)),
                glm::vec2((float)(col + 1)/(nCol_textures), (float)(row    )/(nRow_textures)),
                glm::vec2((float)(col    )/(nCol_textures), (float)(row - 1)/(nRow_textures)),
                glm::vec2((float)(col + 1)/(nCol_textures), (float)(row - 1)/(nRow_textures))
            };
            textureType++;
            if (textureType >= numTotalTextures)  // not gonna be greater but whatever
            {
                // Debug::Get().Log("TEXTURE MAP INPUT DONE.");
                return;
            }
        }
    }
}

TextureCoords& Texture::GetTextureCoords(unsigned int textureType)
{
    return m_textureCoords[textureType];
}