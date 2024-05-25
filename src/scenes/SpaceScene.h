#pragma once

#include "src/pch/pch.h"
#include "src/Scene.h"


// tools.wwwtyro.net/space-3d/index.html
// Cubemap seed: 3cpuyabhwq00
class SpaceScene : public Honeycrisp::Scene
{
private:
    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Mesh> > m_sphere;
    float m_sphere_radius = 2.0f;
    int m_sphere_resolution = 4;
    bool m_change_sphere;

public:
    SpaceScene();
    ~SpaceScene() = default;
    void OnUpdate(const float& dt) override;

    std::unique_ptr< Honeycrisp::SceneRenderObj<Honeycrisp::Mesh> > GenerateCubeSphere(
        unsigned int resolution,
        float radius
    );

private:
    void OnImGui(void) override;
};