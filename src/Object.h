#pragma once

#include "pch/pch.h"

#include "VertexArray.h"
#include "Texture.h"


class Object
{
private:
    VertexArray m_VAO;
    Texture m_texture;

public:
    glm::vec3 position;

public:
    virtual void Draw() const {}
    virtual glm::vec3 GetPosition() const {}
};