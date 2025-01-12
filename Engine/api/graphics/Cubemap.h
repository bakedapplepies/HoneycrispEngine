#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/core/VertexArray.h"


HNCRSP_NAMESPACE_START

class Cubemap
{
private:
    GLuint m_cubemapTextureID;
    VertexArray m_VAO;
    GLuint m_verticesCount;
    int32_t m_channels;

private:
    Cubemap(const Cubemap&) = delete;
    Cubemap& operator=(const Cubemap&) = delete;
    Cubemap(Cubemap&&) = delete;
    Cubemap& operator=(Cubemap&&) = delete;

    void _SetMesh();

public:
    Cubemap(const std::array<FileSystem::Path, 6>& faces);
    ~Cubemap();
    void Draw() const;
};

HNCRSP_NAMESPACE_END