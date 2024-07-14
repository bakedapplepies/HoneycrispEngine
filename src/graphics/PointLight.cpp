#include "PointLight.h"


HNCRSP_NAMESPACE_START

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float ambient, float diffuse, float specular)
    : Light(color), position(position), ambient(ambient), diffuse(diffuse), specular(specular)
{}

void PointLight::ConfigureShader(const Shader* shader) const
{
    shader->setVec3Unf("u_point_light.position", position);
    shader->setVec3Unf("u_point_light.ambient", getAmbient());
    shader->setVec3Unf("u_point_light.diffuse", getDiffuse());
    shader->setVec3Unf("u_point_light.specular", getSpecular());
    shader->setFloatUnf("u_point_light.constant", attenuation_constant);
    shader->setFloatUnf("u_point_light.linear", attenuation_linear);
    shader->setFloatUnf("u_point_light.quadratic", attenuation_quadratic);
}

glm::vec3 PointLight::getAmbient() const
{
    return ambient * colorEmit;
}

glm::vec3 PointLight::getDiffuse() const
{
    return diffuse * colorEmit;
}

glm::vec3 PointLight::getSpecular() const
{
    return specular * colorEmit;
}

HNCRSP_NAMESPACE_END