#pragma once

#include "src/pch/hncrsp_pch.h"

#include "src/Scene.h"
#include "src/ecs/ECSManager.h"
#include "src/managers/ShaderManager.h"
#include "src/graphics/Cube.h"
#include "src/graphics/Mesh.h"
#include "src/graphics/Billboard.h"


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
    DefaultScene();
    ~DefaultScene() = default;
    void OnUpdate(const float& dt) override;

private:
    void SetInitialUniforms(void);
    void OnImGui(void) override;
};