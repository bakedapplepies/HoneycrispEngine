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

public:
    DefaultScene();
    virtual void OnUpdate(Shader& shader);
    void SetInitialUniforms(void);
};