#pragma once

#include "src/pch/pch.h"
#include "src/graphics/core/VertexArray.h"


HNCRSP_NAMESPACE_START

class Cubemap
{
private:
    GLuint m_cubemapTextureID;
    std::unique_ptr<VertexArray> m_VAO = nullptr;
    GLuint m_verticesCount;

private:
    Cubemap(const Cubemap&) = delete;
    Cubemap& operator=(const Cubemap&) = delete;
    Cubemap(Cubemap&&) = delete;
    Cubemap& operator=(Cubemap&&) = delete;

    void SetMesh();

public:
    Cubemap(const std::array<FileSystem::Path, 6>& faces);
    ~Cubemap();
    void Draw() const;
};

HNCRSP_NAMESPACE_END