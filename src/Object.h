#pragma once

#include "pch/pch.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


class Object
{
private:
    VertexArray m_VAO;
    Shader m_shader;
    Texture m_texture;
    glm::vec3 m_position;

public:
    virtual ~Object() {}

    virtual void Draw() const {}
    virtual glm::vec3& GetPosition() {}
    virtual glm::mat4 GetModelMatrix() const {}
    virtual Shader& GetShader() {}
};