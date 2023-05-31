#pragma once

#include "pch/pch.h"

#include "Shader.h"


class Object
{
protected:
    Shader shader;
    std::vector<glm::vec3> positions;

public:
    virtual void Draw() = 0;
    virtual void AddPosition(const glm::vec3& position) = 0;
    virtual std::vector<glm::vec3>& GetPositions() = 0;
    virtual glm::mat4 GetModelMatrix(const glm::vec3& position) const = 0;
    virtual Shader& GetShader() = 0;
};