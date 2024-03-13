#include "Texture2D.h"
#include "src/opengl/Texture2D.h"


HNCRSP_NAMESPACE_START

GLuint Texture2D::sm_textureUnitCounter = 0;
std::unordered_map<std::string, TextureInfo> Texture2D::sm_initiatedTextures;
std::unordered_map<GLuint, GLint> Texture2D::sm_textureUnits;  // key: ID -> texture unit

Texture2D::Texture2D(const FileSystem::Path& texturePath, ETextureType textureType,
    uint32_t atlasWidth, uint32_t atlasHeight
) : m_atlasWidth(atlasWidth), m_atlasHeight(atlasHeight)
{
    if (sm_initiatedTextures[texturePath.string()].id > 0)
    {
        GLuint id = sm_initiatedTextures[texturePath.string()].id;
        if (id)
        {
            // setting up stored texture
            m_textureID = id;
            m_textureCoords = sm_initiatedTextures[texturePath.string()].textureCoords;
            m_textureType = textureType;

            return;
        }
    }

    GLCall(glGenTextures(1, &m_textureID));
    sm_initiatedTextures[texturePath.string()].id = m_textureID;
    m_textureType = textureType;

    sm_textureUnits[m_textureID] = sm_textureUnitCounter;
    sm_textureUnitCounter = (sm_textureUnitCounter + 1) % 32;

    int nrChannels;
    int desiredChannels = 0;
    unsigned char* data = stbi_load(
        texturePath.string().data(), &m_pixelWidth, &m_pixelHeight, &nrChannels, desiredChannels);
    
    if (data)
    {
        GLenum format = -1;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        // Generate texture | bind -> buffer -> mimmap -> config
        GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_pixelWidth, m_pixelHeight, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        
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

    GenerateTextureCoords();  // for quads
}

Texture2D::Texture2D(const Texture2D& other)
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_atlasWidth = other.m_atlasWidth;
    m_atlasHeight = other.m_atlasHeight;
    m_textureType = other.m_textureType;

    path = other.path;
    m_textureID = other.m_textureID;
    m_textureCoords = sm_initiatedTextures[other.path].textureCoords;
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_atlasWidth = other.m_atlasWidth;
    m_atlasHeight = other.m_atlasHeight;
    m_textureType = other.m_textureType;

    path = other.path;
    m_textureID = sm_initiatedTextures[other.path].id;
    m_textureCoords = sm_initiatedTextures[other.path].textureCoords;

    return *this;
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_atlasWidth = other.m_atlasWidth;
    m_atlasHeight = other.m_atlasHeight;
    m_textureType = other.m_textureType;

    m_textureCoords = std::move(other.m_textureCoords);
    path = other.path;

    m_textureID = other.m_textureID;
    other.m_textureID = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_atlasWidth = other.m_atlasWidth;
    m_atlasHeight = other.m_atlasHeight;
    m_textureType = other.m_textureType;

    m_textureCoords = std::move(other.m_textureCoords);
    path = other.path;

    m_textureID = other.m_textureID;
    other.m_textureID = 0;

    return *this;
}

Texture2D::~Texture2D()
{
    if (m_textureID == 0) return;
    Delete();
}

const GLuint& Texture2D::getID() const
{
    return m_textureID;
}

GLuint Texture2D::getTextureUnit() const
{
    return sm_textureUnits[m_textureID];
}

ETextureType Texture2D::getTextureType() const
{
    return m_textureType;
}

void Texture2D::Bind() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + sm_textureUnits[m_textureID]));  // TODO
    GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture2D::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::Delete()
{
    if (m_textureID) GLCall(glDeleteTextures(1, &m_textureID));
    m_textureID = 0;
    sm_textureUnitCounter--;
}

void Texture2D::GenerateTextureCoords()
{
    m_textureCoords.reserve(m_atlasHeight);

    // Reminder: OpenGL has (0, 0) at Bottom left
    for (unsigned int row = 0; row < (unsigned int)m_atlasHeight; row++)
    {
        m_textureCoords.push_back(std::vector<TextureCoords>(m_atlasWidth));
        for (unsigned int col = 0; col < (unsigned int)m_atlasWidth; col++)
        {
            float topRow = 1.0f - (float)(row)/(m_atlasHeight);         // flipping y-coords
            float bottomRow = 1.0f - (float)(row + 1)/(m_atlasHeight);
            
            m_textureCoords[row][col] = {
                glm::vec2((float)(col    )/(m_atlasWidth), topRow),     // tl
                glm::vec2((float)(col + 1)/(m_atlasWidth), topRow),     // tr
                glm::vec2((float)(col    )/(m_atlasWidth), bottomRow),  // bl
                glm::vec2((float)(col + 1)/(m_atlasWidth), bottomRow)   // br
            };
        }
    }

    sm_initiatedTextures[path].textureCoords = m_textureCoords;
}

TextureCoords& Texture2D::GetTextureCoords(uint32_t x, uint32_t y)
{
    return m_textureCoords[y][x];
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