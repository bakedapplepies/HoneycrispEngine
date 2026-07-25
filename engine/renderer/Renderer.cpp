#include "Renderer.h"

#include <cassert>

#include "core/Constants.h"

HNCRSP_NAMESPACE_START

bool Renderer::m_rendererSetup = false;

Renderer::Renderer(const Envy::EnvyInstance* envy_instance, int width, int height)
    : m_width(width), m_height(height)
{
    assert(!m_rendererSetup && "A renderer has already existed.");
    m_rendererSetup = true;

    /*                      size    offset  total_size
        float u_time      : 4       0
        mat4 u_view       : 64      16
        mat4 u_projection : 64      80
        vec4 u_cameraPos  : 16      144
        mat4 u_lightSpace : 64      160     224
    */
    m_globalUBO = envy_instance->CreateUBO(224, UBO_BINDING_INDEX_GLOBAL);

    /*                                               size    offset  total_size
        DirLight u_light                           : 16      0
        uint u_numPointLight                       : 4       16
        PointLight u_pointLights[MAX_POINT_LIGHTS] : 320     32      352
    */
    m_lightUBO = envy_instance->CreateUBO(352, UBO_BINDING_INDEX_LIGHT);

    /*                               size    offset  total_size
        float u_ambient_intensity  : 4       0
        float u_diffuse_intensity  : 4       16
        float u_specular_intensity : 4       32
        float u_roughness_scalar   : 4       48      52
    */
    m_materialUBO = envy_instance->CreateUBO(64, UBO_BINDING_INDEX_MATERIAL);

    /*                           size    offset  total_size
        float u_exposure       : 4       0       
        float u_saturation     : 4       4       
        float u_bloomThres     : 4       8
        float u_bloomFilterRad : 4       12       16
    */
    m_postprocessUBO = envy_instance->CreateUBO(16, UBO_BINDING_INDEX_POSTPROCESS);
}

void Renderer::UpdateDirLight(const DirLight& dir_light) const
{
    m_lightUBO->UploadData(offsetof(LightUBO, u_dirLight), 16, &dir_light);
}

void Renderer::UpdatePointLights(const std::vector<PointLight>& point_lights) const
{
    // m_pointLights = std::move(point_lights);

    uint32_t pointLightNum = point_lights.size();
    m_lightUBO->UploadData(offsetof(LightUBO, u_numPointLight), 4, &pointLightNum);
    for (uint32_t i = 0; i < point_lights.size(); i++)
    {
        m_lightUBO->UploadData(offsetof(LightUBO, u_pointLights) + 16 * i, 16, point_lights.data() + i);
    }
}

void Renderer::UpdateGlobalMatParam(const MaterialUBO& mat_params) const
{
    m_materialUBO->UploadData(offsetof(MaterialUBO, u_ambient_intensity), 4, &mat_params.u_ambient_intensity);
    m_materialUBO->UploadData(offsetof(MaterialUBO, u_diffuse_intensity), 4, &mat_params.u_diffuse_intensity);
    m_materialUBO->UploadData(offsetof(MaterialUBO, u_specular_intensity), 4, &mat_params.u_specular_intensity);
    m_materialUBO->UploadData(offsetof(MaterialUBO, u_roughness_scalar), 4, &mat_params.u_roughness_scalar);
}

void Renderer::UpdatePostprocessParam(const PostprocessUBO& postprocess_params) const
{
    // m_postprocessUBO->UploadData(offsetof(PostprocessUBO, u_exposure), 4, &postprocess_params.u_exposure);
    // m_postprocessUBO->UploadData(offsetof(PostprocessUBO, u_saturation), 4, &postprocess_params.u_exposure);
    // m_postprocessUBO->UploadData(offsetof(PostprocessUBO, u_bloomThres), 4, &postprocess_params.u_bloomThres);
    // m_postprocessUBO->UploadData(offsetof(PostprocessUBO, u_bloomFilterRad), 4, &postprocess_params.u_bloomFilterRad);
    m_postprocessUBO->UploadData(0, sizeof(PostprocessUBO), &postprocess_params);
}

Renderer::~Renderer()
{
    m_rendererSetup = false;
    m_globalUBO = GLResource<Envy::UniformBuffer>::empty;
    m_lightUBO = GLResource<Envy::UniformBuffer>::empty;
    m_materialUBO = GLResource<Envy::UniformBuffer>::empty;
    m_postprocessUBO = GLResource<Envy::UniformBuffer>::empty;
}

HNCRSP_NAMESPACE_END