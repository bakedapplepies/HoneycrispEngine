#include "api/graphics/DirectionalLight.h"


HNCRSP_NAMESPACE_START

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color,
    float ambient, float diffuse, float specular)
    : ILight(color), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
{
}

void DirectionalLight::ConfigureShader(const Shader* shader) const
{
    shader->SetVec3Unf("u_dir_light.direction", direction);
    shader->SetVec3Unf("u_dir_light.ambient", GetAmbient());
    shader->SetVec3Unf("u_dir_light.diffuse", GetDiffuse());
    shader->SetVec3Unf("u_dir_light.specular", GetSpecular());
}

glm::vec3 DirectionalLight::GetAmbient() const
{
    return ambient * colorEmit;
}

glm::vec3 DirectionalLight::GetDiffuse() const
{
    return diffuse * colorEmit;
}

glm::vec3 DirectionalLight::GetSpecular() const
{
    return specular * colorEmit;
}

HNCRSP_NAMESPACE_END