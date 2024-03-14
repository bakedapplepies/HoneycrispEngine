#pragma once

#include "src/pch/pch.h"
#include "src/Scene.h"


// 3cpuyabhwq00
class SpaceScene : public Honeycrisp::Scene
{
private:
    

public:
    SpaceScene();
    ~SpaceScene();
    SpaceScene(SpaceScene&& other) noexcept = default;
    SpaceScene& operator=(SpaceScene&& other) noexcept = default;
    void OnUpdate(const float& dt) override;
    void SetInitialUniforms(void) override {};
    void OnImGui(void) override {};
};