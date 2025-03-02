#pragma once

#include "api/pch/hncrsp_pch.h"

#include "api/scene/IRenderable.h"
#include "api/scene/SceneRenderObj.h"
#include "api/managers/ShaderManager.h"
#include "api/graphics/Cubemap.h"
#include "api/graphics/DirectionalLight.h"
#include "api/graphics/PointLight.h"
#include "api/graphics/SpotLight.h"

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
private:
    friend SceneManager;
    std::vector<const Shader*> m_shadersInScene;
    LightContainer m_lightContainer = {
        .directionalLights = {},
        .pointLights = {},
        .spotLights = {}
    };

    std::unique_ptr<Cubemap> m_cubemap;

    uint32_t m_currentDirectionalLights = 0;
    uint32_t m_currentPointLights = 0;
    uint32_t m_currentSpotLights = 0;

    const char* m_sceneName;

protected:
    template <typename TIRenderable, typename... Args>
    std::unique_ptr< SceneRenderObj<TIRenderable> > CreateRenderObj(const Material& material, Args&&... args)
    {
        HNCRSP_STATIC_ASSERT(std::is_base_of<IRenderable, TIRenderable>(), "TIRenderable is not base of IRenderable.");

        return std::make_unique< SceneRenderObj<TIRenderable> >(material, std::forward<Args>(args)...);
    }
    
    template <typename TIRenderable, typename... Args>
    std::unique_ptr< SceneRenderObj<TIRenderable> > CreateModel(Args&&... args)
    {
        HNCRSP_STATIC_ASSERT(std::is_base_of<IRenderable, TIRenderable>(), "TIRenderable is not base of IRenderable.");
    
        Material material(g_ShaderManager.basicShader);
        return std::make_unique< SceneRenderObj<TIRenderable> >(material, std::forward<Args>(args)...);
    }

    template <typename TLight, typename... Args>
    TLight* CreateLight(Args&&... args)  // TODO: Replace template with enums to better hide implementation details
    {
        HNCRSP_STATIC_ASSERT(!std::is_same<ILight, TLight>(), "TLight cannot be base ILight class.");
        HNCRSP_STATIC_ASSERT(std::is_base_of<ILight, TLight>(), "TLight is not base of ILight.");

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
    
    // To be used in scene files, not g_ShaderManager.GetShader()
    const Shader* CreateShader(
        const FileSystem::Path& vertex,
        const FileSystem::Path& fragment,
        const FileSystem::Path& geometry = FileSystem::Path("")
    );
    void UpdateLight(const ILight* light) const;

public:
    Scene(const char* scene_name);
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