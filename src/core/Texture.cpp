#include "../Debug.h"
#include "Texture.h"
#include <sys/stat.h>


GLuint Texture::sm_textureUnitCounter = 0;
std::unordered_map<std::string, Texture*> Texture::sm_initiatedTextures;
std::unordered_map<GLuint, GLint> Texture::sm_textureUnits;  // key: ID -> texture unit
std::unordered_map<GLuint, unsigned int> Texture::sm_textureIDCount;
std::vector< std::shared_ptr<Texture> > Texture::sm_textureRefs;

Texture::Texture(const std::string& texturePath, uint32_t textureResolutionWidth, uint32_t textureResolutionHeight,
    const std::source_location& location
) : m_textureWidth(textureResolutionWidth), m_textureHeight(textureResolutionHeight)
{
    sm_textureRefs.push_back(std::shared_ptr<Texture>(this));

    /* texture in absolute forms to ensure no paths are repeated */
    std::filesystem::path textureRelativePath("../");  // project root
    textureRelativePath /= std::filesystem::path(location.file_name()).remove_filename();  // where the file is
    textureRelativePath /= texturePath;  // add relative path relative to the above path <---------
                                         // in case this is absolute, it will replace everything  |
    textureRelativePath = std::filesystem::absolute(textureRelativePath);  // make absolute ------
    textureRelativePath.make_preferred();
    
    Debug::Log("hey there");
    if (sm_initiatedTextures[textureRelativePath.string()])
    {
        Debug::Log("i shouldn't be logged");
        GLuint id = sm_initiatedTextures[textureRelativePath.string()]->getID();
        if (id)
        {
            m_textureID = id;
            m_textureCoords = sm_initiatedTextures[textureRelativePath.string()]->m_textureCoords;

            return;
        }
    }

    GLCall(glGenTextures(1, &m_textureID));
    sm_initiatedTextures[textureRelativePath.string()] = this;  // there should only be one pointer

    sm_textureUnits[m_textureID] = sm_textureUnitCounter;
    sm_textureUnitCounter = (sm_textureUnitCounter + 1) % 32;
    sm_textureIDCount[m_textureID]++;

    int nrChannels;
    unsigned char* data = stbi_load(
        textureRelativePath.string().c_str(), &m_pixelWidth, &m_pixelHeight, &nrChannels, 4);
    
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
        
        path = texturePath;
    }
    else
    {
        Debug::Error(fmt::format("Texture failed to load: {}", stbi_failure_reason()));
        assert(false);
    }

    stbi_image_free(data);

    GenerateTextureCoords();
}

Texture::Texture(Texture&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureWidth = other.m_textureWidth;
    m_textureHeight = other.m_textureHeight;

    m_textureCoords = std::move(other.m_textureCoords);

    m_textureID = other.m_textureID;
    other.m_textureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    m_pixelWidth = other.m_pixelWidth;
    m_pixelHeight = other.m_pixelHeight;
    m_textureWidth = other.m_textureWidth;
    m_textureHeight = other.m_textureHeight;

    m_textureCoords = std::move(other.m_textureCoords);

    m_textureID = other.m_textureID;
    other.m_textureID = 0;

    return *this;
}

Texture::~Texture()
{
    Delete();
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
    if (--sm_textureIDCount[m_textureID] == 0)  // only all textures with the same ID are gone then do this
    {
        if (m_textureID) GLCall(glDeleteTextures(1, &m_textureID));
        m_textureID = 0;
        sm_textureUnitCounter--;
    }
}

/* texture maps */
Texture Textures::mainTextureMap;
Texture Textures::mainTextureSpecularMap;
void Texture::LoadTextures()
{
    Textures::mainTextureMap = Texture(
        "../../resources/textures/grass_textures.png",
        3, 1
    );
    Textures::mainTextureSpecularMap = Texture(
        "../../resources/textures/grass_textures_specular_map.png",
        3, 1
    );
}

// we are storing textures in static lists so that must be deleted before going out of context
void Texture::DeleteAllTextures()  // static
{
    for (std::weak_ptr<Texture> texture : sm_textureRefs)
    {
        if (std::shared_ptr<Texture> sp = texture.lock())
        {
            sp->Delete();
        }
    }
    Debug::Log("Deleted all textures.");
}

void Texture::GenerateTextureCoords()
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
}

TextureCoords& Texture::GetTextureCoords(uint32_t x, uint32_t y)
{
    return m_textureCoords[y][x];
}