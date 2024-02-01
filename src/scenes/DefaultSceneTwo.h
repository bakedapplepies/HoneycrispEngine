#pragma once

#include "src/pch/pch.h"
#include "src/PointLight.h"

#include "src/Scene.h"
#include "src/Cube.h"
#include "src/core/Texture2DManager.h"
#include "src/Model.h"
#include "src/Cubemap.h"


class DefaultSceneTwo : public Honeycrisp::Scene
{
private:
    std::shared_ptr<Honeycrisp::Shader> shader;
    std::shared_ptr<Honeycrisp::Shader> backpackShader;
    std::shared_ptr<Honeycrisp::Shader> wackyShader;
    std::shared_ptr<Honeycrisp::Shader> normalWaveShader;
    std::shared_ptr< SceneObject<Honeycrisp::Cube> > cube;
    std::shared_ptr< SceneObject<Honeycrisp::Mesh> > mesh;
    std::shared_ptr< SceneObject<Honeycrisp::Model> > model;
    std::unique_ptr<Honeycrisp::PointLight> pointLight;

public:
    DefaultSceneTwo();
    ~DefaultSceneTwo();
    DefaultSceneTwo(DefaultSceneTwo&& other) noexcept = default;
    DefaultSceneTwo& operator=(DefaultSceneTwo&& other) noexcept = default;
    void OnUpdate() override;
    void InitializeShaders(void) override;
    void SetInitialUniforms(void) override;
    void OnImGui(void) override;
};