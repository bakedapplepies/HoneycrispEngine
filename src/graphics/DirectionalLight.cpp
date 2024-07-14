#include "DirectionalLight.h"


HNCRSP_NAMESPACE_START

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color,
    float ambient, float diffuse, float specular)
    : Light(color), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
{
}

void DirectionalLight::ConfigureShader(const Shader* shader) const
{
    shader->setVec3Unf("u_dir_light.direction", direction);
    shader->setVec3Unf("u_dir_light.ambient", getAmbient());
    shader->setVec3Unf("u_dir_light.diffuse", getDiffuse());
    shader->setVec3Unf("u_dir_light.specular", getSpecular());
}

HNCRSP_NAMESPACE_END