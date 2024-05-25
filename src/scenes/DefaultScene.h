#pragma once

#include "src/pch/pch.h"

#include "src/Scene.h"
#include "src/ecs/ECSManager.h"
#include "src/managers/ShaderManager.h"
#include "src/graphics/Cube.h"


class DefaultScene : public Honeycrisp::Scene
{
private:
    const Honeycrisp::Shader* shader;
    const Honeycrisp::Shader* adjustableColorShader;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Cube> > cube;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Mesh> > customMesh;
    std::shared_ptr<Honeycrisp::PointLight> pointLight;

    float m_color;

public:
    DefaultScene();
    ~DefaultScene() = default;
    void OnUpdate(const float& dt) override;

private:
    void SetInitialUniforms(void);
    void OnImGui(void) override;
};