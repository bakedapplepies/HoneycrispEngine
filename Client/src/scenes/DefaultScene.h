#pragma once

#include "api/pch/hncrsp_pch.h"

#include "api/scene/Scene.h"
#include "api/ecs/ECSManager.h"
#include "api/managers/ShaderManager.h"
#include "api/graphics/Cube.h"
#include "api/graphics/Mesh.h"
#include "api/graphics/Billboard.h"


class DefaultScene : public Honeycrisp::Scene
{
private:
    const Honeycrisp::Shader* m_shader;
    const Honeycrisp::Shader* m_adjustableColorShader;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Cube> > m_cube;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Mesh> > m_customMesh;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Billboard> > m_billboard;
    Honeycrisp::PointLight* m_pointLight;

    float m_color;

public:
    DefaultScene(const char* scene_name);
    ~DefaultScene() = default;
    void OnUpdate(const float& dt) override;

private:
    void SetInitialUniforms(void);
    void OnImGui(void) override;
};