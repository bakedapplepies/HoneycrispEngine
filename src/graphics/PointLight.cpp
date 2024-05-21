#include "PointLight.h"


HNCRSP_NAMESPACE_START

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float ambient, float diffuse, float specular)
    : Light(color), position(position), ambient(ambient), diffuse(diffuse), specular(specular)
{}

void PointLight::ConfigureShader(const Shader* shader) const
{
    shader->setVec3Unf("u_pointLight.position", position);
    shader->setVec3Unf("u_pointLight.ambient", getAmbient());
    shader->setVec3Unf("u_pointLight.diffuse", getDiffuse());
    shader->setVec3Unf("u_pointLight.specular", getSpecular());
    shader->setFloatUnf("u_pointLight.constant", attenuation_constant);
    shader->setFloatUnf("u_pointLight.linear", attenuation_linear);
    shader->setFloatUnf("u_pointLight.quadratic", attenuation_quadratic);
}

PointLight::PointLight(const PointLight& other)
{
    position = other.position;
    colorEmit = other.colorEmit;
    ambient = other.ambient;
    diffuse = other.diffuse;
    specular = other.specular;
}

PointLight& PointLight::operator=(const PointLight& other)
{
    position = other.position;
    colorEmit = other.colorEmit;
    ambient = other.ambient;
    diffuse = other.diffuse;
    specular = other.specular;

    return *this;
}

PointLight::PointLight(PointLight&& other) noexcept
{
    position = std::move(other.position);
    colorEmit = std::move(other.colorEmit);
    ambient = std::move(other.ambient);
    diffuse = std::move(other.diffuse);
    specular = std::move(other.specular);

    other.position = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
    other.ambient = 0.0f;
    other.diffuse = 0.0f;
    other.specular = 0.0f;
}

PointLight& PointLight::operator=(PointLight&& other) noexcept
{
    position = std::move(other.position);
    colorEmit = std::move(other.colorEmit);
    ambient = std::move(other.ambient);
    diffuse = std::move(other.diffuse);
    specular = std::move(other.specular);

    other.position = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
    other.ambient = 0.0f;
    other.diffuse = 0.0f;
    other.specular = 0.0f;

    return *this;
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