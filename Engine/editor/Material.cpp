#include "Material.h"

Material::Material()
{
    albedo = GLResource<Envy::Texture2D>::empty;
    pipeline = GLResource<Envy::Pipeline>::empty;
}

Material::~Material()
{}