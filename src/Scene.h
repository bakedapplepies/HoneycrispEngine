#pragma once

#include "src/pch/pch.h"
#include "src/managers/ShaderManager.h"

#include "src/Renderable.h"
#include "src/graphics/Cubemap.h"
#include "src/SceneRenderObj.h"
#include "src/graphics/DirectionalLight.h"
#include "src/graphics/PointLight.h"
#include "src/graphics/SpotLight.h"


HNCRSP_NAMESPACE_START

static uint32_t currentDirectionalLights = 0;
static uint32_t currentPointLights = 0;
static uint32_t currentSpotLights = 0;

class Scene
{
private:
    std::shared_ptr<Cubemap> m_cubemap;
    std::vector< std::shared_ptr<Shader> > m_shadersInScene;
    std::vector< std::shared_ptr<Light> > m_lightsInscene;

protected:
    template <typename TRenderable, typename... Args>
    std::unique_ptr< SceneRenderObj<TRenderable> > CreateStaticRenderObj(Args&&... args)
    {
        static_assert(std::is_base_of<Renderable, TRenderable>(), "TRenderable is not base of Renderable.");

        return std::make_unique< SceneRenderObj<TRenderable> >(std::forward<Args>(args)...);
    }

    template <typename TLight, typename... Args>
    std::shared_ptr<TLight> CreateLight(Args&&... args)
    {
        static_assert(!std::is_same<Light, TLight>(), "TLight cannot be base Light class.");
        static_assert(std::is_base_of<Light, TLight>(), "TLight is not base of Light.");

        if constexpr(std::is_same<TLight, DirectionalLight>())
        {
            if (currentDirectionalLights >= MAX_DIRECTIONAL_LIGHTS)
            {
                HNCRSP_TERMINATE("Max Directional lights already reached.");
            }
            currentDirectionalLights++;
        }
        else if constexpr(std::is_same<TLight, PointLight>())
        {
            if (currentPointLights >= MAX_POINT_LIGHTS)
            {
                HNCRSP_TERMINATE("Max Point lights already reached.");
            }
            currentPointLights++;
        }
        else if constexpr(std::is_same<TLight, SpotLight>())
        {
            if (currentSpotLights >= MAX_SPOT_LIGHTS)
            {
                HNCRSP_TERMINATE("Max Spot lights already reached.");
            }
            currentSpotLights++;
        }

        std::shared_ptr<Light> newLight = std::make_shared<TLight>(std::forward<Args>(args)...);
        m_lightsInscene.push_back(newLight);
        // newLight->AddShaders(m_shadersInScene);
        
        for (auto& shader : m_shadersInScene)
        {
            newLight->ConfigureShader(shader.get());
        }

        return std::reinterpret_pointer_cast<TLight>(newLight);
    }

    void CreateCubemap(
        const FileSystem::Path& right,
        const FileSystem::Path& left,
        const FileSystem::Path& top,
        const FileSystem::Path& botttom,
        const FileSystem::Path& front,
        const FileSystem::Path& back
    );
    std::shared_ptr<Shader> CreateShader(
        const FileSystem::Path& vertex,
        const FileSystem::Path& fragment,
        const FileSystem::Path& geometry = FileSystem::Path("")
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