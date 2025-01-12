#pragma once

#include "api/pch/hncrsp_pch.h"

#include "api/scene/Scene.h"


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