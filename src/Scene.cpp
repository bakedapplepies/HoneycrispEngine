#include "Scene.h"


HNCRSP_NAMESPACE_START


// Since all attributes are private and the class only
// exposes update methods,
// cubemaps can be created at anytime as the current g_ECSManager
// set by SceneManager will be sure to set the current scene and THEN update everything.
void Scene::CreateCubemap(
    const FileSystem::Path& right,
    const FileSystem::Path& left,
    const FileSystem::Path& top,
    const FileSystem::Path& bottom,
    const FileSystem::Path& front,
    const FileSystem::Path& back
) {
    std::array<FileSystem::Path, 6> cubemapFaces = {
        right, left, top, bottom, front, back  
    };

    m_cubemap = std::make_shared<Cubemap>(cubemapFaces);
    g_ECSManager->Renderer_SetCubemap(m_cubemap);
}

HNCRSP_NAMESPACE_END