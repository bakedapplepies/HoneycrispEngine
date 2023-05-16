#pragma once

#include "pch/pch.h"

#include "Shader.h"


class Object
{
public:
    Shader shader;
    glm::vec3 position;

public:
    virtual ~Object() {}

    virtual void Draw() const {}
    virtual glm::vec3& GetPosition() {}
    virtual glm::mat4 GetModelMatrix() const {}
    virtual Shader& GetShader() {}
    virtual void Delete() {}
};