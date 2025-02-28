#pragma once

#include "api/pch/hncrsp_pch.h"
#include "api/graphics/PointLight.h"
#include "api/graphics/SpotLight.h"

#include "api/scene/Scene.h"
#include "api/graphics/Cube.h"
#include "api/graphics/Mesh.h"
#include "api/graphics/Model.h"

template <typename T>
using RenderObj = std::unique_ptr<Honeycrisp::SceneRenderObj<T>>;

class DefaultSceneTwo : public Honeycrisp::Scene
{
private:
    // scene objects
    const Honeycrisp::Shader* phongShader;
    const Honeycrisp::Shader* phongWTintShader;
    const Honeycrisp::Shader* normalShader;
    RenderObj<Honeycrisp::Cube> cube;
    RenderObj<Honeycrisp::Mesh> mesh;
    RenderObj<Honeycrisp::Model> backpackModel;
    RenderObj<Honeycrisp::Model> appleModel;
    RenderObj<Honeycrisp::Model> appleModelNormal;
    RenderObj<Honeycrisp::Model> sponza;
    RenderObj<Honeycrisp::Model> sponzaNormal;
    Honeycrisp::PointLight* pointLight;
    Honeycrisp::DirectionalLight* dirLight;
    Honeycrisp::SpotLight* spotLight;

    float m_u_normal_length = 0.7f;
    
    // components
    Honeycrisp::Transform* cubeTransform = nullptr;

public:
    DefaultSceneTwo(const char* scene_name);
    ~DefaultSceneTwo() = default;
    void OnUpdate(const float& dt) override;
    void OnImGui(void) override;

private:
    void InitializeShaders(void);
    void SetInitialUniforms(void);

    void _CreateCube(void);
    void _CreatePlane(void);
    void _CreateModels(void);
};