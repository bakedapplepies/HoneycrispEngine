#include "../pch/pch.h"

#include "../Scene.h"
#include "../types/types_index.h"
#include "../Cube.h"
#include "../core/Texture.h"


class DefaultSceneTwo : public Scene
{
private:
    SceneObject<Cube> cube;
    SceneObject<Mesh> mesh;

public:
    DefaultSceneTwo();
    virtual void OnUpdate(Shader& shader);
};