#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/PointLight.h"
#include "api/graphics/SpotLight.h"

#include "api/scene/Scene.h"
#include "api/graphics/Cube.h"
#include "api/graphics/Mesh.h"
#include "api/graphics/Model.h"

class DefaultSceneTwo : public Honeycrisp::Scene
{
private:
    // scene objects
    const Honeycrisp::Shader* phongShader;
    const Honeycrisp::Shader* phongWTintShader;
    const Honeycrisp::Shader* normalShader;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Cube> > cube;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Mesh> > mesh;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Model> > backpackModel;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Model> > appleModel;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Model> > appleModelNormal;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Model> > sponza;
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Model> > sponzaNormal;
    Honeycrisp::PointLight* pointLight;
    Honeycrisp::DirectionalLight* dirLight;
    Honeycrisp::SpotLight* spotLight;

    float m_u_normal_length = 0.7f;
    
    // components
    Transform* cubeTransform = nullptr;

public:
    DefaultSceneTwo(const char* scene_name);
    ~DefaultSceneTwo() = default;
    void OnUpdate(const float& dt) override;
    void OnImGui(void) override;

private:
    void InitializeShaders(void);
    void SetInitialUniforms(void);
};