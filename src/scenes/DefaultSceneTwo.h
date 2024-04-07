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
    // scene objects
    std::shared_ptr<Honeycrisp::Shader> shader;
    std::shared_ptr<Honeycrisp::Shader> backpackShader;
    std::shared_ptr<Honeycrisp::Shader> appleShader;
    std::shared_ptr<Honeycrisp::Shader> wackyShader;
    std::shared_ptr<Honeycrisp::Shader> normalShader;
    std::unique_ptr< SceneRenderObj<Honeycrisp::Cube> > cube;
    std::unique_ptr< SceneRenderObj<Honeycrisp::Mesh> > mesh;
    std::unique_ptr< SceneRenderObj<Honeycrisp::Model> > backpackModel;
    std::unique_ptr< SceneRenderObj<Honeycrisp::Model> > appleModel;
    std::unique_ptr< SceneRenderObj<Honeycrisp::Model> > appleModelNormal;
    std::unique_ptr<Honeycrisp::PointLight> pointLight;

    float m_u_normal_length = 0.7f;
    
    // components
    Transform* cubeTransform = nullptr;

public:
    DefaultSceneTwo();
    ~DefaultSceneTwo() = default;
    void OnUpdate(const float& dt) override;
    void OnImGui(void) override;

private:
    void InitializeShaders(void) override;
    void SetInitialUniforms(void) override;
};