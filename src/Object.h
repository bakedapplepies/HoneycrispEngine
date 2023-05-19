#pragma once

#include "pch/pch.h"

#include "Shader.h"


class Object
{
protected:
    Shader shader;
    glm::vec3 position;

public:
    virtual void Draw() const = 0;
    virtual glm::vec3& GetPosition() = 0;
    virtual glm::mat4 GetModelMatrix() const = 0;
    virtual Shader& GetShader() = 0;
};