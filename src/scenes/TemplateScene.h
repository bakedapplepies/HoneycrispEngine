#pragma once

#include "src/pch/pch.h"

#include "src/Scene.h"


class TemplateScene : public Honeycrisp::Scene
{
private:
    

public:
    TemplateScene();
    ~TemplateScene();
    void OnUpdate(const float& dt) override;
    void SetInitialUniforms(void) override {};
    void OnImGui(void) override {};
};