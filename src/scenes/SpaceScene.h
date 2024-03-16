#pragma once

#include "src/pch/pch.h"
#include "src/Scene.h"


// Cubemap seed: 3cpuyabhwq00
// tools.wwwtyro.net/space-3d/index.html
class SpaceScene : public Honeycrisp::Scene
{
private:
    

public:
    SpaceScene();
    ~SpaceScene();
    void OnUpdate(const float& dt) override;
    void SetInitialUniforms(void) override {};
    void OnImGui(void) override {};
};