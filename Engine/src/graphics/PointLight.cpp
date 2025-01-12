#include "api/graphics/PointLight.h"


HNCRSP_NAMESPACE_START

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float ambient, float diffuse, float specular) :
    ILight(color), position(position), ambient(ambient), diffuse(diffuse), specular(specular)
{}

void PointLight::ConfigureShader(const Shader* shader) const
{
    shader->SetVec3Unf("u_point_light.position", position);
    shader->SetVec3Unf("u_point_light.ambient", GetAmbient());
    shader->SetVec3Unf("u_point_light.diffuse", GetDiffuse());
    shader->SetVec3Unf("u_point_light.specular", GetSpecular());
    shader->SetFloatUnf("u_point_light.constant", attenuation_constant);
    shader->SetFloatUnf("u_point_light.linear", attenuation_linear);
    shader->SetFloatUnf("u_point_light.quadratic", attenuation_quadratic);
}

glm::vec3 PointLight::GetAmbient() const
{
    return ambient * colorEmit;
}

glm::vec3 PointLight::GetDiffuse() const
{
    return diffuse * colorEmit;
}

glm::vec3 PointLight::GetSpecular() const
{
    return specular * colorEmit;
}

HNCRSP_NAMESPACE_END