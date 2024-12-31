#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/managers/ShaderManager.h"

#include "src/Renderable.h"
#include "src/graphics/Cubemap.h"
#include "src/SceneRenderObj.h"
#include "src/graphics/DirectionalLight.h"
#include "src/graphics/PointLight.h"
#include "src/graphics/SpotLight.h"


HNCRSP_NAMESPACE_START

struct LightContainer
{
    std::array<std::unique_ptr<DirectionalLight>, MAX_DIRECTIONAL_LIGHTS> directionalLights;
    std::array<std::unique_ptr<PointLight>, MAX_POINT_LIGHTS> pointLights;
    std::array<std::unique_ptr<SpotLight>, MAX_SPOT_LIGHTS> spotLights;
};

class SceneManager;
class Scene
{
public:
    std::unique_ptr<Cubemap> cubemap;

private:
    friend SceneManager;
    std::vector<const Shader*> m_shadersInScene;
    LightContainer m_lightContainer = {
        .directionalLights = {},
        .pointLights = {},
        .spotLights = {}
    };

    uint32_t m_currentDirectionalLights = 0;
    uint32_t m_currentPointLights = 0;
    uint32_t m_currentSpotLights = 0;

protected:
    template <typename TRenderable, typename... Args>
    std::unique_ptr< SceneRenderObj<TRenderable> > CreateRenderObj(Args&&... args)
    {
        HNCRSP_STATIC_ASSERT(std::is_base_of<Renderable, TRenderable>(), "TRenderable is not base of Renderable.");

        return std::make_unique< SceneRenderObj<TRenderable> >(std::forward<Args>(args)...);
    }

    template <typename TLight, typename... Args>
    TLight* CreateLight(Args&&... args)
    {
        HNCRSP_STATIC_ASSERT(!std::is_same<Light, TLight>(), "TLight cannot be base Light class.");
        HNCRSP_STATIC_ASSERT(std::is_base_of<Light, TLight>(), "TLight is not base of Light.");

        std::unique_ptr<TLight> newLight = std::make_unique<TLight>(std::forward<Args>(args)...);
        TLight* newLightRawPtr = newLight.get();

        if constexpr(std::is_same<TLight, DirectionalLight>())
        {
            if (m_currentDirectionalLights >= MAX_DIRECTIONAL_LIGHTS)
            {
                HNCRSP_TERMINATE("Max Directional lights already reached.");
            }
            m_lightContainer.directionalLights[m_currentDirectionalLights] = std::move(newLight);
            m_currentDirectionalLights++;
        }
        else if constexpr(std::is_same<TLight, PointLight>())
        {
            if (m_currentPointLights >= MAX_POINT_LIGHTS)
            {
                HNCRSP_TERMINATE("Max Point lights already reached.");
            }
            m_lightContainer.pointLights[m_currentPointLights] = std::move(newLight);
            m_currentPointLights++;
        }
        else if constexpr(std::is_same<TLight, SpotLight>())
        {
            if (m_currentSpotLights >= MAX_SPOT_LIGHTS)
            {
                HNCRSP_TERMINATE("Max Spot lights already reached.");
            }
            m_lightContainer.spotLights[m_currentSpotLights] = std::move(newLight);
            m_currentSpotLights++;
        }
        
        for (auto& shader : m_shadersInScene)
        {
            newLightRawPtr->ConfigureShader(shader);
            shader->SetIntUnf("u_num_dir_light", m_currentDirectionalLights);
            shader->SetIntUnf("u_num_point_light", m_currentPointLights);
            shader->SetIntUnf("u_num_spot_light", m_currentSpotLights);  // 2 of these 3 are not used
        }                                                                // but who cares

        return newLightRawPtr;
    }

    void CreateCubemap(
        const FileSystem::Path& right,
        const FileSystem::Path& left,
        const FileSystem::Path& top,
        const FileSystem::Path& botttom,
        const FileSystem::Path& front,
        const FileSystem::Path& back
    );
    const Shader* CreateShader(
        const FileSystem::Path& vertex,
        const FileSystem::Path& fragment,
        const FileSystem::Path& geometry = FileSystem::Path("")
    );

public:
    Scene() = default;
    virtual ~Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&& other) noexcept = delete;
    Scene& operator=(Scene&& other) noexcept = delete;

    virtual void OnUpdate(const float& dt) = 0;
    virtual void OnImGui(void) {}

private:
    void _ReconfigureAllShaders() const;
};

HNCRSP_NAMESPACE_END