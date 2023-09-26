#include "../Scene.h"


class DefaultScene : public Scene
{
private:
    SceneObject<Cube> cube;

public:
    DefaultScene();
    virtual void OnUpdate(Shader& shader);
};