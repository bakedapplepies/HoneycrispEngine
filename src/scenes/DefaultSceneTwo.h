#pragma once

#include "../pch/pch.h"

#include "../Scene.h"
#include "../types/types_index.h"
#include "../Cube.h"
#include "../core/Texture.h"
#include "../Model.h"
#include "../Cubemap.h"


class DefaultSceneTwo : public Scene
{
private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> backpackShader;
    SceneObject<Cube> cube;
    SceneObject<Mesh> mesh;
    SceneObject<Model> model;

public:
    DefaultSceneTwo();
    virtual void OnUpdate();
    void InitializeShaders(void);
    void SetInitialUniforms(void);  // TODO
};