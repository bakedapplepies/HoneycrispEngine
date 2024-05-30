#include "SpotLight.h"


HNCRSP_NAMESPACE_START

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color,
    float ambient, float diffuse, float specular
) : Light(color), position(position), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
{
}

void SpotLight::ConfigureShader(const Shader* shader) const
{
    shader->setFloatUnf("u_spotLight.cutOff", cutOff);
    shader->setFloatUnf("u_spotLight.outerCutOff", outerCutOff);
    shader->setVec3Unf("u_spotLight.ambient", getAmbient());
    shader->setVec3Unf("u_spotLight.diffuse", getDiffuse());
    shader->setVec3Unf("u_spotLight.specular", getSpecular());
    shader->setFloatUnf("u_spotLight.constant", attenuation_constant);
    shader->setFloatUnf("u_spotLight.linear", attenuation_linear);
    shader->setFloatUnf("u_spotLight.quadratic", attenuation_quadratic);
}

glm::vec3 SpotLight::getAmbient() const
{
    return ambient * colorEmit;
}

glm::vec3 SpotLight::getDiffuse() const
{
    return diffuse * colorEmit;
}

glm::vec3 SpotLight::getSpecular() const
{
    return specular * colorEmit;
}

HNCRSP_NAMESPACE_END