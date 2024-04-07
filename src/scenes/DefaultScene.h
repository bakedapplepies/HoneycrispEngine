#pragma once

#include "src/pch/pch.h"

#include "src/Scene.h"
#include "src/ecs/ECSManager.h"
#include "src/core/ShaderManager.h"
#include "src/Cube.h"


class DefaultScene : public Honeycrisp::Scene
{
private:
    std::shared_ptr<Honeycrisp::Shader> shader;
    std::unique_ptr< SceneRenderObj<Honeycrisp::Cube> > cube;
    std::unique_ptr< SceneRenderObj<Honeycrisp::Mesh> > customMesh;

public:
    DefaultScene();
    ~DefaultScene();
    void OnUpdate(const float& dt) override;

private:
    void SetInitialUniforms(void) override;
    void OnImGui(void) override;
};