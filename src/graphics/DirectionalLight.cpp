#include "DirectionalLight.h"


HNCRSP_NAMESPACE_START

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color,
    float ambient, float diffuse, float specular)
    : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
{
}

void DirectionalLight::ConfigureShader(const Shader* shader) const
{
    shader->setVec3Unf("u_dirLight.direction", direction);
    shader->setVec3Unf("u_dirLight.ambient", getAmbient());
    shader->setVec3Unf("u_dirLight.diffuse", getDiffuse());
    shader->setVec3Unf("u_dirLight.specular", getSpecular());
}

DirectionalLight::DirectionalLight(const DirectionalLight& other)
{
    direction = other.direction;
    colorEmit = other.colorEmit;
    ambient = other.ambient;
    diffuse = other.diffuse;
    specular = other.specular;
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& other)
{
    direction = other.direction;
    colorEmit = other.colorEmit;
    ambient = other.ambient;
    diffuse = other.diffuse;
    specular = other.specular;

    return *this;
}

DirectionalLight::DirectionalLight(DirectionalLight&& other) noexcept
{
    direction = std::move(other.direction);
    colorEmit = std::move(other.colorEmit);
    ambient = std::move(other.ambient);
    diffuse = std::move(other.diffuse);
    specular = std::move(other.specular);

    other.direction = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
    other.ambient = 0.0f;
    other.diffuse = 0.0f;
    other.specular = 0.0f;
}

DirectionalLight& DirectionalLight::operator=(DirectionalLight&& other) noexcept
{
    direction = std::move(other.direction);
    colorEmit = std::move(other.colorEmit);
    ambient = std::move(other.ambient);
    diffuse = std::move(other.diffuse);
    specular = std::move(other.specular);
    
    other.direction = glm::vec3(0.0f);
    other.colorEmit = glm::vec3(0.0f);
    other.ambient = 0.0f;
    other.diffuse = 0.0f;
    other.specular = 0.0f;

    return *this;
}

HNCRSP_NAMESPACE_END