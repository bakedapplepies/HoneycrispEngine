#include "SpotLight.h"


HNCRSP_NAMESPACE_START

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color,
    float ambient, float diffuse, float specular
) : Light(color), position(position), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
{
}

void SpotLight::ConfigureShader(const Shader* shader) const
{
    shader->SetFloatUnf("u_spot_light.cutOff", cutOff);
    shader->SetFloatUnf("u_spot_light.outerCutOff", outerCutOff);
    shader->SetVec3Unf("u_spot_light.ambient", getAmbient());
    shader->SetVec3Unf("u_spot_light.diffuse", getDiffuse());
    shader->SetVec3Unf("u_spot_light.specular", getSpecular());
    shader->SetFloatUnf("u_spot_light.constant", attenuation_constant);
    shader->SetFloatUnf("u_spot_light.linear", attenuation_linear);
    shader->SetFloatUnf("u_spot_light.quadratic", attenuation_quadratic);
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