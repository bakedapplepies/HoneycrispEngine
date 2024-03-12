#pragma once

#include "src/pch/pch.h"
#include "Mesh.h"


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
    Cubemap(const std::vector<std::string>& faces);
    ~Cubemap() = default;
    void Draw(Shader* shader) const;
};

HNCRSP_NAMESPACE_END