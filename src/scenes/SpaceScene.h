#pragma once

#include "src/pch/pch.h"
#include "src/Scene.h"


// tools.wwwtyro.net/space-3d/index.html
// Cubemap seed: 3cpuyabhwq00
class SpaceScene : public Honeycrisp::Scene
{
private:
    std::shared_ptr<Honeycrisp::Mesh> m_sphere;

public:
    SpaceScene();
    ~SpaceScene();
    void OnUpdate(const float& dt) override;

    std::shared_ptr<Honeycrisp::Mesh> GenerateCubeSphere(
        unsigned int resolution,
        float radius
    );

private:
    void SetInitialUniforms(void) override {};
    void OnImGui(void) override;
};