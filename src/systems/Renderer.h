#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Shader.h"
#include "src/ecs/System.h"
#include "src/components/Transform.h"
#include "src/Cubemap.h"


HNCRSP_NAMESPACE_START

struct DrawElementsIndirectCommand;

class Renderer : public System
{
private:
    std::weak_ptr<Cubemap> m_weak_currentCubemap;
    std::vector<GLuint> m_shaderIDs_Order;

public:
    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;
    ~Renderer() = default;

    void StartUp();
    void Render() const;
    void SwitchCubemap(std::weak_ptr<Cubemap> weak_cubemap);

    void AddEntityUID(EntityUID entityUID) override;

private:
    glm::mat4 GetModelMatrix(Transform& transform) const;
    void GenerateDrawCommands();
};

struct DrawElementsIndirectCommand
{
    GLuint count = 1;
    GLuint instanceCount = 1;
    GLuint firstIndex = 0;
    GLint baseVertex = 0;
    GLuint baseInstance = 0;
};

HNCRSP_NAMESPACE_END