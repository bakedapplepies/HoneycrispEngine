#pragma once

#include "src/pch/pch.h"
#include "src/opengl/Shader.h"
#include "src/ecs/System.h"
#include "src/components/Transform.h"


HNCRSP_NAMESPACE_START

class Renderer : public System
{
private:
    bool m_startedUp = false;
    std::shared_ptr<Shader> m_basicShader;

public:
    Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;
    ~Renderer() = default;

    void StartUp();
    void Render() const;
    void SwitchSceneToRender();

private:
    glm::mat4 GetModelMatrix(Transform& transform) const;
};

HNCRSP_NAMESPACE_END