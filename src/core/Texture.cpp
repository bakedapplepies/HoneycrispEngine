#include "../utils/Debug.h"
#include "Texture.h"


GLuint Texture2D::sm_textureUnitCounter = 0;
std::unordered_map<std::string, TextureInfo> Texture2D::sm_initiatedTextures;
std::unordered_map<GLuint, GLint> Texture2D::sm_textureUnits;  // key: ID -> texture unit
std::unordered_map<GLuint, unsigned int> Texture2D::sm_textureIDCount;

Texture2D::Texture2D(const FileSystem::Path& texturePath, uint32_t textureResolutionWidth, uint32_t textureResolutionHeight,
    ETextureType textureType
) : m_textureWidth(textureResolutionWidth), m_textureHeight(textureResolutionHeight)
{
    // Debug::Log(fmt::format("{}", std::filesystem::last_write_time(path).time_since_epoch().count()));

    if (sm_initiatedTextures[texturePath.path].id > 0)
    {
        GLuint id = sm_initiatedTextures[texturePath.path].id;
        if (id)
        {
            m_textureID = id;
            m_textureCoords = sm_initiatedTextures[texturePath.path].textureCoords;
            m_textureType = textureType;

            sm_textureIDCount[m_textureID]++;

            return;
        }
    }

    GLCall(glGenTextures(1, &m_textureID));
    sm_initiatedTextures[texturePath.path].id = m_textureID;
    m_textureType = textureType;

    sm_textureUnits[m_textureID] = sm_textureUnitCounter;
    sm_textureUnitCounter = (sm_textureUnitCounter + 1) % 32;
    sm_textureIDCount[m_textureID]++;

    int nrChannels;
    int desiredChannels = 0;
    unsigned char* data = stbi_load(
        texturePath.path.c_str(), &m_pixelWidth, &m_pixelHeight, &nrChannels, desiredChannels);
    
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
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pixelWidth, m_pixelHeight, 0, format, GL_UNSIGNED_BYTE, data));
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
        Debug::Error(texturePath.path);
        Debug::Error(fmt::format("Texture failed to load: {}", stbi_failure_reason()));
        stbi_image_free(data);
        glfwTerminate();
        assert(!"Texture failed to load.");
    }

    GenerateTextureCoords();  // for quads

    Bind();
}

Texture2D::Texture2D(const Texture2D& other)
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureWidth = other.m_textureWidth;
    m_textureHeight = other.m_textureHeight;
    m_textureType = other.m_textureType;

    path = other.path;
    m_textureID = other.m_textureID;
    sm_textureIDCount[m_textureID]++;
    m_textureCoords = sm_initiatedTextures[other.path].textureCoords;
}

Texture2D& Texture2D::operator=(const Texture2D& other)
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureWidth = other.m_textureWidth;
    m_textureHeight = other.m_textureHeight;
    m_textureType = other.m_textureType;

    path = other.path;
    m_textureID = sm_initiatedTextures[other.path].id;
    sm_textureIDCount[m_textureID]++;
    m_textureCoords = sm_initiatedTextures[other.path].textureCoords;

    return *this;
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureWidth = other.m_textureWidth;
    m_textureHeight = other.m_textureHeight;
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
    m_textureWidth = other.m_textureWidth;
    m_textureHeight = other.m_textureHeight;
    m_textureType = other.m_textureType;

    m_textureCoords = std::move(other.m_textureCoords);
    path = other.path;

    m_textureID = other.m_textureID;
    other.m_textureID = 0;

    return *this;
}

Texture2D::~Texture2D()
{
    Delete();
}

GLuint Texture2D::getID() const
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
    GLCall(glActiveTexture(GL_TEXTURE0 + sm_textureUnits[m_textureID]));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture2D::Unbind() const  // could be static
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::Delete()
{
    if (--sm_textureIDCount[m_textureID] == 0)  // only all textures with the same ID are gone then do this
    {
        if (m_textureID) GLCall(glDeleteTextures(1, &m_textureID));
        m_textureID = 0;
        sm_textureUnitCounter--;
    }
}

/* texture maps */
Texture2D Textures::mainTextureMap;
Texture2D Textures::mainTextureSpecularMap;
void Texture2D::LoadTextures()
{
    Textures::mainTextureMap = Texture2D(
        FileSystem::Path("resources/textures/grass_textures.png"),
        3, 1,
        ETextureType::DIFFUSE
    );
    Textures::mainTextureSpecularMap = Texture2D(
        FileSystem::Path("resources/textures/grass_textures_specular_map.png"),
        3, 1,
        ETextureType::SPECULAR
    );
}

// we are storing textures in static lists so that must be deleted before going out of context
void Texture2D::DeleteAllTextures()  // static
{
    for (auto iter = sm_textureIDCount.begin(); iter != sm_textureIDCount.end(); iter++)
    {
        iter->second = 0;
        GLCall(glDeleteTextures(1, &iter->first));
        sm_textureUnitCounter--;
    }
    Debug::Log("Deleted all textures.");
}

void Texture2D::GenerateTextureCoords()
{
    m_textureCoords.reserve(m_textureHeight);

    // Reminder: OpenGL has (0, 0) at Bottom left
    for (int row = 0; row < m_textureHeight; row++)
    {
        m_textureCoords.push_back(std::vector<TextureCoords>(m_textureWidth));
        for (int col = 0; col < m_textureWidth; col++)
        {
            float topRow = 1.0f - (float)(row)/(m_textureHeight);         // flipping y-coords
            float bottomRow = 1.0f - (float)(row + 1)/(m_textureHeight);
            
            m_textureCoords[row][col] = {
                glm::vec2((float)(col    )/(m_textureWidth), topRow),     // tl
                glm::vec2((float)(col + 1)/(m_textureWidth), topRow),     // tr
                glm::vec2((float)(col    )/(m_textureWidth), bottomRow),  // bl
                glm::vec2((float)(col + 1)/(m_textureWidth), bottomRow)   // br
            };
        }
    }

    sm_initiatedTextures[path].textureCoords = m_textureCoords;
}

TextureCoords& Texture2D::GetTextureCoords(uint32_t x, uint32_t y)
{
    return m_textureCoords[y][x];
}