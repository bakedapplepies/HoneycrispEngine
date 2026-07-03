#include "Material.h"

HNCRSP_NAMESPACE_START

Material::Material()
{
    albedo = GLResource<Envy::Texture2D>::empty;
    normal = GLResource<Envy::Texture2D>::empty;
    roughness = GLResource<Envy::Texture2D>::empty;
    pipeline = GLResource<Envy::Pipeline>::empty;
}

Material::~Material()
{}

HNCRSP_NAMESPACE_END