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

    /*                      size    offset  total_size
        float u_exposure  : 4       0       4
    */
    m_postprocessUBO = envy_instance->CreateUBO(4, UBO_BINDING_INDEX_POSTPROCESS);
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