#pragma once

#include "../pch/pch.h"

#include "../Scene.h"
#include "../core/Shader.h"
#include "../Cube.h"


class DefaultScene : public Scene
{
private:
    std::shared_ptr<Shader> shader;
    SceneObject<Cube> cube;
    SceneObject<Mesh> customMesh;

public:
    DefaultScene();
    virtual void OnUpdate();
    void SetInitialUniforms(void);
};