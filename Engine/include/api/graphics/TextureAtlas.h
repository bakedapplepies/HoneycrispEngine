#pragma once

#include "api/pch/hncrsp_pch.h"


struct QuadUV
{
    glm::vec2 tl;  // top left
    glm::vec2 tr;  // top right
    glm::vec2 bl;  // bottom left
    glm::vec2 br;  // bottom right
};

// TODO: maybe enable flipping coords to get flipped UVs
class TextureAtlas
{
private:
    uint32_t m_atlasWidth;
    uint32_t m_atlasHeight;
    std::vector<QuadUV> m_quadUVs;

public:
    TextureAtlas() = default;
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;
    TextureAtlas(TextureAtlas&&) noexcept;
    TextureAtlas& operator=(TextureAtlas&&) noexcept;
    ~TextureAtlas() = default;
    TextureAtlas(uint32_t width, uint32_t height);

    QuadUV& GetQuadUVs(uint32_t x, uint32_t y);
};