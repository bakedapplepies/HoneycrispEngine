#pragma once

#include "src/pch/pch.h"

#include "../Scene.h"
#include "../Cube.h"
#include "../core/Texture.h"
#include "../Model.h"
#include "../Cubemap.h"
#include "../utils/utils.h"


class DefaultSceneTwo : public Scene
{
private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> backpackShader;
    std::shared_ptr<Shader> wackyShader;
    std::shared_ptr<Shader> normalWaveShader;
    std::shared_ptr< SceneObject<Cube> > cube;
    std::shared_ptr< SceneObject<Mesh> > mesh;
    std::shared_ptr< SceneObject<Model> > model;

public:
    DefaultSceneTwo();
    ~DefaultSceneTwo();
    DefaultSceneTwo(DefaultSceneTwo&& other) noexcept = default;
    DefaultSceneTwo& operator=(DefaultSceneTwo&& other) noexcept = default;
    virtual void onUpdate();
    void InitializeShaders(void);
    void SetInitialUniforms(void);
};