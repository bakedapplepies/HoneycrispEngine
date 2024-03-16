#include "SpaceScene.h"


using namespace Honeycrisp;

SpaceScene::SpaceScene()
{
    CreateCubemap(
        FileSystem::Path("resources/textures/cubemaps/space/right.png"),
        FileSystem::Path("resources/textures/cubemaps/space/left.png"),
        FileSystem::Path("resources/textures/cubemaps/space/top.png"),
        FileSystem::Path("resources/textures/cubemaps/space/bottom.png"),
        FileSystem::Path("resources/textures/cubemaps/space/front.png"),
        FileSystem::Path("resources/textures/cubemaps/space/back.png")
    );
}

SpaceScene::~SpaceScene() {}

void SpaceScene::OnUpdate(const float& dt)
{
    DrawCubemap();
}