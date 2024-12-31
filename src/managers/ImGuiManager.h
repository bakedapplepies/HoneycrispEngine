#pragma once

#include "src/pch/hncrsp_pch.h"
#include "src/systems/Renderer.h"


HNCRSP_NAMESPACE_START

class ImGuiManager
{
private:
    Renderer* m_renderer;

public:
    ImGuiManager() = default;
    ImGuiManager(const ImGuiManager&) = delete;
    ImGuiManager& operator=(const ImGuiManager&) = delete;
    ImGuiManager(ImGuiManager&&) = delete;
    ImGuiManager& operator=(ImGuiManager&&) = delete;
    ~ImGuiManager() = default;

public:
    void StartUp();
    void ShutDown();

    void Update();

private:
    void _NewFrame();
    void _Render();
};

extern ImGuiManager g_ImGuiManager;

HNCRSP_NAMESPACE_END