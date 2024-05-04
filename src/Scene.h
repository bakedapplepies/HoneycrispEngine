#pragma once

#include "src/pch/pch.h"
#include "src/ecs/ECSManager.h"
#include "src/managers/ShaderManager.h"
#include "src/Renderable.h"
#include "src/graphics/Model.h"
#include "src/graphics/Cubemap.h"
#include "src/SceneRenderObj.h"


HNCRSP_NAMESPACE_START

class Scene
{
private:
    std::shared_ptr<Cubemap> m_cubemap;

public:
    glm::vec3 bgColor = glm::vec3(0.0f, 0.0f, 0.0f);

protected:
    template <typename TRenderable, typename... Args>
    std::unique_ptr< SceneRenderObj<TRenderable> > CreateStaticRenderObj(Args&&... args)
    {
        static_assert(std::is_base_of<Renderable, TRenderable>());

        return std::make_unique< SceneRenderObj<TRenderable> >(std::forward<Args>(args)...);
    }

    void CreateCubemap(
        const Honeycrisp::FileSystem::Path& right,
        const Honeycrisp::FileSystem::Path& left,
        const Honeycrisp::FileSystem::Path& top,
        const Honeycrisp::FileSystem::Path& botttom,
        const Honeycrisp::FileSystem::Path& front,
        const Honeycrisp::FileSystem::Path& back
    );
    virtual void InitializeShaders(void) {}
    virtual void SetInitialUniforms(void) {}

public:
    Scene() = default;
    virtual ~Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&& other) noexcept = delete;
    Scene& operator=(Scene&& other) noexcept = delete;

    virtual void OnUpdate(const float& dt) = 0;
    virtual void OnImGui(void) {}
};

HNCRSP_NAMESPACE_END