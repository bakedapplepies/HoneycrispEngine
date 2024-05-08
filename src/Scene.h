#pragma once

#include "src/pch/pch.h"
#include "src/managers/ShaderManager.h"

#include "src/Renderable.h"
#include "src/graphics/Cubemap.h"
#include "src/SceneRenderObj.h"
#include "src/Light.h"


HNCRSP_NAMESPACE_START

class Scene
{
private:
    std::shared_ptr<Cubemap> m_cubemap;

protected:
    template <typename TRenderable, typename... Args>
    std::unique_ptr< SceneRenderObj<TRenderable> > CreateStaticRenderObj(Args&&... args)
    {
        static_assert(std::is_base_of<Renderable, TRenderable>(), "TRenderable is not base of Renderable.");

        return std::make_unique< SceneRenderObj<TRenderable> >(std::forward<Args>(args)...);
    }

    template <typename TLight, typename... Args>
    std::unique_ptr<TLight> CreateLight(Args&&... args)
    {
        static_assert(std::is_base_of<Light, TLight>(), "TLight is not base of Light.");

        std::unique_ptr<TLight> newLight = std::make_unique<TLight>(std::forward<Args>(args)...);
        
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