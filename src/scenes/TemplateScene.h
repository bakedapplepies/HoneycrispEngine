#pragma once

#include "src/pch/pch.h"

#include "src/Scene.h"


class TemplateScene : public Honeycrisp::Scene
{
private:
    

public:
    TemplateScene();
    ~TemplateScene();
    TemplateScene(TemplateScene&& other) noexcept = default;
    TemplateScene& operator=(TemplateScene&& other) noexcept = default;
    void OnUpdate() override;
    void SetInitialUniforms(void) override {};
    void OnImGui(void) override {};
};