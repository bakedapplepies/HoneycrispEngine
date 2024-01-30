#pragma once

#include "src/pch/pch.h"

#include "../Scene.h"
#include "../renderer/Shader.h"
#include "../Cube.h"


class DefaultScene : public Honeycrisp::Scene
{
private:
    std::shared_ptr<Honeycrisp::Shader> shader;
    std::shared_ptr< SceneObject<Honeycrisp::Cube> > cube;
    std::shared_ptr< SceneObject<Honeycrisp::Mesh> > customMesh;

public:
    DefaultScene();
    ~DefaultScene();
    DefaultScene(DefaultScene&& other) noexcept = default;
    DefaultScene& operator=(DefaultScene&& other) noexcept = default;
    void OnUpdate() override;
    void SetInitialUniforms(void) override;
    void OnImGui() const override;
};