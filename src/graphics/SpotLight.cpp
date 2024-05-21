#include "SpotLight.h"


HNCRSP_NAMESPACE_START

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color,
    float ambient, float diffuse, float specular
) : position(position), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
{
}

void SpotLight::ConfigureShader(const Shader* shader) const
{
    shader->setVec3Unf("u_spotLight.position", position);
    shader->setVec3Unf("u_spotLight.direction", direction);
    shader->setFloatUnf("u_spotLight.cutOff", cutOff);
    shader->setFloatUnf("u_spotLight.outerCutOff", outerCutOff);
    shader->setVec3Unf("u_spotLight.ambient", getAmbient());
    shader->setVec3Unf("u_spotLight.diffuse", getDiffuse());
    shader->setVec3Unf("u_spotLight.specular", getSpecular());
    shader->setFloatUnf("u_spotLight.constant", attenuation_constant);
    shader->setFloatUnf("u_spotLight.linear", attenuation_linear);
    shader->setFloatUnf("u_spotLight.quadratic", attenuation_quadratic);
}

SpotLight::SpotLight(const SpotLight& other)
{
    position = other.position;
    direction = other.direction;
    colorEmit = other.colorEmit;
    ambient = other.ambient;
    diffuse = other.diffuse;
    specular = other.specular;
}

SpotLight& SpotLight::operator=(const SpotLight& other)
{
    position = other.position;
    direction = other.direction;
    colorEmit = other.colorEmit;
    ambient = other.ambient;
    diffuse = other.diffuse;
    specular = other.specular;

    return *this;
}

SpotLight::SpotLight(SpotLight&& other) noexcept
{
    position = std::move(other.position);
    direction = std::move(other.direction);
    colorEmit = std::move(other.colorEmit);
    ambient = std::move(other.ambient);
    diffuse = std::move(other.diffuse);
    specular = std::move(other.specular);

    other.position = glm::vec3(0.0f);
    other.direction = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
    other.ambient = 0.0f;
    other.diffuse = 0.0f;
    other.specular = 0.0f;
}

SpotLight& SpotLight::operator=(SpotLight&& other) noexcept
{
    position = std::move(other.position);
    direction = std::move(other.direction);
    colorEmit = std::move(other.colorEmit);
    ambient = std::move(other.ambient);
    diffuse = std::move(other.diffuse);
    specular = std::move(other.specular);

    other.position = glm::vec3(0.0f);
    other.direction = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
    other.ambient = 0.0f;
    other.diffuse = 0.0f;
    other.specular = 0.0f;

    return *this;
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