#pragma once

#include "../pch/pch.h"

#include "../Scene.h"
#include "../types/types_index.h"
#include "../Cube.h"
#include "../core/Texture.h"
#include "../Model.h"


class DefaultSceneTwo : public Scene
{
private:
    std::shared_ptr<Shader> shader;
    SceneObject<Cube> cube;
    SceneObject<Mesh> mesh;
    SceneObject<Model> model;

public:
    DefaultSceneTwo();
    virtual void OnUpdate();
    void SetInitialUniforms(void);  // TODO
};