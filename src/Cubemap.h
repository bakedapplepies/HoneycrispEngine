#pragma once

#include "pch/pch.h"
#include "Renderable.h"
#include "Mesh.h"
#include "utils/Debug.h"


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
    void OnUpdate() {};
    void Draw(std::shared_ptr<Shader> shader) const;

private:
    void AddPosition(const glm::vec3& position) {}
    void SetMesh();
};