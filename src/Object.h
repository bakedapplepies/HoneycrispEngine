#pragma once

#include "pch/pch.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


class Object
{
private:
    Shader m_shader;
    glm::vec3 m_position;

public:
    virtual ~Object() {}

    virtual void Draw() const {}
    virtual void BuildMeshData() {}
    virtual glm::vec3& GetPosition() {}
    virtual glm::mat4 GetModelMatrix() const {}
    virtual Shader& GetShader() {}
};