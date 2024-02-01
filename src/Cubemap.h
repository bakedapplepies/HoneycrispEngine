#pragma once

#include "src/pch/pch.h"
#include "Renderable.h"
#include "Mesh.h"


HNCRSP_NAMESPACE_START

class Cubemap : public Renderable
{
private:
    GLuint m_cubemapTextureID, m_VBO_ID, m_EBO_ID, m_VAO_ID;
    std::vector<glm::vec3> verticesPos;
    std::vector<unsigned int> indicesData;
    Mesh cubemapMesh;

private:
    Cubemap(const Cubemap&) = delete;
    Cubemap& operator=(const Cubemap&) = delete;
    Cubemap(Cubemap&&) = delete;
    Cubemap& operator=(Cubemap&&) = delete;

public:
    Cubemap(const std::vector<std::string>& faces);
    void OnUpdate() override {};
    void Draw(Shader* shader) const override;

private:
    void SetMesh();
};

HNCRSP_NAMESPACE_END