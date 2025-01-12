#include "api/graphics/TextureAtlas.h"


TextureAtlas::TextureAtlas(uint32_t width, uint32_t height)
    : m_atlasWidth(width), m_atlasHeight(height)
{
    m_quadUVs.reserve(m_atlasWidth * m_atlasHeight);

    // Reminder: OpenGL has (0, 0) at Bottom left
    for (unsigned int row = 0; row < height; row++)
    {
        for (unsigned int col = 0; col < width; col++)
        {
            float topRow = 1.0f - (float)(row)/(height);         // flipping y-coords
            float bottomRow = 1.0f - (float)(row + 1)/(m_atlasHeight);
            
            m_quadUVs.push_back({
                glm::vec2((float)(col    )/(width), topRow),     // tl
                glm::vec2((float)(col + 1)/(width), topRow),     // tr
                glm::vec2((float)(col    )/(width), bottomRow),  // bl
                glm::vec2((float)(col + 1)/(width), bottomRow)   // br
            });
        }
    }
}

QuadUV& TextureAtlas::GetQuadUVs(uint32_t x, uint32_t y)
{
    assert(0 <= x && x <= m_atlasWidth - 1 && 0 <= y && y <= m_atlasHeight - 1 && "Invalid UV coordinate.");
    return m_quadUVs[y * m_atlasWidth + x];
}

TextureAtlas::TextureAtlas(TextureAtlas&& other) noexcept
{
    m_atlasWidth = other.m_atlasWidth;
    other.m_atlasWidth = 0;
    m_atlasHeight = other.m_atlasHeight;
    other.m_atlasHeight = 0;

    m_quadUVs = std::move(other.m_quadUVs);
}

TextureAtlas& TextureAtlas::operator=(TextureAtlas&& other) noexcept
{
    m_atlasWidth = other.m_atlasWidth;
    other.m_atlasWidth = 0;
    m_atlasHeight = other.m_atlasHeight;
    other.m_atlasHeight = 0;

    m_quadUVs = std::move(other.m_quadUVs);

    return *this;
}