#pragma once

#include "src/pch/pch.h"

#include "../Scene.h"
#include "../core/Shader.h"
#include "../Cube.h"


class DefaultScene : public Scene
{
private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr< SceneObject<Cube> > cube;
    std::shared_ptr< SceneObject<Mesh> > customMesh;

public:
    DefaultScene();
    ~DefaultScene();
    DefaultScene(DefaultScene&& other) noexcept = default;
    DefaultScene& operator=(DefaultScene&& other) noexcept = default;
    virtual void onUpdate();
    void SetInitialUniforms(void);
};