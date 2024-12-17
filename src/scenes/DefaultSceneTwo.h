#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/graphics/PointLight.h"
#include "src/graphics/SpotLight.h"

#include "src/Scene.h"
#include "src/graphics/Cube.h"
#include "src/graphics/Mesh.h"
#include "src/graphics/Model.h"
#include "src/graphics/Cubemap.h"
#include "src/managers/Texture2DManager.h"


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
    DefaultSceneTwo();
    ~DefaultSceneTwo() = default;
    void OnUpdate(const float& dt) override;
    void OnImGui(void) override;

private:
    void InitializeShaders(void);
    void SetInitialUniforms(void);
};